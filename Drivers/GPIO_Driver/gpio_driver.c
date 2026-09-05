/**
 * @file    gpio_driver.c
 * @author  Pratik Dhulubulu
 * @brief   This file implements general purpose input output configuration,
 *          read, write, toggle pin state and the pin lock configuration.
 */
 
#include "gpio_driver.h"

/**
 * @section Private Function Declaration.
 */
static void gpioEnableClock(GPIO_TypeDef *ptr_port);

/**
 * @section Public Function Definations.
 */

/**
 * @brief  This fuction initializes a GPIO pin as per configuration structure.
 * @param  ptr_cfg Pointer to GPIO_CFG structure.
 * @return None.
 */
void gpioInit(const GPIO_CFG *ptr_cfg)
{
    uint32_t pin = (uint32_t)ptr_cfg->pin;

    gpioEnableClock(ptr_cfg->ptr_port);

    ptr_cfg->ptr_port->MODER &= ~(3UL << (pin * 2UL));
    ptr_cfg->ptr_port->MODER |= ((uint32_t)ptr_cfg->mode << (pin * 2UL));

    ptr_cfg->ptr_port->OTYPER &= ~(1UL << pin);
    ptr_cfg->ptr_port->OTYPER |= ((uint32_t)ptr_cfg->otype << pin);

    ptr_cfg->ptr_port->OSPEEDR &= ~(3UL << (pin * 2UL));
    ptr_cfg->ptr_port->OSPEEDR |= ((uint32_t)ptr_cfg->speed << (pin * 2UL));

    ptr_cfg->ptr_port->PUPDR &= ~(3UL << (pin * 2UL));
    ptr_cfg->ptr_port->PUPDR |= ((uint32_t)ptr_cfg->pupd << (pin * 2UL));

    if (ptr_cfg->mode == GPIO_MODE_ALT)
    {
        uint32_t index = pin / 8UL;
        uint32_t pos = (pin % 8UL) * 4UL;

        ptr_cfg->ptr_port->AFR[index] &= ~(0xFUL << pos);
        ptr_cfg->ptr_port->AFR[index] |= ((uint32_t)ptr_cfg->alt_func << pos);
    }
}

/**
 * @brief  This fuction writes logic level to a GPIO pin using atomic BSRR register.
 * @param  ptr_port Pointer to GPIO port.
 * @param  pin Pin number.
 * @param  value Logic state (0 or 1).
 * @return None.
 */
void gpioWritePin(GPIO_TypeDef *ptr_port, GPIO_PIN pin, uint8_t value)
{
    if (value != 0U)
    {
        ptr_port->BSRR = (1UL << pin);
    }
    else
    {
        ptr_port->BSRR = (1UL << (pin + 16UL));
    }
}

/**
 * @brief  This fuction toggles the output state of a GPIO pin.
 * @param  ptr_port Pointer to GPIO port.
 * @param  pin Pin number.
 * @return None.
 */
void gpioTogglePin(GPIO_TypeDef *ptr_port, GPIO_PIN pin)
{
    ptr_port->ODR ^= (1UL << pin);
}

/**
 * @brief  This fuction reads the input logic level of a GPIO pin.
 * @param  ptr_port Pointer to GPIO port.
 * @param  pin Pin number.
 * @return Current pin state (0 or 1).
 */
uint8_t gpioReadPin(GPIO_TypeDef *ptr_port, GPIO_PIN pin)
{
    return (uint8_t)((ptr_port->IDR >> pin) & 1UL);
}

/**
 * @brief   This fuction locks the configuration of a GPIO pin.
 * @details Once locked, configuration cannot be modified until next reset.
 * @param   ptr_port Pointer to GPIO port.
 * @param   pin Pin number.
 * @return  None.
 */
void gpioLockPin(GPIO_TypeDef *ptr_port, GPIO_PIN pin)
{
    uint32_t temp;

    ptr_port->LCKR = (1UL << pin);
    ptr_port->LCKR |= (1UL << 16UL);
    temp = ptr_port->LCKR;
    temp = ptr_port->LCKR;
    (void)temp;
}

/**
 * @section Private Function Definations.
 */

/**
 * @brief  This fuction enables the AHB1 peripheral clock for requested port.
 * @param  ptr_port Pointer to GPIO port base address.
 * @return None.
 */
static void gpioEnableClock(GPIO_TypeDef *ptr_port)
{
    if (ptr_port == GPIOA)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    }
    else if (ptr_port == GPIOB)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    }
    else if (ptr_port == GPIOC)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    }
    else if (ptr_port == GPIOD)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    }
    else if (ptr_port == GPIOE)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    }
}
