/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: taskUART.c   
 * Author: Anthony Bloch
 * Comments: transceiver task for uart. Receives protocol via uart and processes them accordingly
 * Revision history: 
 */

#include <stdlib.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "uart.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>
#include "global_variables.h"

#include "taskUART.h"

#include "../mcc_generated_files/pin_manager.h"


#define taskUART_PRIORITY        0
#define taskUART_STACK_SIZE      512

#define MAX_PROTOCOL_LENGTH 8
#define PROTOCOL_BEGIN '<'
#define PROTOCOL_END '>'
#define ENABLED 1
#define DISABLED 0
#define CH_1 0
#define CH_2 1
#define CH_3 5

char protocol[MAX_PROTOCOL_LENGTH];
int protocol_length;
int periodic_pulse_width = 1;
int periodic_status = DISABLED;
int periodic_count = 0;
int led_red_status = OFF;
int timed_status = DISABLED;
int timed_duration = 1;
int timed_count = 0;
int led_yellow_status = OFF;

void receive_protocol(void)
{
    ch_tx = '!'; // signal gui to send rest of command
    uart_send();
    ch_new = 0;
    while(ch_new == 0); // wait for first character of protocol
    protocol_length = 0;
    while(ch_rx != PROTOCOL_END) // get protocol characters until end of string ('!') is indicated
    {
        if(protocol_length < MAX_PROTOCOL_LENGTH) // save only the first MAX_PROTOCOL_LENGTH characters of protocol
        {
            protocol[protocol_length] = ch_rx; // save current character of protocol
            protocol_length++;
        }
        ch_new = 0;
        while(ch_new == 0); // wait for next character in protocol
    }
    ch_new = 0;
}

void process_protocol(void)
{
    int i, value;
    if(protocol > 0) // protects against receiving only '!'
    {
        switch(protocol[0])
        {
            case 'a': // turn on LED
            {
                turn_on(LED_GREEN);
                break;
            }
            case 'b': // turn off LED
            {
                turn_off(LED_GREEN);
                break; 
            }
            case 'c': // enable timed output
            {
                timed_status = ENABLED;
                timed_duration = 0;
                timed_count = 0;
                for(i = 1; i < protocol_length; i++)
                {
                    timed_duration += (int)(protocol[i]-'0')*(int)pow(10, (protocol_length-i));
                }
                break;
            }
            case 'd': // disable timed output
            {
                timed_status = DISABLED;
                turn_off(LED_YELLOW);
                break;
            }
            case 'e': // check status of timed output
            {
                ch_tx = PROTOCOL_BEGIN;
                uart_send();
                ch_tx = 'e';
                uart_send();
                if(status(LED_YELLOW) == 1)
                {
                    ch_tx = '1';
                }
                else
                {
                    ch_tx = '0';
                }
                uart_send();
                ch_tx = PROTOCOL_END;
                uart_send();
                break;
            }
            case 'f': // enable periodic output
            {
                periodic_status = ENABLED;
                periodic_pulse_width = 0;
                periodic_count = 0;
                for(i = 1; i < protocol_length; i++)
                {
                    periodic_pulse_width += (int)(protocol[i]-'0')*(int)pow(10, (protocol_length-i));
                }
                break;
            }
            case 'g': // disable periodic output
            {
                periodic_status = DISABLED;
                turn_off(LED_RED);
                break;
            }
            case 'h': // read digital
            {
                ch_tx = PROTOCOL_BEGIN;
                uart_send();
                ch_tx = 'h';
                uart_send();
                if(check_digital() == 1)
                {
                    ch_tx = '1';
                }
                else
                {
                    ch_tx = '0';
                }
                uart_send();
                ch_tx = PROTOCOL_END;
                uart_send();
                break;
            }
            case 'i': // read adc channel 1
            {
                value = check_analog(CH_1);
                ch_tx = PROTOCOL_BEGIN;
                uart_send();
                ch_tx = 'i';
                uart_send();
                i = 1000;
                while(i > 0)
                {
                    ch_tx = value/i + '0';
                    uart_send();
                    value = value % i;
                    i = i/10;
                }
                ch_tx = PROTOCOL_END;
                uart_send();
                break;
            }
            case 'j': // read adc channel 2
            {
                value = check_analog(CH_2);
                ch_tx = PROTOCOL_BEGIN;
                uart_send();
                ch_tx = 'j';
                uart_send();
                i = 1000;
                while(i > 0)
                {
                    ch_tx = value/i + '0';
                    uart_send();
                    value = value % i;
                    i = i/10;
                }
                ch_tx = PROTOCOL_END;
                uart_send();
                break;
            }
            case 'k': // read adc channel 3
            {
                value = check_analog(CH_3);
                ch_tx = PROTOCOL_BEGIN;
                uart_send();
                ch_tx = 'k';
                uart_send();
                i = 1000;
                while(i > 0)
                {
                    ch_tx = value/i + '0';
                    uart_send();
                    value = value % i;
                    i = i/10;
                }
                ch_tx = PROTOCOL_END;
                uart_send();
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

/*****************************************************************************

    Private functions prototype

*****************************************************************************/
static portTASK_FUNCTION(vTaskUART, pvParameters);

/*****************************************************************************

    FreeRTOS Task implementation

*****************************************************************************/
static inline void vCreatNewTasks(unsigned portBASE_TYPE uxPriority)
{
    xTaskCreate(vTaskUART, (char const*)"taskName", (uint16_t)taskUART_STACK_SIZE/(uint16_t)(2), NULL, uxPriority, (TaskHandle_t *) NULL );
}

/*****************************************************************************

    Public functions implementation.

*****************************************************************************/
void taskUART_init(void)
{
    vCreatNewTasks(taskUART_PRIORITY);
}

static portTASK_FUNCTION(vTaskUART, pvParameters)
{
    /* Just to stop compiler warnings. */
    (void) pvParameters;
    //===========================================
    //Task entrance
    //===========================================
    while(1)
    {
        if(protocol_new == 1)
        {
            if(ch_rx == PROTOCOL_BEGIN) //signifies beginning of protocol
            {
                receive_protocol();
                process_protocol();
            }
            protocol_new = 0;
        }
        vTaskDelay(1);
    }
}