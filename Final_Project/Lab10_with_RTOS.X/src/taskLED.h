
#ifndef TASK_LED_H
#define	TASK_LED_H

#ifdef	__cplusplus
extern "C" {
#endif

#define crf_TIMED_INDEX             (0)
#define crf_TIMED_PRIORITY    (1)
#define crf_PERIODIC_INDEX             (1)
#define crf_PERIODIC_PRIORITY    (2)
#define crf_HEARTBEAT_INDEX             (2)
#define crf_HEARTBEAT_PRIORITY    (3)
    
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

