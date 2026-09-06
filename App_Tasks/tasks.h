/**
 * @file    tasks.h
 * @brief   Application Tasks declarations for FreeRTOS
 * @details Contains prototypes for Task One and Task Two.
 */

#ifndef TASKS_H
#define TASKS_H

#include "FreeRTOS.h"
#include "task.h"

#define TASK_STACK_SIZE   128
#define TASK_PRIORITY     1

void osInit(void);
void Task(void *pvParameters);

#endif
