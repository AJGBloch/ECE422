#include <stdlib.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "uart.h"
#include "gpio.h"
#include "global_variables.h"

#include "taskLED.h"

#include "../mcc_generated_files/pin_manager.h"


#define taskLED_PRIORITY        2
#define taskLED_STACK_SIZE      512

/*****************************************************************************

    Private functions prototype

*****************************************************************************/
static portTASK_FUNCTION(vTaskLED, pvParameters);

/*****************************************************************************

    FreeRTOS Task implementation

*****************************************************************************/
static inline void vCreatNewTasks(unsigned portBASE_TYPE uxPriority)
{
    xTaskCreate(vTaskLED, (char const*)"taskName", (uint16_t)taskLED_STACK_SIZE/(uint16_t)(2), NULL, uxPriority, (TaskHandle_t *) NULL );
}

/*****************************************************************************

    Public functions implementation.

*****************************************************************************/
void taskLED_init(void)
{
    vCreatNewTasks(taskLED_PRIORITY);
}

static portTASK_FUNCTION(vTaskLED, pvParameters)
{
    /* Just to stop compiler warnings. */
    (void) pvParameters;

    //===========================================
    //Task entrance
    //===========================================
    while(1)
    {
        if(timed_status == DISABLED)
        {
            vTaskDelay(100);
        }
        else
        {
            vTaskDelay(1000);
        }
        if(timed_status == ENABLED)
        {
            if(timed_count == 0)
            {
                turn_on(LED_YELLOW);
                timed_count++;
            }
            else
            {
                if(timed_count >= timed_duration)
                {
                    turn_off(LED_YELLOW);
                    timed_status = DISABLED;
                }
                timed_count++;
            }
        }
        //IO_RB14_Toggle();
    }
}