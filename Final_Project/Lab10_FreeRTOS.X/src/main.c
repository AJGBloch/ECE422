
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
 * File: main.c (for lab9_FreeRTOS_UART)
 * Date: 11/19/18  
 * Author: Anthony Bloch
 * Comments: creates an independent task to do the following:
 * With the push of a button on the board, the LED will toggle on/off.
 * When 'a' is entered on the computer's keyboard on the serial terminal, LED2 will activate/deactivate.
 * When 's' is entered, the status (on/off) is reported on the terminal.
 * When 'c' is entered, the number of times (count) the button or 'a' have been pressed is reported on the terminal.
 * When 'r' is entered, the counter tracking the number of times the button or 't' has been pressed is reset to 0.
 * There is also a reset push button on the board that resets the system when pushed.
 * When there is UART activity, LED1 will turn on. Otherwise it is off.
 * Revision history: 
 */
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE

#include "FreeRTOS.h"
#include "croutine.h"
#include "task.h"

#include "../mcc_generated_files/system.h"
#include "../mcc_generated_files/pin_manager.h"

//RTOS tasks include
#include "app.h"
#include "taskUART.h"
#include "../taskTimedOutput.h"
#include "../taskPeriodicPulse.h"
#include "timer.h"

/*****************************************************************************

    Private Macros

 *****************************************************************************/
/* The number of flash co-routines to create. */
#define mainNUM_FLASH_COROUTINES   (1)


// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
int main( void )
{
    // initialize the device
    SYSTEM_Initialize();
    
    //=========================================================================
    //    Board initialization
    //=========================================================================
    

    //=========================================================================
    //    Set application's initial state
    //=========================================================================
    
    //=========================================================================
    //    Application Task initialization
    //=========================================================================
    taskUART_init();
    //taskTimedOutput_init();
    //taskPeriodicPulse_init();

    //=========================================================================
    //    FreeRTOS scheduler
    //=========================================================================
    vTaskStartScheduler();

    /* If all is well then this line will never be reached.  If it is reached
    then it is likely that there was insufficient (FreeRTOS) heap memory space
    to create the idle task.  This may have been trapped by the malloc() failed
    hook function, if one is configured.
    */
    
    while (1)
    {
    }
    
    return -1;
}

