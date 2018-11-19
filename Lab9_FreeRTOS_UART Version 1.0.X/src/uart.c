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
 * File: uart.c   
 * Author: Anthony Bloch
 * Comments: provides uart functionality to PIC24FJ256GA2017
 * Revision history: 
 */


#include "uart.h"
#include "led.h"
#include "interrupt.h"

char ch;

void uart_init(void)
{
    RPINR18bits.U1RXR = 2; //U1RX assigned to RP2
    RPOR3bits.RP7R = 3; // U1TX assigned to RP7
    U1BRG = 103; // 4 MHz and 9600 baud rate with BRGH = 1
    ANSBbits.ANSB2 = 0; // turn off analog for Rx
    U1MODE = 0;
    U1MODEbits.BRGH = 1; // high speed -> 4 clocks per bit period
    IEC0bits.U1RXIE = 1; // Rx interrupt enabled

    IPC2bits.U1RXIP = 1; // priority
    IFS0bits.U1RXIF = 0; // clear interrupt flag
    
    U1MODEbits.UARTEN = 1; // enabling UART ON bit
    U1STAbits.UTXEN = 1; // Transmit enabled
}

void __attribute__((interrupt, no_atuo_psv)) _U1RXInterrupt(void)
{
    turn_on(15);
    /*if(LED2 == ON) // Turn on LED1 while processing UART interrupt
    {
        LATB = 0xC000;
    }
    else
    {
        LATB = 0x8000;
    }*/
    unsigned int ctr_digits[7]; // allows a value of up to 1 million to be transmitted
    unsigned int digits = 0;
    unsigned int temp_counter;
    unsigned int temp;
    unsigned int j;
    ch = U1RXREG; // read data received by UART
    if(ch == 'a') // activate/deactivate LED2
    {
        if(ctr >= 1000000) // reset when counter equals 1 million
        {
            ctr = 0;
        }
        ctr++;
        if(LED2 == ON)
        {
            LED2 = OFF;
        }
        else //LED2 == OFF
        {
            LED2 = ON;
        }
    }
    else if(ch == 's') // report status of LED2
    {
        if(LED2 == ON) // transmit "on"
        {
            while(U1STAbits.UTXBF == 1);
            U1TXREG = 'o'; 
            while(U1STAbits.UTXBF == 1);
            U1TXREG = 'n'; 
            while(U1STAbits.UTXBF == 1);
            U1TXREG = '\n';
            while(U1STAbits.UTXBF == 1);
            U1TXREG = '\r';
        }
        else // LED2 == OFF -> transmit "off"
        {
            while(U1STAbits.UTXBF == 1);
            U1TXREG = 'o'; 
            while(U1STAbits.UTXBF == 1);
            U1TXREG = 'f'; 
            while(U1STAbits.UTXBF == 1);
            U1TXREG = 'f'; 
            while(U1STAbits.UTXBF == 1);
            U1TXREG = '\n';
            while(U1STAbits.UTXBF == 1);
            U1TXREG = '\r';
        }
    }
    else if(ch == 'c') // report # of times (count) button has been pressed
    {
        temp = 10; // print counter in base 10
        temp_counter = ctr;
        digits = 0;
        while(temp_counter >= temp) // process bit by bit temp_counter
        {
          ctr_digits[digits]  = (temp_counter%temp)+48; 
          digits++;
          temp_counter = (temp_counter-(temp_counter%temp))/temp;
        }
        ctr_digits[digits] = temp_counter+48;
        digits++;
        for(j = digits; j > 0; j--) // transmit bits left to right
        {
            while(U1STAbits.UTXBF == 1);
            U1TXREG = ctr_digits[j-1];
        }
        while(U1STAbits.UTXBF == 1);
        U1TXREG = '\n';
        while(U1STAbits.UTXBF == 1);
        U1TXREG = '\r';
    }
    else if(ch == 'r') // reset button counter
    {
        ctr = 0;
        // transmit "clear"
        while(U1STAbits.UTXBF == 1);
        U1TXREG = 'c'; 
        while(U1STAbits.UTXBF == 1);
        U1TXREG = 'l'; 
        while(U1STAbits.UTXBF == 1);
        U1TXREG = 'e'; 
        while(U1STAbits.UTXBF == 1);
        U1TXREG = 'a'; 
        while(U1STAbits.UTXBF == 1);
        U1TXREG = 'r';
        while(U1STAbits.UTXBF == 1);
        U1TXREG = '\n';
        while(U1STAbits.UTXBF == 1);
        U1TXREG = '\r';
    }
    else // invalid command
    {
        ;
    }
    turn_off(15);
    IFS0bits.U1RXIF = 0; // clear interrupt flag

}