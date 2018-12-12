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
 * File: taskLED.c  
 * Author: Anthony Bloch
 * Comments: runs tasks for heartbeat, timed, and periodic LEDs
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
#include "taskLED.h"
#include "gpio.h"
#include "global_variables.h"

/*****************************************************************************

    FreeRTOS Task implementation: Co-routine for periodic pulse output when enabled

 *****************************************************************************/
void prvPeriodicCoRoutine(CoRoutineHandle_t xHandle, unsigned portBASE_TYPE uxIndex)
{
    /* Co-routines MUST start with a call to crSTART. */
    crSTART(xHandle);

    for (;;)
    {
        if(periodic_status == DISABLED)
        {
            crDELAY(xHandle, Time_MillisecondsToTicks(100));
        }
        else
        {
            crDELAY(xHandle, Time_MillisecondsToTicks(2500/periodic_pulse_width));
        }
        if(periodic_status == ENABLED)
        {
            taskPeriodic_Execute();
        }
    }

    /* Co-routines MUST end with a call to crEND. */
    crEND();
}

/*****************************************************************************

    FreeRTOS Task implementation: Co-routine for timed output when enabled

 *****************************************************************************/
void prvTimedCoRoutine(CoRoutineHandle_t xHandle, unsigned portBASE_TYPE uxIndex)
{
    /* Co-routines MUST start with a call to crSTART. */
    crSTART(xHandle);

    for (;;)
    {
        crDELAY(xHandle, Time_MillisecondsToTicks(50));
        if(timed_status == ENABLED)
        {
            taskTimed_Execute();
        }
    }

    /* Co-routines MUST end with a call to crEND. */
    crEND();
}

/*****************************************************************************

    FreeRTOS Task implementation: Co-routine for heartbeat

 *****************************************************************************/
void prvHeartbeatCoRoutine(CoRoutineHandle_t xHandle, unsigned portBASE_TYPE uxIndex)
{
    /* Co-routines MUST start with a call to crSTART. */
    crSTART(xHandle);

    for (;;)
    {
        crDELAY(xHandle, Time_MillisecondsToTicks(249));
        taskHeartbeat_Execute();
    }

    /* Co-routines MUST end with a call to crEND. */
    crEND();
}

// *****************************************************************************
// Private functions implementation
// *****************************************************************************

void taskLED_init(unsigned portBASE_TYPE uxNumberToCreate)
{
    xCoRoutineCreate(prvPeriodicCoRoutine, crf_PERIODIC_PRIORITY, crf_PERIODIC_INDEX);
    xCoRoutineCreate(prvTimedCoRoutine, crf_TIMED_PRIORITY, crf_TIMED_INDEX);
    xCoRoutineCreate(prvHeartbeatCoRoutine, crf_HEARTBEAT_PRIORITY, crf_HEARTBEAT_INDEX);
}

//turns on LED to begin, and turns it off when the appropriate time has passed
void taskTimed_Execute(void)
{
    portENTER_CRITICAL();
    {
        if(timed_count == 0)
        {
            turn_on(LED_YELLOW);
        }
        else if(timed_count >= timed_duration)
        {
            turn_off(LED_YELLOW);
            timed_status = DISABLED;
        }
        timed_count++;
    }
    portEXIT_CRITICAL();
}


// toggles periodic pulse LED
void taskPeriodic_Execute(void)
{
    portENTER_CRITICAL();
    {
        toggle(LED_RED);
    }
    portEXIT_CRITICAL();
}

// toggles the heartbeat LED
void taskHeartbeat_Execute(void)
{
    portENTER_CRITICAL();
    {
        toggle(LED_POWER);
    }
    portEXIT_CRITICAL();
}

/*******************************************************************************
 End of File
*/