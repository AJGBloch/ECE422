
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
 * File: taskTimedOutput.c   
 * Author: Anthony Bloch
 * Comments: 
 * 
 * Revision history: 
 */

#include <stdlib.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "taskTimedOutput.h"
#include "gpio.h"
#include "macros.h"
//#include "timer.h"



#include "../mcc_generated_files/pin_manager.h"


#define taskPRIORITY        1
#define taskSTACK_SIZE      512

int timed_status = ENABLED;
int timed_duration = 1;
int timed_count = 0;
int led_yellow_status = OFF;

/*****************************************************************************

    Private functions prototype

*****************************************************************************/
static portTASK_FUNCTION(vTaskTimedOutput, pvParameters);

/*****************************************************************************

    FreeRTOS Task implementation

*****************************************************************************/
static inline void vCreatNewTasks(unsigned portBASE_TYPE uxPriority)
{
    xTaskCreate(vTaskTimedOutput, (char const*)"taskName", (uint16_t)taskSTACK_SIZE/(uint16_t)(2), NULL, uxPriority, (TaskHandle_t *) NULL );
}

/*****************************************************************************

    Public functions implementation.

*****************************************************************************/
void taskTimedOutput_init(void)
{
    vCreatNewTasks(taskPRIORITY);
}


static portTASK_FUNCTION(vTaskTimedOutput, pvParameters)
{
    /* Just to stop compiler warnings. */
    (void) pvParameters;

    //===========================================
    //Task entrance
    //===========================================
    
    while(1)
    {
        if(timed_status == ENABLED)
        {
            vTaskDelay(500);
            toggle(LED_YELLOW);
            timed_status = DISABLED;
        }
    }    
}
