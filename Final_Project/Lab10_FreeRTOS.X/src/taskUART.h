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
 * File: taskUART.h   
 * Author: Anthony Bloch
 * Comments: creates an independent task to do the following:
 * With the push of a button on the board, the LED will toggle on/off.
 * When 'a' is entered on the computer's keyboard on the serial terminal, LED2 will activate/deactivate.
 * When 's' is entered, the status (on/off) is reported on the terminal.
 * When 'c' is entered, the number of times (count) the button or 'a' have been pressed is reported on the terminal.
 * When 'r' is entered, the counter tracking the number of times the button or 't' has been pressed is reset to 0.
 * There is also a reset push button on the board that resets the system when pushed.
 * When there is UART activity, LED1 will turn on. Otherwise it is off.
 * 
 * Revision history: 
 */
#ifndef TASK_UART_H
#define	TASK_UART_H

#ifdef	__cplusplus
extern "C" {
#endif

extern void taskUART_init(void);


#ifdef	__cplusplus
}
#endif

#endif	/* TASK_UART_H */

