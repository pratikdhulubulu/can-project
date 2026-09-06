/**
 * @file    timer_driver.c
 * @author  Pratik Dhulubulu
 * @brief   This file implements timer initialization, mode configuration,
 *          start/stop control, and interrupt handling for general-purpose timers.
 */

#include "timer_driver.h"
#include "rcc_driver.h"

/**
 * @section Private Function Declarations.
 */
static void timerConfigBase(const TIM_CONFIG *ptr_cfg);
static void timerConfigPwm(const TIM_CONFIG *ptr_cfg);
static void timerConfigInputCapture(const TIM_CONFIG *ptr_cfg);
static void timerConfigOutputCompare(const TIM_CONFIG *ptr_cfg);
static void timerConfigEncoder(const TIM_CONFIG *ptr_cfg);

/**
 * @section Public Function Definations.
 */

/**
 * @brief   This function initializes timer as per provided configuration.
 * @param   ptr_cfg Pointer to timer configuration structure.
 * @return  None.
 */
void timerInit(const TIM_CONFIG *ptr_cfg)
{
    if (ptr_cfg->ptr_tim == TIM1) {
        rccEnableAPB2(RCC_APB2ENR_TIM1EN);
    } 
    else if (ptr_cfg->ptr_tim == TIM2) {
        rccEnableAPB1(RCC_APB1ENR_TIM2EN);
    }
    else if (ptr_cfg->ptr_tim == TIM3) {
        rccEnableAPB1(RCC_APB1ENR_TIM3EN);
    }
    else if (ptr_cfg->ptr_tim == TIM4) {
        rccEnableAPB1(RCC_APB1ENR_TIM4EN);
    }
    else if (ptr_cfg->ptr_tim == TIM5) {
        rccEnableAPB1(RCC_APB1ENR_TIM5EN);
    }
    else if (ptr_cfg->ptr_tim == TIM8) {
        rccEnableAPB2(RCC_APB2ENR_TIM8EN);
    }
    else if (ptr_cfg->ptr_tim == TIM9) {
        rccEnableAPB2(RCC_APB2ENR_TIM9EN);
    }
    else if (ptr_cfg->ptr_tim == TIM10) {
        rccEnableAPB2(RCC_APB2ENR_TIM10EN);
    }
    else if (ptr_cfg->ptr_tim == TIM11) {
        rccEnableAPB2(RCC_APB2ENR_TIM11EN);
    }
    
    ptr_cfg->ptr_tim->CR1 = 0u;
    ptr_cfg->ptr_tim->CR2 = 0u;
    ptr_cfg->ptr_tim->DIER = 0u;

    switch (ptr_cfg->mode) {
        case TIM_MODE_BASIC:
            timerConfigBase(ptr_cfg);
            break;

        case TIM_MODE_PWM:
            timerConfigPwm(ptr_cfg);
            break;

        case TIM_MODE_INPUT_CAPTURE:
            timerConfigInputCapture(ptr_cfg);
            break;

        case TIM_MODE_OUTPUT_COMPARE:
            timerConfigOutputCompare(ptr_cfg);
            break;

        case TIM_MODE_ENCODER:
            timerConfigEncoder(ptr_cfg);
            break;

        default:
            break;
    }
}

/**
 * @brief   This function starts the timer counter.
 * @param   ptr_cfg Pointer to timer configuration structure.
 * @return  None.
 */
