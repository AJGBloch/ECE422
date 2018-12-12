
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
 * File: gpio.c   
 * Author: Anthony Bloch
 * Comments: handles GPIO for PIC24
 * Revision history: 
 */

#include "gpio.h"

void gpio_init(void)
{
   LATB = 0x0000;                    // initialize output to be off
   TRISB = 0x0FFF;              // RP12, 13, 14, 15 are output, the rest are input
   TRISA = 0xFFFF;             // all are input
   ANSB = 0x0FFF;               //RP12, 13, 14, 15 are digital
   ANSA = 0xFFFB;               // RA2 is digital
}


//turn on RPx where x is LED
void turn_on(int LED)
{
    LATB |= 0x0001 << LED;
}

//turn off RPx where x is LED
void turn_off(int LED)
{
    LATB &= (~(0x0001 << LED));
}

//toggle RPx where x is LED
void toggle(int LED)
{
    if(((LATB >> LED)&0x0001) == 0) // check if LED is off
    {
        turn_on(LED);
    }
    else
    {
        turn_off(LED);
    }
}

//checks the status of RPx where x is LED
int status(int LED)
{
    int status;
    if(((LATB >> LED)&0x0001) == 0) // check if LED is off
    {
        status = 0;
    }
    else
    {
        status = 1;
    }
    return status;
}

//checks digital input at RA2
int check_digital(void)
{
    return PORTAbits.RA2;
}