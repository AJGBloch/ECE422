
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
 * File: adc.c   
 * Author: Anthony Bloch
 * Comments: handles ADC for PIC24
 * Revision history: 
 */

#include "adc.h"

void delay(int delayVal)
{
    unsigned int i;

    for (i = 0; i < delayVal; i++)
    {
        ;
    }
}

void adc_init(void)
{
    //initialize adc for manual sampling
    AD1CON1 = 0;
    AD1CSSL = 0;
    AD1CON2 = 0;
    AD1CON3 = 0x2;
    AD1CON1bits.ADON = 1; // turn on ADC
}

int check_analog(int channel)
{
    AD1CHS = channel; // set channel
    AD1CON1bits.SAMP = 1; // start sampling
    delay(100000);
    AD1CON1bits.SAMP = 0; // stop sampling
    while(AD1CON1bits.DONE == 0);
    return ADC1BUF0;
}