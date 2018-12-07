
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

    FreeRTOS Task implementation: Co-routine running heart beat

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

void prvHeartbeatCoRoutine(CoRoutineHandle_t xHandle, unsigned portBASE_TYPE uxIndex)
{
    /* Co-routines MUST start with a call to crSTART. */
    crSTART(xHandle);

    for (;;)
    {
        crDELAY(xHandle, Time_MillisecondsToTicks(250));
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

/*
    Process the heartbeat. This is done in the main event loop (as
    opposed to an interrupt) so we can see if the App has locked up.
*/
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
        //IO_RB15_Toggle();        //Toggle signal
    }
    portEXIT_CRITICAL();
}

void taskPeriodic_Execute(void)
{
    portENTER_CRITICAL();
    {
        //IO_RB15_Toggle();        //Toggle signal
        toggle(LED_RED);
    }
    portEXIT_CRITICAL();
}

void taskHeartbeat_Execute(void)
{
    portENTER_CRITICAL();
    {
        //IO_RB15_Toggle();        //Toggle signal
        toggle(LED_POWER);
    }
    portEXIT_CRITICAL();
}

/*******************************************************************************
 End of File
*/