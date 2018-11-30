
#ifndef TASK_HEARTBEAT_H
#define	TASK_HEARTBEAT_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* Only one co-routine is created so the index is not significant. */
#define crfFLASH_INDEX             (0)
#define crf_HEART_BEAT_PRIORITY    (0)
    
extern void taskHeartbeat_Init(unsigned portBASE_TYPE uxPriority);
extern void taskHeartbeat_Execute(void);
extern void prvMainCoRoutine(CoRoutineHandle_t xHandle, unsigned portBASE_TYPE uxIndex);


#ifdef	__cplusplus
}
#endif

#endif	/* TASK_HEARTBEAT_H */

