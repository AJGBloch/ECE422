
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

