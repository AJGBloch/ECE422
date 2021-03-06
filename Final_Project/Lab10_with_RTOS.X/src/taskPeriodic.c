
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
#include "taskPeriodic.h"
#include "gpio.h"
#include "global_variables.h"


/*****************************************************************************

    FreeRTOS Task implementation: Co-routine running heart beat

 *****************************************************************************/
void prvMainCoRoutine(CoRoutineHandle_t xHandle, unsigned portBASE_TYPE uxIndex)
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
            crDELAY(xHandle, Time_MillisecondsToTicks(1000/periodic_pulse_width));
        }
        if(periodic_status == ENABLED)
        {
            taskPeriodic_Execute();
        }
    }

    /* Co-routines MUST end with a call to crEND. */
    crEND();
}

// *****************************************************************************
// Private functions implementation
// *****************************************************************************

void taskPeriodic_init(unsigned portBASE_TYPE uxNumberToCreate)
{
    xCoRoutineCreate(prvMainCoRoutine, crf_PERIODIC_PRIORITY, crf_PERIODIC_INDEX);
}

/*
    Process the heartbeat. This is done in the main event loop (as
    opposed to an interrupt) so we can see if the App has locked up.
*/
void taskPeriodic_Execute(void)
{
    portENTER_CRITICAL();
    {
        //IO_RB15_Toggle();        //Toggle signal
        toggle(LED_RED);
    }
    portEXIT_CRITICAL();
}

/*******************************************************************************
 End of File
*/