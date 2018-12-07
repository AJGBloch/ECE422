
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
 * Comments: handles ADC
 * Revision history: 
 */

#include "adc.h"

void delay(int delayVal)
{
    unsigned int i;

    for (i = 0; i < delayVal; i++)
    {
        ;//for (j = 0; j < 100; j++);
    }
}

void adc_init(void)
{
    /*
    // configure port pins as analog inputs by setting ANSx
    // select voltage reference source (AD1CON2<15:13>
    AD1CON2bits.PVCFG = 00; // positive voltage reference is AVdd
    AD1CON2bits.NVCFG0 = 0; // negative voltage reference is AVss
    //select positive and negative multiplexer inputs for each channel AD1CHS <15:0>
    AD1CHSbits.CH0NB = 000; // sample B channel 0 negative input AVss
    AD1CHSbits.CH0SB = 30; // AVdd
    AD1CHSbits.CH0NA = 0;
    AD1CHSbits.CH0SA = 30;
    // select analog conversion clock to match desired data rate with the processor clock AD1CON3<7:0>
    AD1CON3bits.ADCS = 1; // Tad = 2*Tcy
    // select appropriate sample/conversion sequence AD1CON1<7:4> and AD1CON3 <12:8>
    AD1CON3bits.SAMC = 0b11111;
    AD1CON1bits.SSRC = 0; //SAMP is cleared by software
    // for channel A scanning operations select positive channels to be included AD1CSSH and AD1CSSL registers
    
    // select how conversion results are presented in the buffer AD1CON1<9:8> and AD1CON5 register
    AD1CSSH = 0; // skip channels for input scan
    AD1CSSL = 0; // skips channels for input scan
    // select interrupt rate AD1CON2<5:2>
    AD1CON2bits.SMPI = 0; // interrupts at the completion of the conversion for each sample
    //turn on AD module AD1CON1<15>
    AD1CON1bits.ADON = 1;
    // clear the AD1IFbit IFS0<13>
    IFS0 &= 0b01111111111111;
    // enable the AD1IE interrupt IEC0<13>
    IEC0 |= 0b10000000000000;
    //select AD interrupt priority IPC3 <6:4>
    IPC3 |= 0b1000000;
    
    // if the module is configure for manual sampling set the SAMP bit AD1CON1<1> to begin sampling
     */
   
    
    AD1CON1 = 0;
    AD1CSSL = 0;
    AD1CON2 = 0;
    AD1CON3 = 0x2;
    AD1CON1bits.ADON = 1;
    
    /*AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3bits.ADCS = 0xFF;
    AD1CON3bits.SAMC = 0b10000;
    AD1CON1bits.ADON = 1;*/
    
}

int check_analog(int channel)
{
    //AD1CHS = channel;
    AD1CHS = channel;
    AD1CON1bits.SAMP = 1;
    //TMR1 = 0;
    //delay(492*10);
    delay(100000);
    AD1CON1bits.SAMP = 0;
    //AD1CON1bits.DONE = 1;
    while(AD1CON1bits.DONE == 0);
    return ADC1BUF0;
 
    /*adc_init(); b code
    
    TRISBbits.TRISB3 = 1;
    ANSBbits.ANSB3 = 1;*/
    
    /*AD1CHS = channel;
    AD1CON1bits.SAMP = 1;
    delay(1000);
    AD1CON1bits.SAMP = 0;
    while(!AD1CON1bits.DONE){}
    return ADC1BUF0;*/
}