void timerStart(const TIM_CONFIG *ptr_cfg)
{
    ptr_cfg->ptr_tim->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief   This function stops the timer counter.
 * @param   ptr_cfg Pointer to timer configuration structure.
 * @return  None.
 */
void timerStop(const TIM_CONFIG *ptr_cfg)
{
    ptr_cfg->ptr_tim->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief   This function handles timer update interrupt events.
 * @param   ptr_tim Pointer to timer instance that generated interrupt.
 * @return  None.
 */
void timerHandleIrq(TIM_TypeDef *ptr_tim)
{
    /* Check update interrupt flag */
    if ((ptr_tim->SR & TIM_SR_UIF) != 0u) 
    {
        /* Clear interrupt flag */
        ptr_tim->SR &= ~TIM_SR_UIF;

        /* ---- PWM Pulse Auto Update Logic ---- */
        uint32_t arr_val = ptr_tim->ARR;
        uint32_t next = ptr_tim->CCR1 + 1u;

        if (next >= arr_val) 
        {
            next = 0u;
        }

        ptr_tim->CCR1 = next;
    }
}

/**
 * @section Private Function Definations.
 */

/**
 * @brief   This function configures timer in basic timer mode.
 * @param   ptr_cfg Pointer to timer configuration structure.
 * @return  None.
 */
static void timerConfigBase(const TIM_CONFIG *ptr_cfg)
{
    ptr_cfg->ptr_tim->PSC = ptr_cfg->prescaler;
    ptr_cfg->ptr_tim->ARR = ptr_cfg->period;
    ptr_cfg->ptr_tim->EGR = TIM_EGR_UG;
}

/**
 * @brief   This function configures timer in PWM mode.
 * @param   ptr_cfg Pointer to timer configuration structure.
 * @return  None.
 */
static void timerConfigPwm(const TIM_CONFIG *ptr_cfg)
{
    ptr_cfg->ptr_tim->PSC = ptr_cfg->prescaler;
    ptr_cfg->ptr_tim->ARR = ptr_cfg->period;

    switch (ptr_cfg->channel) {
        case TIM_CHANNEL_1:
            ptr_cfg->ptr_tim->CCMR1 &= ~TIM_CCMR1_OC1M;
            ptr_cfg->ptr_tim->CCMR1 |= (6u << TIM_CCMR1_OC1M_Pos);
            ptr_cfg->ptr_tim->CCMR1 |= TIM_CCMR1_OC1PE;
            ptr_cfg->ptr_tim->CCR1 = ptr_cfg->pulse;
            ptr_cfg->ptr_tim->CCER |= TIM_CCER_CC1E;
            break;

        case TIM_CHANNEL_2:
            ptr_cfg->ptr_tim->CCMR1 &= ~TIM_CCMR1_OC2M;
            ptr_cfg->ptr_tim->CCMR1 |= (6u << TIM_CCMR1_OC2M_Pos);
            ptr_cfg->ptr_tim->CCMR1 |= TIM_CCMR1_OC2PE;
            ptr_cfg->ptr_tim->CCR2 = ptr_cfg->pulse;
            ptr_cfg->ptr_tim->CCER |= TIM_CCER_CC2E;
            break;

        case TIM_CHANNEL_3:
            ptr_cfg->ptr_tim->CCMR2 &= ~TIM_CCMR2_OC3M;
            ptr_cfg->ptr_tim->CCMR2 |= (6u << TIM_CCMR2_OC3M_Pos);
            ptr_cfg->ptr_tim->CCMR2 |= TIM_CCMR2_OC3PE;
            ptr_cfg->ptr_tim->CCR3 = ptr_cfg->pulse;
            ptr_cfg->ptr_tim->CCER |= TIM_CCER_CC3E;
            break;

        case TIM_CHANNEL_4:
            ptr_cfg->ptr_tim->CCMR2 &= ~TIM_CCMR2_OC4M;
            ptr_cfg->ptr_tim->CCMR2 |= (6u << TIM_CCMR2_OC4M_Pos);
            ptr_cfg->ptr_tim->CCMR2 |= TIM_CCMR2_OC4PE;
            ptr_cfg->ptr_tim->CCR4 = ptr_cfg->pulse;
            ptr_cfg->ptr_tim->CCER |= TIM_CCER_CC4E;
            break;

        default:
            break;
    }

    ptr_cfg->ptr_tim->EGR = TIM_EGR_UG;
}

/**
 * @brief   This function configures timer in input capture mode.
 * @param   ptr_cfg Pointer to timer configuration structure.
 * @return  None.
 */
static void timerConfigInputCapture(const TIM_CONFIG *ptr_cfg)
{
    ptr_cfg->ptr_tim->PSC = ptr_cfg->prescaler;
    ptr_cfg->ptr_tim->ARR = ptr_cfg->period;

    switch (ptr_cfg->channel) {
        case TIM_CHANNEL_1:
            ptr_cfg->ptr_tim->CCMR1 &= ~TIM_CCMR1_CC1S;
            ptr_cfg->ptr_tim->CCMR1 |= 1u;
            ptr_cfg->ptr_tim->CCER |= TIM_CCER_CC1E;
            break;

        case TIM_CHANNEL_2:
            ptr_cfg->ptr_tim->CCMR1 &= ~TIM_CCMR1_CC2S;
            ptr_cfg->ptr_tim->CCMR1 |= (1u << TIM_CCMR1_CC2S_Pos);
            ptr_cfg->ptr_tim->CCER |= TIM_CCER_CC2E;
            break;

        default:
            break;
    }

    ptr_cfg->ptr_tim->EGR = TIM_EGR_UG;
}

/**
 * @brief   This function configures timer in output compare mode.
 * @param   ptr_cfg Pointer to timer configuration structure.
 * @return  None.
 */
static void timerConfigOutputCompare(const TIM_CONFIG *ptr_cfg)
{
    ptr_cfg->ptr_tim->PSC = ptr_cfg->prescaler;
    ptr_cfg->ptr_tim->ARR = ptr_cfg->period;

    switch (ptr_cfg->channel) {
        case TIM_CHANNEL_1:
            ptr_cfg->ptr_tim->CCR1 = ptr_cfg->pulse;
            ptr_cfg->ptr_tim->CCMR1 |= TIM_CCMR1_OC1PE;
            ptr_cfg->ptr_tim->CCER |= TIM_CCER_CC1E;
            break;

        case TIM_CHANNEL_2:
            ptr_cfg->ptr_tim->CCR2 = ptr_cfg->pulse;
            ptr_cfg->ptr_tim->CCMR1 |= TIM_CCMR1_OC2PE;
            ptr_cfg->ptr_tim->CCER |= TIM_CCER_CC2E;
            break;

        default:
            break;
    }

    ptr_cfg->ptr_tim->EGR = TIM_EGR_UG;
}

/**
 * @brief   This function configures timer in encoder interface mode.
 * @param   ptr_cfg Pointer to timer configuration structure.
 * @return  None.
 */
static void timerConfigEncoder(const TIM_CONFIG *ptr_cfg)
{
    ptr_cfg->ptr_tim->PSC = ptr_cfg->prescaler;
    ptr_cfg->ptr_tim->ARR = ptr_cfg->period;

    ptr_cfg->ptr_tim->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;

    ptr_cfg->ptr_tim->CCMR1 |= (1u << TIM_CCMR1_CC1S_Pos);
    ptr_cfg->ptr_tim->CCMR1 |= (1u << TIM_CCMR1_CC2S_Pos);

    ptr_cfg->ptr_tim->CCER |= TIM_CCER_CC1E;
    ptr_cfg->ptr_tim->CCER |= TIM_CCER_CC2E;

    ptr_cfg->ptr_tim->EGR = TIM_EGR_UG;
}
