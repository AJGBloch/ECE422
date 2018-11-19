
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
 * File: taskUART.c   
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

#include <stdlib.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "taskUART.h"
#include "led.h"
#include "interrupt.h"
#include "uart.h"

#include "../mcc_generated_files/pin_manager.h"


#define taskPRIORITY        1
#define taskSTACK_SIZE      512

/*****************************************************************************

    Private functions prototype

*****************************************************************************/
static portTASK_FUNCTION(vTaskUART, pvParameters);

/*****************************************************************************

    FreeRTOS Task implementation

*****************************************************************************/
static inline void vCreatNewTasks(unsigned portBASE_TYPE uxPriority)
{
    xTaskCreate(vTaskUART, (char const*)"taskName", (uint16_t)taskSTACK_SIZE/(uint16_t)(2), NULL, uxPriority, (TaskHandle_t *) NULL );
}

/*****************************************************************************

    Public functions implementation.

*****************************************************************************/
void taskUART_init(void)
{
    vCreatNewTasks(taskPRIORITY);
}

static portTASK_FUNCTION(vTaskUART, pvParameters)
{
    /* Just to stop compiler warnings. */
    (void) pvParameters;

    //===========================================
    //Task entrance
    //===========================================
    gpio_init();
    external_interrupt_init();
    uart_init();
    turn_on(15);
    turn_on(14);
    //send "ready" via UART
    while(U1STAbits.UTXBF == 1); // wait if send buffer is full
    U1TXREG = 'r';
    while(U1STAbits.UTXBF == 1);
    U1TXREG = 'e';
    while(U1STAbits.UTXBF == 1);
    U1TXREG = 'a';
    while(U1STAbits.UTXBF == 1);
    U1TXREG = 'd';
    while(U1STAbits.UTXBF == 1);
    U1TXREG = 'y';
    while(U1STAbits.UTXBF == 1);
    U1TXREG = '\n';
    while(U1STAbits.UTXBF == 1);
    U1TXREG = '\r';
    turn_off(15);
    while(1)
    {
        if(LED2 == ON)
        {
            turn_on(14);
        }
        else
        {
            turn_off(14);
        }
    }
}