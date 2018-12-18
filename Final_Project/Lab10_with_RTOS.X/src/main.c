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
 * File: main.c   
 * Author: Anthony Bloch
 * Comments: This is the main for Lab10 for ECE422
 *           This program runs on the PIC24FJ256GA2017 along with a GUI connected via uart.
 *           Protocols are sent from the GUI which are processed and responded to accordingly by the PIC24.
 *           Functionality includes:
 *                  constantly blink LED1 with period of 1 second
 *               prompted by protocol...
 *                  turn on/off LED2
 *                  turn on LED3 for a period of time after which it turns off
 *                      this LED can be disabled while it is on, and its status can also be read
 *                  enable LED4 such that is blinks on/off at a given frequency
 *                      this LED can be disabled such that it remains off
 *                  read a digital input button
 *                  read 3 analog channel inputs
 *                  reset capability through push button
 *          information is sent back to the GUI via uart as needed
 * 
 * Revision history:
 * version 1.0
			§ GUI set up to send proper protocols to PIC24
			§ PIC24 set up to receive protocol
				? Can process protocols for turning on/off a single LED (green)
			§ PIC project
				? Files Added
					® Main.c
					® App.h/.c
					® Gpio.h/.c
					® Uart.h.c
			§ GUI
				? Files added
                    Template from D2L
 *version 1.1
			§ PIC project
				? Files Added
					® Timer.h/.c
				? Added LED pulsed and LED timed output functionality
				? Added status reporting of LED timed output 
            Protocols now begin with '<' and end with '>'
 * version 1.2
			§ PIC project
				? Added functionality to read digital input (button) - verified as working
				? Added functionality to read analog, but it needs more work
            Full communication between GUI and PIC is functional
 * Version 1.3 to git
			§ PIC project
				? Added files for FreeRTOS tasks
					® taskUART.h/.c
					® taskLEDs.h/.c
            UART doesn't always receive command (perhaps because of delay). May need receive buffer instead of character for uart
 * version 2.0
			§ PIC24
				? Added Adc.h/.c
                Added 3rd coroutine
 * version 2.1 
 *          commented code
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
//#include "taskTimed.h"
#include "taskLED.h"
#include "gpio.h"
#include "uart.h"
#include "adc.h"

/*****************************************************************************

    Private Macros

 *****************************************************************************/
/* The number of flash co-routines to create. */
#define mainNUM_FLASH_COROUTINES   (3)


// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
int main( void )
{
    // initialize the device
    SYSTEM_Initialize();
    gpio_init();
    uart_init();
    adc_init();
    
    /*while(1)
    {
        temp2 = 0;
        temp = check_analog(temp2);
        temp2 = 0;
        temp2 = 1;
        temp = check_analog(temp2);
        temp2 = 0;
        temp2 = 5;
        temp = check_analog(temp2);
        temp2 = 0;
    }*/
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
    taskLED_init(mainNUM_FLASH_COROUTINES);
    //taskPeriodic_init(mainNUM_FLASH_COROUTINES);

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
        // Add your application code
    }
    
    return -1;
}

