/**
 * @file    rcc_driver.c
 * @author  Pratik Dhulubulu
 * @brief   This file implements system clock configuration, bus clock control,
 *          peripheral clock enabling/disabling, and runtime frequency queries.
 */
 
#include "rcc_driver.h"

/**
 * @brief Public Macro Definations.
 */
#define TIMEOUT      50000U
#define RCC_OK       0
#define RCC_ERR_CFG -1
#define RCC_ERR_HSE -2
#define RCC_ERR_PLL -3
#define RCC_ERR_HSI -4
#define RCC_ERR_SYS -5
#define HSE_VALUE    ((uint32_t)8000000)

/**
 * @section Private Function Declarations
 */
static int waitForFlag(volatile uint32_t *ptr_reg, uint32_t flag);

/**
 * @section Public Function Definations.
 */

/**
 * @brief  This function configures the system clock (SYSCLK) and bus prescalers.
 * @param  ptr_config Pointer to RCC_SYS_CFG structure with desired clock configuration.
 * @return RCC_OK on success, otherwise error code.
 */
int rccSystemClockConfig(const RCC_SYS_CFG *ptr_config)
{
    if (ptr_config == NULL)
    {
        return RCC_ERR_CFG;
    }

    /* HSE as SYSCLK */
    if (ptr_config->CLK_SOURCE == RCC_CLK_SRC_HSE)
    {
        /* Enable HSE oscillator */
        RCC->CR |= RCC_CR_HSEON;

        if (waitForFlag(&RCC->CR, RCC_CR_HSERDY) != 0)
        {
            return RCC_ERR_HSE;
        }

        /* Configure flash latency */
        FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) |
                     (ptr_config->FLASH_LATENCY << FLASH_ACR_LATENCY_Pos);

        /* Switch SYSCLK to HSE */
        RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSE;

        /* Wait until HSE is used as SYSCLK */
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE)
        {
            __NOP();
        }

        /* Update SystemCoreClock variable */
        SystemCoreClockUpdate();
        
        return RCC_OK;
    }

    /* PLL as SYSCLK */
    if (ptr_config->CLK_SOURCE == RCC_CLK_SRC_PLL)
    {
        /* Enable PLL source */
        if (ptr_config->PLL.SRC == RCC_CLK_SRC_HSE)
        {
            RCC->CR |= RCC_CR_HSEON;
            if (waitForFlag(&RCC->CR, RCC_CR_HSERDY) != 0)
            {
                return RCC_ERR_PLL;
            }
        }
        else
        {
            RCC->CR |= RCC_CR_HSION;
            if (waitForFlag(&RCC->CR, RCC_CR_HSIRDY) != 0)
            {
                return RCC_ERR_PLL;
            }
        }

        uint32_t pllsrc = 0;
        if (ptr_config->PLL.SRC == RCC_CLK_SRC_HSE)
        {
            pllsrc = RCC_PLLCFGR_PLLSRC_HSE;
        }

        /* Configure PLL multipliers and dividers */
        RCC->PLLCFGR = (ptr_config->PLL.M & 0x3FU) |
                       ((ptr_config->PLL.N & 0x1FF) << 6) |
                       (((ptr_config->PLL.P >> 1) - 1) << 16) |
                       pllsrc |
                       (ptr_config->PLL.Q << 24);

        /* Configure flash latency */
        FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) |
                     (ptr_config->FLASH_LATENCY << FLASH_ACR_LATENCY_Pos);

        /* Enable PLL */
        RCC->CR |= RCC_CR_PLLON;

        if (waitForFlag(&RCC->CR, RCC_CR_PLLRDY) != 0)
        {
            return RCC_ERR_PLL;
        }

        /* Configure AHB and APB prescalers */
        uint32_t rcc_cfgr = RCC->CFGR;
        rcc_cfgr &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
        rcc_cfgr |= (ptr_config->AHB_PRESCALER << 4) & RCC_CFGR_HPRE;
        rcc_cfgr |= (ptr_config->APB1_PRESCALER << 10) & RCC_CFGR_PPRE1;
        rcc_cfgr |= (ptr_config->APB2_PRESCALER << 13) & RCC_CFGR_PPRE2;
        RCC->CFGR = rcc_cfgr;

        /* Switch SYSCLK to PLL */
        RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;

        /* Wait until PLL is used as SYSCLK */
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
        {
            __NOP();
        }

        /* Update SystemCoreClock variable */
        SystemCoreClockUpdate();
        return RCC_OK;
    }

    /* HSI as SYSCLK */
    if (ptr_config->CLK_SOURCE == RCC_CLK_SRC_HSI)
    {
        /* Enable HSI */
        RCC->CR |= RCC_CR_HSION;

        if (waitForFlag(&RCC->CR, RCC_CR_HSIRDY) != 0)
        {
            return RCC_ERR_HSI;
        }

        /* Configure flash latency */
        FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) |
                     (ptr_config->FLASH_LATENCY << FLASH_ACR_LATENCY_Pos);

        /* Switch SYSCLK to HSI */
        RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSI;

        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
        {
            __NOP();
        }

        /* Update SystemCoreClock variable */
        SystemCoreClockUpdate();

        return RCC_OK;
    }

    return RCC_ERR_SYS;
}

