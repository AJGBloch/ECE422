#include <stdlib.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "uart.h"
#include "gpio.h"

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
        vTaskDelay(250);
        toggle(LED_YELLOW);
        //IO_RB14_Toggle();
    }
}