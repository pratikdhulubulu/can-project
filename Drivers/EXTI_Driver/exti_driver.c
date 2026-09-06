/**
 * @file    exti_driver.c
 * @author  Pratik Dhulubulu
 * @brief   This file contains initialization, callback handling,
 *          and IRQ processing logic EXTI lines.
 */

#include "exti_driver.h"
#include "stm32f446xx.h"
#include "rcc_driver.h"

/**
* @section Private Function Declarations.
*/
static fp_exti_callback fp_exti_callback_table[EXTI_LINE_MAX] = { (fp_exti_callback)0 };
static void extiPortSelect(const EXTI_CONFIG *ptr_cfg);
static void extiTriggerConfig(const EXTI_CONFIG *ptr_cfg);
static void extiClearPending(EXTI_LINE line);
static void extiEnableNVIC(const EXTI_CONFIG *ptr_cfg);

/**
* @section Public Function Definations.
*/

/**
 * @brief  This function initializes a specific EXTI line.
 * @param  ptr_cfg Pointer to configuration structure.
 * @return None.
 */
void extiInit(const EXTI_CONFIG *ptr_cfg)
{
    if (ptr_cfg == NULL)
    {
        return;
    }

    extiPortSelect(ptr_cfg);

    extiTriggerConfig(ptr_cfg);

    EXTI->IMR |= (1UL << (uint32_t)ptr_cfg->line);

    extiClearPending(ptr_cfg->line);

    extiEnableNVIC(ptr_cfg);
}

/**
 * @brief  This function registers a callback for a specific EXTI line.
 * @param  line EXTI line.
 * @param  ptr_callback Pointer to user handler function.
 * @return None.
 */
void extiRegisterCallback(EXTI_LINE line, fp_exti_callback ptr_callback)
{
    if (line < EXTI_LINE_MAX)
    {
        fp_exti_callback_table[line] = ptr_callback;
    }
}

/**
 * @brief  This function handles EXTI interrupt event.
 * @param  line EXTI line.
 * @return None.
 */
void extiHandleIrq(EXTI_LINE line)
{
    if (line >= EXTI_LINE_MAX)
    {
        return;
    }

    extiClearPending(line);

    if (fp_exti_callback_table[line] != (fp_exti_callback)0)
    {
        fp_exti_callback_table[line]();
    }
}

/**
* @section Private Function Definations
*/

/**
 * @brief  This function selects the port.
 * @param  ptr_cfg Pointer to configuration structure.
 * @return None.
 */
static void extiPortSelect(const EXTI_CONFIG *ptr_cfg)
{
    uint32_t index = ptr_cfg->line / 4U;
    uint32_t shift = (ptr_cfg->line % 4U) * 4U;
    uint32_t port_code = ((uint32_t)ptr_cfg->ptr_port - (uint32_t)GPIOA) / 0x400U;
    rccEnableAPB2(RCC_APB2ENR_SYSCFGEN);

    SYSCFG->EXTICR[index] &= ~(0xFUL << shift);
    SYSCFG->EXTICR[index] |= (port_code << shift);
}

/**
 * @brief  This function configures edge triggers.
 * @param  ptr_cfg Pointer to configuration structure.
 * @return None.
 */
static void extiTriggerConfig(const EXTI_CONFIG *ptr_cfg)
{
    uint32_t mask = 1UL << (uint32_t)ptr_cfg->line;

    EXTI->RTSR &= ~mask;
    EXTI->FTSR &= ~mask;

    if (ptr_cfg->trigger == EXTI_TRIGGER_RISING)
    {
        EXTI->RTSR |= mask;
    }
    else if (ptr_cfg->trigger == EXTI_TRIGGER_FALLING)
    {
        EXTI->FTSR |= mask;
    }
    else
    {
        EXTI->RTSR |= mask;
        EXTI->FTSR |= mask;
    }
}

/**
 * @brief  This function clears pending interrupt bit.
 * @param  line EXTI line.
 * @return None.
 */
static void extiClearPending(EXTI_LINE line)
{
    uint32_t mask = 1UL << (uint32_t)line;
    EXTI->PR = mask;
}

/**
 * @brief  This function set priority and enables NVIC for EXTI line.
 * @param  ptr_cfg Pointer to configuration structure.
 * @return None.
 */
static void extiEnableNVIC(const EXTI_CONFIG *ptr_cfg)
{
    IRQn_Type irq;

    if (ptr_cfg->line <= EXTI_LINE_4)
    {
        irq = (IRQn_Type)(EXTI0_IRQn + ptr_cfg->line);
    }
    else if (ptr_cfg->line <= EXTI_LINE_9)
    {
        irq = EXTI9_5_IRQn;
    }
    else
    {
        irq = EXTI15_10_IRQn;
    }

    NVIC_SetPriority(irq, ptr_cfg->priority);
    NVIC_EnableIRQ(irq);
}
