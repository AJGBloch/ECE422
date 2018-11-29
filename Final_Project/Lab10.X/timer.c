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
 * File: timer.c  
 * Author: Anthony Bloch
 * Comments: handles timers for PIC24
 * Revision history: 
 */


#include "timer.h"

int t1_flag = UP;
int t1_periods_1 = 0;
int t1_periods_2 = 0;

void timer_init(void)
{
T1CON = 0x00; //Stops the Timer1 and reset control reg.
TMR1 = 0x00; //Clear contents of the timer register
T1CONbits.TCKPS = 2; // pre-scaler 1:64
PR1 = 1; //Load the Period register with the value 1
IPC0bits.T1IP = 0x01; //Setup Timer1 interrupt for desired priority level
IFS0bits.T1IF = 0; //Clear the Timer1 interrupt status flag
IEC0bits.T1IE = 1; //Enable Timer1 interrupts
T1CONbits.TON = 1; //Start Timer1
}

// timer interrupt handler
void __attribute__((__interrupt__, __shadow__)) _T1Interrupt(void)
{
    // led_counter controls which led is on in main()
    t1_flag = 1;
    if(t1_periods_1 >= 2000000000)
    {
        t1_periods_1 = 0;
    }
    if(t1_periods_2 >= 2000000000)
    {
        t1_periods_2 = 0;
    }
    t1_periods_1++;
    t1_periods_2++;
    IFS0bits.T1IF = 0; //Reset Timer1 interrupt flag and Return from ISR
}