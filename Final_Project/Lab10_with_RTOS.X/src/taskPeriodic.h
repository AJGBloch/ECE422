
#ifndef TASK_PERIODIC_H
#define	TASK_PERIODIC_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* Only one co-routine is created so the index is not significant. */
#define crf_PERIODIC_INDEX             (0)
#define crf_PERIODIC_PRIORITY    (2)
    
extern void taskPeriodic_init(unsigned portBASE_TYPE uxPriority);
extern void taskPeriodic_Execute(void);
extern void prvMainCoRoutine(CoRoutineHandle_t xHandle, unsigned portBASE_TYPE uxIndex);


#ifdef	__cplusplus
}
#endif

#endif	/* TASK_Periodic_H */

