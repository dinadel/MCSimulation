
#include "sim_task.h"
#include "sim_core.h"
#include <stdio.h>
	


tstrTask aTasks[NO_OF_TASKS];

uint32 gu32SimTime=0;
uint32 gu32IdleTime=0;
uint32 gu32MIPS = MAX_MIPS,gu32OldMIPS = MAX_MIPS,gu32EnergySaving;

FILE *fp;


uint32 FindFreq(uint32 u32Freq)
{
	uint32 u32Quotient,u32Remainder;

	u32Quotient = u32Freq/(MAX_MIPS/CLK_STEPS);
	u32Remainder = u32Freq%(MAX_MIPS/CLK_STEPS);

	if(u32Remainder > 0) 
		u32Quotient++;

	return u32Quotient*(MAX_MIPS/CLK_STEPS);

}

void FreqControl_A1(uint32 u32IdleTime)
{
	uint32 u32TargetFreq;
	int i;
	/*Downgrade the freq if the IDLE > 5 (5%) */
	if(u32IdleTime > 5)
	{
		/* Switch Frequency */
		u32TargetFreq=((1000000/TICK)-u32IdleTime)*(MAX_MIPS/(1000000/TICK));
		gu32MIPS=FindFreq(u32TargetFreq);

		if (gu32MIPS != gu32OldMIPS) /* delay of switching frequency */
		{
			//fprintf(fp, "Idle Interval: %d - Frequency has been updated to %d MHz\n", u32IdleTime, gu32MIPS/1000000);
			for(i = 0; i < SWITCH_DELAY; i += STEP)
			{
				gu32SimTime += STEP;
				update_ST();
				UpdateActiveTime();
			}
			gu32OldMIPS=gu32MIPS;
		} 
		else 
		{
			//fprintf(fp, "No need to change frequency, current frequency=%d MHz\n",gu32MIPS/1000000);
		}
	}
	else 
		gu32MIPS=MAX_MIPS;
}

uint32 CalcEnergySaving(void)

{   
	uint32 u32EnergyNoAlg, u32SavingPercentage = 0;
	static uint32 u32EnergyWithAlg= 0,u32SampleCnt = 0;
	u32SampleCnt++;
	u32EnergyWithAlg += (gu32MIPS/1000000); /* Accumulated amount of energy dissipated with algorithm applied*/
	u32EnergyNoAlg = u32SampleCnt*(MAX_MIPS/1000000); /* Accumulated amount of energy dissipated without algorithm applied*/
	u32SavingPercentage = 100-((u32EnergyWithAlg*100)/u32EnergyNoAlg); /* Amount of energy saved*/
	return u32SavingPercentage;

}


void UpdateActiveTime(void)
{
	int i;

	for(i = 0; i < NO_OF_TASKS; i++)
		if(aTasks[i].state == READY || aTasks[i].state == RUNNING)
			aTasks[i].u32ActiveTime += STEP;
}


tstrTask* find_Rdy_Task(void)
{
	
	uint8 u8CurrentPriority = LOWEST_PRIORITY + 1;
	uint8 i;
	uint8 u8IndxReadyTask = NO_OF_TASKS + 1; /* set to invalid value so that we retun null if we can't find a ready task*/

	for(i = 0; i < NO_OF_TASKS; i++)
	{
		if(aTasks[i].state!=SLEEPING && aTasks[i].u8Priority < LOWEST_PRIORITY) /*high priority is smaller number*/
		{
			u8CurrentPriority = aTasks[i].u8Priority;
			u8IndxReadyTask = i;
			if(u8CurrentPriority== HIGHEST_PRIORITY) 
				break;
		}
	}
	if(u8IndxReadyTask < NO_OF_TASKS)
		return &aTasks[u8IndxReadyTask];
	else
		return NULL;
}

void update_EIC(tstrTask *task)
{
	task->u32ExecInstCnt += TICK*(gu32MIPS/1000000);
	/* if task finished execution, put it in sleep state if its active time is less than the period */
	if(task->u32ExecInstCnt >= task->u32InstCnt) /*i.e finsihed execution*/
	{
		/* make sure a task didn't miss its deadline */
		if(task->u32ActiveTime < task->u32Period) 
		{
			sleep_task(task);
		}
		/* if the task needed time more than its period than consider it reset*/
		else
		{
	 		task->u32ExecInstCnt = 0; 
			//fprintf(fp, "active time %d period %d\n",task->u32ActiveTime, task->u32Period);
			//fprintf(fp, "missing deadline of %d by %d\n",task->u8Priority,task->u32ActiveTime-task->u32Period);
			task->u32ActiveTime = task->u32ActiveTime - task->u32Period;
			
		}

	}	
}
	
void sleep_task(tstrTask* task)
{
	task->state = SLEEPING;
	task->u32ExecInstCnt = 0;
	task->u32IdleTime = task->u32SleepTime;
	//fprintf(fp, "task %d went to sleep %d\n", task->u8Priority, task->u32SleepTime);
}

void update_ST(void)
{
	uint8 i;
	for(i = 0; i < NO_OF_TASKS; i++)
	{
		if(aTasks[i].state == SLEEPING)
		{	
			aTasks[i].u32IdleTime -= STEP;
			if(aTasks[i].u32IdleTime <= 0) /*Sleeping time is over*/
			{
				aTasks[i].state = READY;
				aTasks[i].u32ActiveTime = 0;
				fp = fopen("test.txt","a");
				fprintf(fp, "task %d pri %d is ready to run\n",i, aTasks[i].u8Priority);
				fclose(fp);
			}
		}
	}
}
