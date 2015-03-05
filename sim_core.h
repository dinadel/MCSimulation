#ifndef SIM_CORE_H
#define SIM_CORE_H


#include "types.h"

#define MAX_NB_TASKS_PER_CORE		4 /*total no of tasks divided by number of cores*/

typedef enum {
	RUN_MODE = 0,
	STANDBY_MODE,
	DORMANT_MODE,
	SHUT_DOWN_MODE,
} CORE_STATE;

typedef struct {
	uint8 		u8Id;
	uint8		u8NbOfTasks;
	uint8		u8ClusterId;	
	CORE_STATE 	state;		
} tstrCore;

#endif

