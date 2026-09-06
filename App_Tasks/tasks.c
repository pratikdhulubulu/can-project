/**
 * @file    tasks.c
 * @brief   Application Tasks implementation for FreeRTOS
 * @details Implements os init and tasks.
 */

#include "tasks.h"
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "semphr.h"



#define DELAY_200_MS 200

void osInit(void)
{
    /* Create RTOS tasks*/
    xTaskCreate(Task, "Task", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);

    /* Start scheduler */
    vTaskStartScheduler();
}

void Task(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        /* CAN processing to be implemented */
        vTaskDelay(pdMS_TO_TICKS(DELAY_200_MS));
    }
}

