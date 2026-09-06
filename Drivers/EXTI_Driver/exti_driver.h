/**
 * @file    exti_driver.h
 * @author  Pratik Dhulubulu
 * @brief   External Interrupt Controller Driver Interface.
 */
#ifndef EXTI_DRIVER_H
#define EXTI_DRIVER_H

#include <stddef.h>
#include <stdint.h>
#include "stm32f446xx.h"

/** 
* @section Public Type Declarations.
*/
typedef enum
{
    EXTI_LINE_0  = 0,
    EXTI_LINE_1,
    EXTI_LINE_2,
    EXTI_LINE_3,
    EXTI_LINE_4,
    EXTI_LINE_5,
    EXTI_LINE_6,
    EXTI_LINE_7,
    EXTI_LINE_8,
    EXTI_LINE_9,
    EXTI_LINE_10,
    EXTI_LINE_11,
    EXTI_LINE_12,
    EXTI_LINE_13,
    EXTI_LINE_14,
    EXTI_LINE_15,
    EXTI_LINE_MAX
} EXTI_LINE;

typedef enum
{
    EXTI_TRIGGER_RISING = 0,
    EXTI_TRIGGER_FALLING,
    EXTI_TRIGGER_BOTH
} EXTI_TRIGGER;

typedef struct
{
    GPIO_TypeDef  *ptr_port;
    EXTI_LINE     line;
    EXTI_TRIGGER  trigger;
    uint8_t       priority;
} EXTI_CONFIG;

/**
 * @brief Callback function pointer for EXTI user handlers.
 */
typedef void (*fp_exti_callback)(void);

/**
* @section Public Function Declarations.
*/
void extiInit(const EXTI_CONFIG *ptr_cfg);
void extiRegisterCallback(EXTI_LINE line, fp_exti_callback ptr_callback);
void extiHandleIrq(EXTI_LINE line);

#endif