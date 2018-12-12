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
 * File: taskLED.h   
 * Author: Anthony Bloch
 * Comments: runs tasks for heartbeat, timed, and periodic LEDs
 * Revision history: 
 */

#ifndef TASK_LED_H
#define	TASK_LED_H

#ifdef	__cplusplus
extern "C" {
#endif

#define crf_TIMED_INDEX             (0)
#define crf_TIMED_PRIORITY          (1)
#define crf_PERIODIC_INDEX          (1)
#define crf_PERIODIC_PRIORITY       (2)
#define crf_HEARTBEAT_INDEX         (2)
#define crf_HEARTBEAT_PRIORITY      (3)
    
extern void taskLED_init(unsigned portBASE_TYPE uxPriority);
extern void taskTimed_Execute(void);
extern void taskPeriodic_Execute(void);
extern void taskHeartbeat_Execute(void);
extern void prvTimedCoRoutine(CoRoutineHandle_t xHandle, unsigned portBASE_TYPE uxIndex);
extern void prvPeriodicCoRoutine(CoRoutineHandle_t xHandle, unsigned portBASE_TYPE uxIndex);
extern void prvHeartbeatCoRoutine(CoRoutineHandle_t xHandle, unsigned portBASE_TYPE uxIndex);



#ifdef	__cplusplus
}
#endif

#endif	/* TASK_LED_H */

