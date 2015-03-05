/*
 ============================================================================
 Name        : MC_Sim.c
 Author      : dina elsissy
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "types.h"
#include "sim_task.h"
#include "sim_core.h"
#include "sim_dispatcher.h"
#include <stdio.h>
#include <stdlib.h>


tstrCore aCores[NO_OF_CORES];

static void initialize_Q(void);
//static uint8 GenPriority(uint8 *pu8PriorityList);

extern tstrTask aTasks[NO_OF_TASKS];

FILE *fp, *fp2;

void main(void)
{


	aCores[0].u8Id = 1;
	aCores[0].u8NbOfTasks = 0;
	aCores[0].u8ClusterId = 1;
	aCores[0].state = RUN_MODE;

	aCores[1].u8Id = 2;
	aCores[1].u8NbOfTasks = 0;
	aCores[1].u8ClusterId = 1;
	aCores[1].state = RUN_MODE;
#if 0
	aCores[2].u8Id = 2;
	aCores[2].u8NbOfTasks = 0;
	aCores[2].u8ClusterId = 2;
	aCores[2].state = STANDBY_MODE;

	aCores[3].u8Id = 2;
	aCores[3].u8NbOfTasks = 0;
	aCores[3].u8ClusterId = 2;
	aCores[3].state = STANDBY_MODE;
#endif

	fp = fopen("test.txt", "w");
	fprintf(fp, "hello\n");
	fclose(fp);

	fp2 = fopen("percentage.txt", "w");
	fprintf(fp2, "hello\n");
	fclose(fp2);

	initialize_Q();
	Simulate();

}

void initialize_Q(void)
{
	uint8 i;


	uint8 au8PriorityList[NO_OF_TASKS];

	for(i = 0; i < NO_OF_TASKS; i++)
		au8PriorityList[i] = 0;

#if 0
	for(i = 0; i < NO_OF_TASKS; i++)
	{
		aTasks[i].u8Priority = (rand() % LOWEST_PRIORITY)+1;//GenPriority(au8PriorityList);
		aTasks[i].u32InstCnt = (rand() % 1000000)+1000000;
		aTasks[i].state = READY;
		aTasks[i].u32ExecInstCnt = 0;
		aTasks[i].u32SleepTime= 0;
		aTasks[i].u32Period = (aTasks[i].u32InstCnt*5)/50;

	}
#else
		aTasks[0].u8Priority = 1;
		aTasks[0].u32InstCnt = 1000000;
		aTasks[0].state = READY;
		aTasks[0].u32ExecInstCnt = 0;
		aTasks[0].u32SleepTime = (aTasks[0].u32InstCnt)/10;
		aTasks[0].u32Period = (aTasks[0].u32InstCnt*4)/10;

		aTasks[1].u8Priority = 1;
		aTasks[1].u32InstCnt = 1000000;
		aTasks[1].state = READY;
		aTasks[1].u32ExecInstCnt = 0;
		aTasks[1].u32SleepTime = (aTasks[1].u32InstCnt)/10;;
		aTasks[1].u32Period = (aTasks[1].u32InstCnt*4)/10;

		aTasks[2].u8Priority = 2;
		aTasks[2].u32InstCnt = 1000000;
		aTasks[2].state = SLEEPING;
		aTasks[2].u32ExecInstCnt = 0;
		aTasks[2].u32SleepTime = (aTasks[2].u32InstCnt*4)/10;
		aTasks[2].u32IdleTime = aTasks[2].u32SleepTime;
		aTasks[2].u32Period = (aTasks[2].u32InstCnt*4)/10;

		aTasks[3].u8Priority = 2;
		aTasks[3].u32InstCnt = 1000000;
		aTasks[3].state = SLEEPING;
		aTasks[3].u32ExecInstCnt = 0;
		aTasks[3].u32SleepTime = (aTasks[3].u32InstCnt*4)/10;
		aTasks[3].u32IdleTime = aTasks[3].u32SleepTime;
		aTasks[3].u32Period = (aTasks[3].u32InstCnt*4)/10;

		aTasks[4].u8Priority = 3;
		aTasks[4].u32InstCnt = 1000000;
		aTasks[4].state = SLEEPING;
		aTasks[4].u32ExecInstCnt = 0;
		aTasks[4].u32SleepTime = (aTasks[4].u32InstCnt*6)/10;
		aTasks[4].u32IdleTime = aTasks[4].u32SleepTime;
		aTasks[4].u32Period = (aTasks[4].u32InstCnt*4)/10;

		aTasks[5].u8Priority = 3;
		aTasks[5].u32InstCnt = 1000000;
		aTasks[5].state = SLEEPING;
		aTasks[5].u32ExecInstCnt = 0;
		aTasks[5].u32SleepTime = (aTasks[5].u32InstCnt*6)/10;
		aTasks[5].u32IdleTime = aTasks[5].u32SleepTime;
		aTasks[5].u32Period = (aTasks[5].u32InstCnt*4)/10;

		aTasks[6].u8Priority = 4;
		aTasks[6].u32InstCnt = 1000000;
		aTasks[6].state = SLEEPING;
		aTasks[6].u32ExecInstCnt = 0;
		aTasks[6].u32SleepTime = (aTasks[6].u32InstCnt*8)/10;
		aTasks[6].u32IdleTime = aTasks[6].u32SleepTime;
		aTasks[6].u32Period = (aTasks[6].u32InstCnt*4)/10;

		aTasks[7].u8Priority = 4;
		aTasks[7].u32InstCnt = 1000000;
		aTasks[7].state = SLEEPING;
		aTasks[7].u32ExecInstCnt = 0;
		aTasks[7].u32SleepTime = (aTasks[7].u32InstCnt*8)/10;
		aTasks[7].u32IdleTime = aTasks[7].u32SleepTime;
		aTasks[7].u32Period = (aTasks[7].u32InstCnt*4)/10;
#endif

}

#if 0
uint8 GenPriority(uint8 *pu8PriorityList)
{
	uint8 u8r;

	do{
		u8r =  (rand()%LOWEST_PRIORITY)+1;
	} while(pu8PriorityList[u8r] == 1);

	pu8PriorityList[u8r] = 1;
	return u8r;

}
#endif
