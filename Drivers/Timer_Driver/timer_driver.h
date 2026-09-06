/**
 * @file    timer_driver.h
 * @author  Pratik Dhulubulu
 * @brief   Timer Driver Interface.
 */
 
#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <stdint.h>
#include "stm32f446xx.h"

/**
 * @section Public Type Declaration.
 */
typedef enum {
    TIM_MODE_BASIC = 0u,     
    TIM_MODE_PWM,            
    TIM_MODE_INPUT_CAPTURE,    
    TIM_MODE_OUTPUT_COMPARE,
    TIM_MODE_ENCODER 
} TIM_MODE;

typedef enum {
    TIM_CHANNEL_1 = 0u,
    TIM_CHANNEL_2,
    TIM_CHANNEL_3,
    TIM_CHANNEL_4
} TIM_CHANNEL;

typedef struct {
    TIM_TypeDef *ptr_tim;
    uint32_t clock_hz;
    TIM_MODE mode;
    uint32_t prescaler;
    uint32_t period;
    TIM_CHANNEL channel;
    uint32_t pulse;
    uint32_t polarity;
} TIM_CONFIG;

/**
 * @section Public Function Declarations.
 */
void timerInit(const TIM_CONFIG *ptr_cfg);
void timerStart(const TIM_CONFIG *ptr_cfg);
void timerStop(const TIM_CONFIG *ptr_cfg);
void timerHandleIrq(TIM_TypeDef *ptr_tim);

#endif
