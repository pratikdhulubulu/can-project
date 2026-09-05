#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/*-----------------------------------------------------------
 * Application specific definitions.
 * Adjust these for your hardware and project.
 *----------------------------------------------------------*/

/* Scheduler & System */
#define configUSE_PREEMPTION            1
#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0
#define configCPU_CLOCK_HZ              ((uint32_t)168000000)  /* STM32F446 running at 168MHz */
#define configTICK_RATE_HZ              ((TickType_t)1000)    /* 1ms tick */
#define configMAX_PRIORITIES            5
#define configMINIMAL_STACK_SIZE        ((uint16_t)128)       /* in words */
#define configTOTAL_HEAP_SIZE           ((size_t)(10*1024))   /* heap_4 */
#define configMAX_TASK_NAME_LEN         16
#define configUSE_16_BIT_TICKS          0
#define configIDLE_SHOULD_YIELD         1

/* Mutexes and Semaphores */
#define configUSE_MUTEXES               1
#define configUSE_RECURSIVE_MUTEXES     1
#define configUSE_COUNTING_SEMAPHORES   1
#define configUSE_QUEUE_SETS            1

/* Software timers */
#define configUSE_TIMERS                0
#define configTIMER_TASK_PRIORITY       (configMAX_PRIORITIES-1)
#define configTIMER_QUEUE_LENGTH        5
#define configTIMER_TASK_STACK_DEPTH    (configMINIMAL_STACK_SIZE * 2)

/* Co-routines (not used, can set 0) */
#define configUSE_CO_ROUTINES           0
#define configMAX_CO_ROUTINE_PRIORITIES 2

/* API function inclusion */
#define INCLUDE_vTaskPrioritySet        1
#define INCLUDE_uxTaskPriorityGet       1
#define INCLUDE_vTaskDelete             1
#define INCLUDE_vTaskSuspend            1
#define INCLUDE_xResumeFromISR          1
#define INCLUDE_vTaskDelay              1
#define INCLUDE_vTaskDelayUntil         1
#define INCLUDE_xTaskGetSchedulerState  1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define INCLUDE_xTaskGetCurrentTaskHandle 1

/* Cortex-M specific definitions */
#ifdef __NVIC_PRIO_BITS
    #define configPRIO_BITS              __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS              4   /* STM32F4 default */
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         0xf
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5

/* Kernel interrupt priorities for Cortex-M */
#define configKERNEL_INTERRUPT_PRIORITY  (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#endif /* FREERTOS_CONFIG_H */
