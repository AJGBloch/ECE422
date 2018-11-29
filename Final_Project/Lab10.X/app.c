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
 * File: app.c
 * Author: Anthony Bloch
 * Comments:
 * Revision history: 
 */
#include <math.h>
#include "app.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"

#define MAX_PROTOCOL_LENGTH 8
#define PROTOCOL_BEGIN '<'
#define PROTOCOL_END '>'
#define ENABLED 1
#define DISABLED 0

char protocol[MAX_PROTOCOL_LENGTH];
int protocol_length;
int periodic_pulse_width = 1;
int periodic_status = DISABLED;
int led_red_status = OFF;
int timed_status = DISABLED;
int timed_duration = 1;
int timed_count = 0;
int led_yellow_status = OFF;

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
    int i;
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
                t1_periods_2 = 0;
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
                t1_periods_1 = 0;
                break;
            }
            case 'g':
            {
                periodic_status = DISABLED;
                turn_off(LED_RED);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void run_app(void)
{
    timer_init();
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
        if(periodic_status == ENABLED)
        {
            if(t1_periods_1 >= (ms_1000/periodic_pulse_width))
            {
                toggle(LED_RED);
                t1_periods_1 = 0;
            }
        }
        if(timed_status == ENABLED)
        {
            if(t1_periods_2 >= ms_100/6) //62520
            {
                timed_count++;
                if(timed_count >= timed_duration)
                {
                    toggle(LED_YELLOW);
                    timed_status = DISABLED;
                }
                t1_periods_2 = 0;
            }
        }
    }
}
