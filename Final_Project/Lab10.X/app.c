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
#include "app.h"
#include "gpio.h"
#include "uart.h"

#define MAX_PROTOCOL_LENGTH 8

char protocol[MAX_PROTOCOL_LENGTH];
int protocol_length;

void receive_protocol(void)
{
    protocol_length = 0;
    while(ch_new == 0); // wait for first character of protocol
    while(ch_rx != '!') // get protocol characters until end of string ('!') is indicated
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
            default:
            {
                break;
            }
        }
    }
}

void run_app(void)
{
    turn_on(15);
    turn_on(14);
    
    ch_tx = 'r';
    uart_send();
    
    while(1)
    {
        receive_protocol();
        process_protocol();
    }
}
