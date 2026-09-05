/**
 * @file   interrupt_handler.c
 * @author Pratik Dhulubulu
 * @brief  This file implements system and peripheral interrupt handlers.
 */

#include "interrupt_handler.h"
#include "exti_driver.h"
#include "systick_driver.h"
#include "timer_driver.h"

/**
 * @section System Exception Handlers.
 */

/**
 * @brief  Handles System Tick interrupt.
 * @param  None
 * @return None
 */
void SysTick_Handler(void)
{
    tick++;
}

/**
 * @section Peripheral Interrupt Handlers.
 */

/**
 * @brief  Handles EXTI Line 0 interrupt.
 * @param  None
 * @return None
 */
void EXTI0_IRQHandler(void)
{
    extiHandleIrq(EXTI_LINE_0);
}

/**
 * @brief  Handles EXTI Line 1 interrupt.
 * @param  None
 * @return None
 */
void EXTI1_IRQHandler(void)
{
    extiHandleIrq(EXTI_LINE_1);
}

/**
 * @brief  Handles EXTI Line 2 interrupt.
 * @param  None
 * @return None
 */
void EXTI2_IRQHandler(void)
{
    extiHandleIrq(EXTI_LINE_2);
}

/**
 * @brief  Handles EXTI Line 3 interrupt.
 * @param  None
 * @return None
 */
void EXTI3_IRQHandler(void)
{
    extiHandleIrq(EXTI_LINE_3);
}

/**
 * @brief  Handles EXTI Line 4 interrupt.
 * @param  None
 * @return None
 */
void EXTI4_IRQHandler(void)
{
    extiHandleIrq(EXTI_LINE_4);
}

/**
 * @brief  Handles EXTI Line 5–9 shared interrupt.
 * @param  None
 * @return None
 */
void EXTI9_5_IRQHandler(void)
{
    extiHandleIrq(5U);
    extiHandleIrq(6U);
    extiHandleIrq(7U);
    extiHandleIrq(8U);
    extiHandleIrq(9U);
}

/**
 * @brief  Handles EXTI Line 10–15 shared interrupt.
 * @param  None
 * @return None
 */
void EXTI15_10_IRQHandler(void)
{
    extiHandleIrq(10U);
    extiHandleIrq(11U);
    extiHandleIrq(12U);
    extiHandleIrq(13U);
    extiHandleIrq(14U);
    extiHandleIrq(15U);
}

/**
 * @brief  Handles Timer 2 interrupt.
 * @param  None
 * @return None
 */
void TIM2_IRQHandler(void)
{
    timerHandleIrq(TIM2);
}