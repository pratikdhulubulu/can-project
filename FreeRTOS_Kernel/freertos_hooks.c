/**
 * @file    freertos_hooks.c
 * @brief   FreeRTOS hook functions for STM32F446RE project.
 *
 * @details
 * This file contains user-defined hook functions that FreeRTOS calls
 * under certain conditions (optional). All hooks are weakly linked,
 * so you can modify them as needed.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f446xx.h"  // Include HAL if needed for debugging/peripherals

/*-----------------------------------------------------------
 * Idle Hook
 *----------------------------------------------------------*/
#if configUSE_IDLE_HOOK
/**
 * @brief  Function called by the Idle task.
 * @note   Runs in the context of the idle task.
 */
void vApplicationIdleHook(void)
{
    /* Example: Enter low-power mode or toggle debug LED */
}
#endif /* configUSE_IDLE_HOOK */

/*-----------------------------------------------------------
 * Tick Hook
 *----------------------------------------------------------*/
#if configUSE_TICK_HOOK
/**
 * @brief  Function called every system tick (configTICK_RATE_HZ)
 */
void vApplicationTickHook(void)
{
    /* Example: Update software timers or counters */
}
#endif /* configUSE_TICK_HOOK */

/*-----------------------------------------------------------
 * Malloc Failed Hook
 *----------------------------------------------------------*/
#if configUSE_MALLOC_FAILED_HOOK
/**
 * @brief  Called if pvPortMalloc fails to allocate memory.
 * @note   This is critical; typically halt or log error.
 */
void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for (;;)
    {
        /* Memory allocation failed: loop forever */
    }
}
#endif /* configUSE_MALLOC_FAILED_HOOK */

/*-----------------------------------------------------------
 * Stack Overflow Hook
 *----------------------------------------------------------*/
#if configCHECK_FOR_STACK_OVERFLOW > 0
/**
 * @brief  Called if a task overflows its stack.
 * @param  pxTaskHandle: handle of offending task
 * @param  pcTaskName:   name of offending task
 */
void vApplicationStackOverflowHook(TaskHandle_t pxTaskHandle, char *pcTaskName)
{
    (void)pxTaskHandle;
    (void)pcTaskName;

    taskDISABLE_INTERRUPTS();
    for (;;)
    {
        /* Stack overflow detected: loop forever */
    }
}
#endif /* configCHECK_FOR_STACK_OVERFLOW */

/*-----------------------------------------------------------
 * Optional: Pre/Post Sleep Hook for tickless idle
 *----------------------------------------------------------*/
#if configUSE_IDLE_HOOK && defined(configUSE_TICKLESS_IDLE)
/**
 * @brief  Called before MCU enters sleep in tickless idle mode
 */
void vApplicationSleep(TickType_t xExpectedIdleTime)
{
    (void)xExpectedIdleTime;
    /* Implement low-power mode if needed */
}
#endif
