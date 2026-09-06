/**
 * @file    rcc_driver.h
 * @author  Pratik Dhulubulu
 * @brief   Reset and Clock Control Driver Interface.
 */
 
#ifndef RCC_DRIVER_H
#define RCC_DRIVER_H

#include <stddef.h>
#include <stdint.h>
#include "stm32f446xx.h"

/**
* @section  Public Type Declaration
*/
typedef enum {
    RCC_CLK_SRC_HSI = 0,
    RCC_CLK_SRC_HSE,
    RCC_CLK_SRC_PLL
} RCC_CLK_SRC;

typedef struct {
    uint32_t SRC;
    uint32_t M;
    uint32_t N;
    uint32_t P;
    uint32_t Q;
} RCC_PLL_CFG;

typedef struct {
    RCC_CLK_SRC CLK_SOURCE;
    RCC_PLL_CFG PLL;
    uint32_t AHB_PRESCALER;
    uint32_t APB1_PRESCALER;
    uint32_t APB2_PRESCALER;
    uint32_t FLASH_LATENCY;
} RCC_SYS_CFG;

/**
 * @section Public Functions Declaration
 */
int rccSystemClockConfig(const RCC_SYS_CFG *ptr_config);
void rccEnableAHB1(uint32_t mask);
void rccDisableAHB1(uint32_t mask);
void rccEnableAPB1(uint32_t mask);
void rccDisableAPB1(uint32_t mask);
void rccResetAPB1(uint32_t mask);
void rccEnableAPB2(uint32_t mask);
void rccDisableAPB2(uint32_t mask);
void rccResetAPB2(uint32_t mask);
uint32_t rccGetSYSCLK(void);
uint32_t rccGetHCLK(void);
uint32_t rccGetPCLK1(void);
uint32_t rccGetPCLK2(void);

#endif
