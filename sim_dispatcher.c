#include"sim_task.h"
#include"sim_core.h"
#include <stdio.h>


extern uint32 gu32IdleTime;
extern uint32 gu32SimTime;
extern uint32 gu32EnergySaving;
extern tstrCore aCores[NO_OF_CORES];
FILE *fp, *fp2;



void Simulate(void)
{
	tstrTask *task = NULL, *task2 = NULL, *oldtask = NULL, *oldtask2 = NULL;
	//uint8 i =0 , j = 0, u8CoreIndx = 0, u8TaskIsScheduled = 0;
	

	//fp = fopen("test.txt","a");

	while(1)
	{
		/*each tick search for ready task and update ST for sleeping tasks*/
		if ((gu32SimTime%TICK) == 0)
		{
			
			if(task) update_EIC(task);
			if(task2) update_EIC(task2);

			task = find_Rdy_Task();
			task2 = find_Rdy_Task();
			if(task)
			{
				if(oldtask != task)
				{ 
					fp = fopen("test.txt","a");
					fprintf(fp, "[%06d] Start Running task %d on core1\n",gu32SimTime,task->u8Priority);
					fclose(fp);
					oldtask = task;
					aCores[0].u8NbOfTasks ++;
				}
				task->state=RUNNING;
			}
			if(task2)
			{
				if(oldtask2 != task2)
				{
					fp = fopen("test.txt","a");
					fprintf(fp, "[%06d] Start Running task %d on core2\n",gu32SimTime,task->u8Priority);
					fclose(fp);
					oldtask2 = task2;
					aCores[1].u8NbOfTasks ++;
				}
				task2->state=RUNNING;
			}
#if 0
				/*Search for a core in Standby Mode*/
				for(i = 0; i < NO_OF_CORES; i++)
				{
					if(aCores[i].state == STANDBY_MODE)
					{
						aCores[i].state = RUN_MODE;
						aCores[i].u8NbOfTasks ++;
						u8TaskIsScheduled = 1;
						break;
					}
				}

				/*if all cores are in RUN_MODE, search for core with minimum number of tasks*/
				if(!u8TaskIsScheduled)
				{
					if (aCores[0].u8NbOfTasks < aCores[1].u8NbOfTasks){
						i = 0;
					}
					else {
						i = 1;
					}

					if (aCores[2].u8NbOfTasks < aCores[3].u8NbOfTasks){
						j = 2;
					}
					else{
						j = 3;
					}
					if (aCores[i].u8NbOfTasks < aCores[j].u8NbOfTasks){
						u8CoreIndx = i;
					}
					else{
						u8CoreIndx = j;
					}

					aCores[u8CoreIndx].u8NbOfTasks ++;
					u8TaskIsScheduled = 1;

				}
#endif
					
			if(!task && !task2)
			{
				gu32IdleTime++;
				//fp2 = fopen("percentage.txt","a");
				//fprintf(fp2,"[%06d] No task to run!\n",gu32SimTime);
				//fclose(fp2);
			}
		}
		
		//monitor CPU utilization each 1 sec
		if(gu32SimTime > 0 && (gu32SimTime % SAMPLING) == 0)
		{	
			FreqControl_A1(gu32IdleTime);
			gu32EnergySaving = CalcEnergySaving();
			fp2 = fopen("percentage.txt","a");
			fprintf(fp2, "Energy saving percentage is %d%%\n",gu32EnergySaving);
			fclose(fp2);
			gu32IdleTime = 0;
			gu32SimTime = 0;
		}

			
		
		update_ST();
		UpdateActiveTime();
		gu32SimTime+=STEP;
	}
	//fclose(fp);
}