/**
 * @brief This function Enables AHB1 peripheral clocks.
 * @param mask Bitmask of peripherals to enable.
 */
void rccEnableAHB1(uint32_t mask)
{
    RCC->AHB1ENR |= mask;
}

/**
 * @brief This function Disables AHB1 peripheral clocks.
 * @param mask Bitmask of peripherals to disable.
 */
void rccDisableAHB1(uint32_t mask)
{
    RCC->AHB1ENR &= ~mask;
}

/**
 * @brief This function Enables APB1 peripheral clocks.
 * @param mask Bitmask of peripherals to enable.
 */
void rccEnableAPB1(uint32_t mask)
{
    RCC->APB1ENR |= mask;
}

/**
 * @brief This function Disables APB1 peripheral clocks.
 * @param mask Bitmask of peripherals to disable.
 */
void rccDisableAPB1(uint32_t mask)
{
    RCC->APB1ENR &= ~mask;
}

/**
 * @brief This function resets APB1 peripherals.
 * @param mask Bitmask of peripherals to reset.
 */
void rccResetAPB1(uint32_t mask)
{
    RCC->APB1RSTR |= mask;
    RCC->APB1RSTR &= ~mask;
}

/**
 * @brief This function enable APB2 peripheral clocks.
 * @param mask Bitmask of peripherals to enable.
 */
void rccEnableAPB2(uint32_t mask)
{
    RCC->APB2ENR |= mask;
}

/**
 * @brief This function disable APB2 peripheral clocks.
 * @param mask Bitmask of peripherals to disable.
 */
void rccDisableAPB2(uint32_t mask)
{
    RCC->APB2ENR &= ~mask;
}

/**
 * @brief This function reset APB2 peripherals.
 * @param mask Bitmask of peripherals to reset.
 */
void rccResetAPB2(uint32_t mask)
{
    RCC->APB2RSTR |= mask;
    RCC->APB2RSTR &= ~mask;
}

/**
 * @brief  This function returns current system clock (SYSCLK) frequency.
 * @return System clock in Hz.
 */
uint32_t rccGetSYSCLK(void)
{
    return SystemCoreClock;
}

/**
 * @brief  This function returns current AHB clock (HCLK) frequency.
 * @return HCLK in Hz.
 */
uint32_t rccGetHCLK(void)
{
    uint32_t hclk = 0;

    if ((RCC->CFGR & RCC_CFGR_HPRE) != 0)
    {
        hclk = SystemCoreClock >> ((RCC->CFGR & RCC_CFGR_HPRE) >> 4);
    }
    else
    {
        hclk = SystemCoreClock;
    }

    return hclk;
}

/**
 * @brief  This function returns current APB1 peripheral clock (PCLK1) frequency.
 * @return PCLK1 in Hz.
 */
uint32_t rccGetPCLK1(void)
{
    uint32_t pclk1 = rccGetHCLK();

    if (((RCC->CFGR & RCC_CFGR_PPRE1) >> 10) != 0)
    {
        pclk1 = pclk1 >> (((RCC->CFGR & RCC_CFGR_PPRE1) >> 10) - 1);
    }

    return pclk1;
}

/**
 * @brief  This function returns current APB2 peripheral clock (PCLK2) frequency.
 * @return PCLK2 in Hz.
 */
uint32_t rccGetPCLK2(void)
{
    uint32_t pclk2 = rccGetHCLK();

    if (((RCC->CFGR & RCC_CFGR_PPRE2) >> 13) != 0)
    {
        pclk2 = pclk2 >> (((RCC->CFGR & RCC_CFGR_PPRE2) >> 13) - 1);
    }

    return pclk2;
}

/**
 * @section Private Function Definations.
 */

/**
 * @brief  This function waits for a specific flag in a register to be set within a timeout.
 * @param  ptr_reg Pointer to the register.
 * @param  flag Flag to wait for.
 * @return RCC_OK if flag is set, -1 if timeout occurs.
 */
static int waitForFlag(volatile uint32_t *ptr_reg, uint32_t flag)
{
    uint32_t timeout = TIMEOUT;

    while (((*ptr_reg & flag) == 0) && (timeout-- > 0))
    {
        __NOP();
    }

    return ((*ptr_reg & flag) != 0) ? RCC_OK : -1;
}