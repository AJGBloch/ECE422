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
#include "gpio.h"

char ch_rx;
char ch_tx;
int ch_new = 0;

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

void uart_send(void)
{
    while(U1STAbits.UTXBF == 1); // wait if send buffer is full
    U1TXREG = ch_tx;
}

void __attribute__((interrupt, no_atuo_psv)) _U1RXInterrupt(void)
{
    ch_rx = U1RXREG; // read data received by UART
    ch_new = 1; // raise flag that new character was received
    IFS0bits.U1RXIF = 0; // clear interrupt flag

}