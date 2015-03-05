#ifndef TASK_H
#define TASK_H


#include "types.h"
#include <stdio.h>



/* Everything in microseconds */
#define STEP	 			50						/* 50Usec */
#define TICK 				10000  					/* TICK (10 msec) */
#define SAMPLING    		(1000000) 				/* (1 sec) */
#define SWITCH_DELAY 		(500)					/* 500Usec */
#define MAX_MIPS 			50000000 				/* 50MHz; just as an example */
#define	CLK_STEPS			5
#define	NO_OF_TASKS			8
#define NO_OF_CORES			2
#define NO_OF_CLUSTERS		2
#define HIGHEST_PRIORITY	1
#define LOWEST_PRIORITY		5

typedef enum {
	READY=0,
	RUNNING,
	SLEEPING,
} STATE;

typedef struct {
	uint8 	u8Priority;
	uint8	u8ClusterId;
	uint8	u8CoreId;
	STATE 	state;
	uint32 	u32InstCnt;			/* Instructions count */
	uint32 	u32Period;
	uint32 	u32ExecInstCnt;		/* Executed Instructions count */
	uint32 	u32SleepTime;		/* time to wake up */
	uint32 	u32IdleTime;
	uint32	u32ActiveTime;		/* time since the task has been made ready */
} tstrTask;

/* functions prototypes */
tstrTask* find_Rdy_Task(void);
void update_EIC(tstrTask *task);
void sleep_task(tstrTask *task);
void update_ST(void);
void UpdateActiveTime(void);
/*power management functions*/
uint32 FindFreq(uint32 u32Freq);
void FreqControl_A1(uint32 u32IdleTime);
uint32 CalcEnergySaving(void);

#endif
