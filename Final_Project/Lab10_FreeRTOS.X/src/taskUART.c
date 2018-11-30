
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
 * Comments: creates an independent task to do the following:
 * With the push of a button on the board, the LED will toggle on/off.
 * When 'a' is entered on the computer's keyboard on the serial terminal, LED2 will activate/deactivate.
 * When 's' is entered, the status (on/off) is reported on the terminal.
 * When 'c' is entered, the number of times (count) the button or 'a' have been pressed is reported on the terminal.
 * When 'r' is entered, the counter tracking the number of times the button or 't' has been pressed is reset to 0.
 * There is also a reset push button on the board that resets the system when pushed.
 * When there is UART activity, LED1 will turn on. Otherwise it is off.
 * 
 * Revision history: 
 */

#include <stdlib.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "taskUART.h"
#include "gpio.h"
//#include "interrupt.h"
#include "uart.h"
#include <math.h>
//#include "timer.h"
#include "../taskTimedOutput.h"
#include "../taskPeriodicPulse.h"

#include "../mcc_generated_files/pin_manager.h"


#define taskPRIORITY        1
#define taskSTACK_SIZE      512

#define MAX_PROTOCOL_LENGTH 8
#define PROTOCOL_BEGIN '<'
#define PROTOCOL_END '>'

char protocol[MAX_PROTOCOL_LENGTH];
int protocol_length;

/*****************************************************************************

    Private functions prototype

*****************************************************************************/
static portTASK_FUNCTION(vTaskUART, pvParameters);

/*****************************************************************************

    FreeRTOS Task implementation

*****************************************************************************/
static inline void vCreatNewTasks(unsigned portBASE_TYPE uxPriority)
{
    xTaskCreate(vTaskUART, (char const*)"taskName", (uint16_t)taskSTACK_SIZE/(uint16_t)(2), NULL, uxPriority, (TaskHandle_t *) NULL );
}

/*****************************************************************************

    Public functions implementation.

*****************************************************************************/
void taskUART_init(void)
{
    vCreatNewTasks(taskPRIORITY);
}

void receive_protocol(void)
{
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
            case 'a':
            {
                turn_on(LED_GREEN);
                break;
            }
            case 'b':
            {
                turn_off(LED_GREEN);
                break; 
            }
            case 'c':
            {
                timed_status = ENABLED;
                timed_duration = 0;
                timed_count = 0;
                for(i = 1; i < protocol_length; i++)
                {
                    timed_duration += (int)(protocol[i]-'0')*(int)pow(10, (protocol_length-i));
                }
                turn_on(LED_YELLOW);
                //t1_periods_2 = 0;
                break;
            }
            case 'd':
            {
                timed_status = DISABLED;
                turn_off(LED_YELLOW);
                break;
            }
            case 'e':
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
            case 'f':
            {
                periodic_status = ENABLED;
                periodic_pulse_width = 0;
                for(i = 1; i < protocol_length; i++)
                {
                    periodic_pulse_width += (int)(protocol[i]-'0')*(int)pow(10, (protocol_length-i));
                }
                turn_on(LED_RED);
                //t1_periods_1 = 0;
                break;
            }
            case 'g':
            {
                periodic_status = DISABLED;
                turn_off(LED_RED);
                break;
            }
            case 'h':
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
            case 'i':
            {
                value = check_analog(3);
                ch_tx = PROTOCOL_BEGIN;
                uart_send();
                ch_tx = 'i';
                uart_send();
                ch_tx = '~';
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
            case 'j':
            {
                value = check_analog(4);
                ch_tx = PROTOCOL_BEGIN;
                uart_send();
                ch_tx = 'j';
                uart_send();
                ch_tx = '~';
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
            case 'k':
            {
                value = check_analog(5);
                ch_tx = PROTOCOL_BEGIN;
                uart_send();
                ch_tx = 'k';
                uart_send();
                ch_tx = '~';
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

static portTASK_FUNCTION(vTaskUART, pvParameters)
{
    /* Just to stop compiler warnings. */
    (void) pvParameters;

    //===========================================
    //Task entrance
    //===========================================
    uart_init();
    turn_on(LED_GREEN);
    turn_on(LED_YELLOW);
    turn_on(LED_RED);
    while(1)
    {
        if(ch_new == 1)
        {
            if(ch_rx == PROTOCOL_BEGIN) //signifies beginning of protocol
            {
                receive_protocol();
                process_protocol();
            }
            ch_new = 0;
        }
    }    
}