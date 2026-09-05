/**
 * @file    gpio_driver.h
 * @author  Pratik Dhulubulu
 * @brief   GPIO Driver Interface.
 * @details This module provides general purpose input output configuration,
 *          read, write, toggle pin state and the pin lock configuration.
 */
 
#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stddef.h>
#include <stdint.h>
#include "stm32f446xx.h"

/**
* @section Public Type Declaration
*/
typedef enum
{
    GPIO_MODE_INPUT  = 0U,
    GPIO_MODE_OUTPUT = 1U,
    GPIO_MODE_ALT    = 2U,
    GPIO_MODE_ANALOG = 3U
} GPIO_MODE;

typedef enum
{
    GPIO_OTYPE_PP = 0U,
    GPIO_OTYPE_OD = 1U
} GPIO_OTYPE;

typedef enum
{
    GPIO_SPEED_LOW    = 0U,
    GPIO_SPEED_MEDIUM = 1U,
    GPIO_SPEED_FAST   = 2U,
    GPIO_SPEED_HIGH   = 3U
} GPIO_SPEED;

typedef enum
{
    GPIO_PUPD_NONE = 0U,
    GPIO_PUPD_UP   = 1U,
    GPIO_PUPD_DOWN = 2U
} GPIO_PUPD;

typedef enum
{
    PIN_0 = 0U,
    PIN_1,
    PIN_2,
    PIN_3,
    PIN_4,
    PIN_5,
    PIN_6,
    PIN_7,
    PIN_8,
    PIN_9,
    PIN_10,
    PIN_11,
    PIN_12,
    PIN_13,
    PIN_14,
    PIN_15
} GPIO_PIN;

typedef struct
{
    GPIO_TypeDef *ptr_port; /* GPIO port base address */
    GPIO_PIN      pin;
    GPIO_MODE     mode;
    GPIO_OTYPE    otype;
    GPIO_SPEED    speed;
    GPIO_PUPD     pupd;
    uint8_t       alt_func;
} GPIO_CFG;

/**
 * @section Public Function Declaration.
 */
void gpioInit(const GPIO_CFG *ptr_cfg);
void gpioWritePin(GPIO_TypeDef *ptr_port, GPIO_PIN pin, uint8_t value);
void gpioTogglePin(GPIO_TypeDef *ptr_port, GPIO_PIN pin);
uint8_t gpioReadPin(GPIO_TypeDef *ptr_port, GPIO_PIN pin);
void gpioLockPin(GPIO_TypeDef *ptr_port, GPIO_PIN pin);

#endif
