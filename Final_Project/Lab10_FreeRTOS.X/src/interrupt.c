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
 * File: interrupt.h   
 * Author: Anthony Bloch
 * Comments: external interrupt initialization and handler on RP12
 * Revision history: 
 */

#include "interrupt.h"
#include "led.h"

int ctr = 0;

void external_interrupt_init(void)
{
    // Configure Output Functions (Table 11-7)
    // Assign INT2 To Pin RP12
    RPINR1bits.INT2R = 12;
    
    ANSBbits.ANSB12 = 0;    // turn off analog
    INTCON2 |= 0x04;        // set interrupt to be negative edge triggered
    IPC7bits.INT2IP = 0x01; // set priority to 1
    IFS1bits.INT2IF = 0;    // clear interrupt flag
    IEC1bits.INT2IE = 1;    //enable external interrupt
}


void __attribute__((__interrupt__, __auto_psv__)) _INT2Interrupt(void)
{
    if(ctr >= 1000000) // reset when counter equals 1 million
    {
        ctr = 0;
    }
    ctr++;
    if(LED2 == ON) // toggle LED2
    {
        LED2 = OFF;
    }
    else //LED2 == OFF
    {
        LED2 = ON;
    }
    IFS1bits.INT2IF = 0; // clear interrupt flag
}
