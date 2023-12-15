/*
 * scheduler.c
 *
 *  Created on: Dec 15, 2023
 *      Author: truon
 */

#include "scheduler.h"

uint8_t current_index_task = 0;
unsigned char Error_code_G = 0;
sTask SCH_tasks_G[SCH_MAX_TASKS];

void SCH_Init(void) {
    unsigned char i;
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_Delete_Task(i);
    }
    Error_code_G = 0;
}

void SCH_Update(void){
    unsigned char Index;
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        if (SCH_tasks_G[Index].pTask){
            if (SCH_tasks_G[Index].Delay == 0) {
                SCH_tasks_G[Index].RunMe += 1;
                if (SCH_tasks_G[Index].Period) {
                    SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                }
            } else {
                SCH_tasks_G[Index].Delay -= TIME_CYCLE;
            }
        }
    }
}

void SCH_Add_Task(void (* pFunction)(), unsigned int DELAY, unsigned int PERIOD)
{
    if (current_index_task == SCH_MAX_TASKS)
    {
    	return;
    }
    current_index_task++;
	SCH_tasks_G[current_index_task].pTask = pFunction;
	SCH_tasks_G[current_index_task].Delay = DELAY;
	SCH_tasks_G[current_index_task].Period = PERIOD;
	SCH_tasks_G[current_index_task].RunMe = 0;
	SCH_tasks_G[current_index_task].TaskID = current_index_task;

}

void SCH_Dispatch_Tasks(void)
{
    unsigned char Index;
    for (Index = 0; Index < SCH_MAX_TASKS; Index++){
        if (SCH_tasks_G[Index].RunMe > 0) {
            (*SCH_tasks_G[Index].pTask)();
            SCH_tasks_G[Index].RunMe -= 1;
            if (SCH_tasks_G[Index].Period == 0)
            {
                SCH_Delete_Task(Index);
            }
        }
    }
}

void SCH_Delete_Task(uint32_t TASK_INDEX){
    if (TASK_INDEX > SCH_MAX_TASKS || TASK_INDEX < NO_TASK_ID) {
    	return;
    }
    if (SCH_tasks_G[TASK_INDEX].pTask == 0) {
        return;
    }
    SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
    SCH_tasks_G[TASK_INDEX].Delay = 0;
    SCH_tasks_G[TASK_INDEX].Period = 0;
    SCH_tasks_G[TASK_INDEX].RunMe = 0;
}
