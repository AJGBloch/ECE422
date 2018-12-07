
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
 * Comments: handles GPIO for LEDs on RP13, 14, 15
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
   //LATB = 0x0000;                    // initialize output to be off
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

int check_digital(void)
{
    return PORTAbits.RA2;
}

/*int check_analog(int channel)
{
    int i;
    AD1CON1 = 0x000; // SAMP bit = 0 ends sampling and starts converting
    AD1CHS = (0x0001 << channel); // connect AN(channel) as S/H+ input
   
    AD1CSSL = 0;
    AD1CON3 = 0x0002; // manual sample, Tad = 3Tcy
    AD1CON2 = 0;
    AD1CON1bits.ADON = 1; // turn ADC on
    AD1CON1bits.SAMP = 1;
    for(i = 0; i < 100; i++) // delay
    {
       ;
    }
    AD1CON1bits.SAMP = 0;
    while(!AD1CON1bits.DONE);
    return ADC1BUF0;
    /*AD1CON1bits = 0x0000;
    AD1CON1bits.ADON = 1; // turn on ADC
    AD1CON1bits.ADSIDL = 1; // discontinues module operation in idle mode
    AD1CON1bits.DMABM = 0; // PIA mode: buffer addresses are defined by the DMA controller and AD1CON4<2:0>
    AD1CON1bits.DMAEN = 0; // extended DMA and buffer features are disabled
    AD1CON1bits.MODE12 = 1; // 12 bit ADC
    AD1CON1bits.FORM = 00; // absolute decimal result, unsigned, right justified
    AD1CON1bits.SSRC = 0000; // SAMP is cleared by software
    AD1CON1bits.ASAM = 0; // sampling begins when SAMP bit is manually set
    
    //AD1CON1bits.SAMP = 0; // hold (set to 1 for sample)
    //AD1CON1bits.DONE == 1 for ADC cycle complete
    
    AD1CON2 = 0x0000;
    AD1CON2bits.PVCFG = 00; // positive voltage reference is AVdd
    AD1CON2bits.NVCFG0 = 0; // negative voltage reference is AVss
    AD1CON2bits.BUFREGEN = 1; // conversion result is loaded into the buffer location determined by the converted channel
    AD1CON2bits.CSCNA = 0; // does not scan input selection for CH0+ during sample A bit
    //AD1CON2bits.BUFS based on DMAEN and DMABM == 0 -> ADC is filling ADC1BUF13-25, == 1 ADC is currently filling ADCBUF0-12
    AD1CON2bits.SMPI = 1111; // interrupts at the completion of the conversion for each 32nd sample
    AD1CON2bits.BUFM = 0; // always starts filling buffer at ADC1BUF0
    AD1CON2bits.ALTS = 0; // always uses channel input selects for sample A
    
    AD1CON3 = 0x0000;
    AD1CON3bits.ADRC = 0; // clock derived from system clock
    //AD1CON3bits.EXTSAM == 1 -> ADC is still sampling after SAMP = 0, == 0 -> ADC is finished sampling
    AD1CON3bits.PUMPEN = 0; // charge pump for switches is disabled
    AD1CON3bits.SAMC = 4; // auto sample time select bits
    AD1CON3bits.ADCS = 0x01; // ADC clock select bits = 2*Tcy = Tad
    
    
    AD1CON4 = 0x0000;
    AD1CON4bits. DMABL = 1; // allocates 2 words of buffer to each analog input
    
    AD1CON5 = 0x0000;
    AD1CON5bits.ASEN = 0; // auto-scan is disabled
    AD1CON5bits.LPEN = 0; // full power is enabled after scan
    AD1CON5bits.CTMREQ = 0; // CTMU is not enabled by the ADC
    AD1CON5bits.BGREQ = 0; // band gap is not enabled by the ADC
    AD1CON5bits.ASINT = 00; // no interrupt
    AD1CON5bits.WM = 01; // convert and saved
    AD1CON5bits.CM = 00; // less than mode*/
    
    //300
    
    /*// configure port pins as analog inputs by setting ANSx
    // select voltage reference source (AD1CON2<15:13>
    AD1CON2bits.PVCFG = 00; // positive voltage reference is AVdd
    AD1CON2bits.NVCFG0 = 0; // negative voltage reference is AVss
    //select positive and negative multiplexer inputs for each channel AD1CHS <15:0>
    AD1CHSbits.CH0NB = 000; // sample B channel 0 negative input AVss
    AD1CHSbits.CH0SB = 5; // AN5
    // select analog conversion clock to match desired data rate with the processor clock AD1CON3<7:0>
    // select appropriate sample/conversion sequence AD1CON1<7:4> and AD1CON3 <12:8>
    // for channel A scanning operations select positive channels to be included AD1CSSH and AD1CSSL registers
    // select how conversion reults are presented in the buffer AD1CON1<9:8> and AD1CON5 register
    // select interrupt rate AD1CON2<5:2>
    //turn on AD module AD1CON1<15>
    
    // clear the AD1IFbit IFS0<13>
    // enable the AD1IE interrupt IEC0<13>
    //select AD interrupt priority IPC3 <6:4>
    
    // if the module is configure for manual sampling set the SAMP bit AD1CON1<1> to begin sampling
    */        
/*}*/