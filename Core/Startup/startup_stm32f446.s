/**
 * @file    startup_stm32f446.s
 * @author  Pratik Dhulubulu
 * @brief   Startup File for Cortex-M4 MCU.
 */

.cpu cortex-m4
.thumb

/* Vector Table */
.section .isr_vector, "a", %progbits
.align 2
.global __isr_vector
__isr_vector:
    /* Core Exception Handlers */
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler

    /* Peripheral IRQs */
    .word WWDG_IRQHandler
    .word PVD_IRQHandler
    .word TAMP_STAMP_IRQHandler
    .word RTC_WKUP_IRQHandler
    .word FLASH_IRQHandler
    .word RCC_IRQHandler
    .word EXTI0_IRQHandler
    .word EXTI1_IRQHandler
    .word EXTI2_IRQHandler
    .word EXTI3_IRQHandler
    .word EXTI4_IRQHandler
    .word DMA1_Stream0_IRQHandler
    .word DMA1_Stream1_IRQHandler
    .word DMA1_Stream2_IRQHandler
    .word DMA1_Stream3_IRQHandler
    .word DMA1_Stream4_IRQHandler
    .word DMA1_Stream5_IRQHandler
    .word DMA1_Stream6_IRQHandler
    .word ADC_IRQHandler
    .word CAN1_TX_IRQHandler
    .word CAN1_RX0_IRQHandler
    .word CAN1_RX1_IRQHandler
    .word CAN1_SCE_IRQHandler
    .word EXTI9_5_IRQHandler
    .word TIM1_BRK_TIM9_IRQHandler
    .word TIM1_UP_TIM10_IRQHandler
    .word TIM1_TRG_COM_TIM11_IRQHandler
    .word TIM1_CC_IRQHandler
    .word TIM2_IRQHandler
    .word TIM3_IRQHandler
    .word TIM4_IRQHandler
    .word I2C1_EV_IRQHandler
    .word I2C1_ER_IRQHandler
    .word I2C2_EV_IRQHandler
    .word I2C2_ER_IRQHandler
    .word SPI1_IRQHandler
    .word SPI2_IRQHandler
    .word USART1_IRQHandler
    .word USART2_IRQHandler
    .word USART3_IRQHandler
    .word EXTI15_10_IRQHandler
    .word RTC_Alarm_IRQHandler
    .word OTG_FS_WKUP_IRQHandler
    .word TIM8_BRK_TIM12_IRQHandler
    .word TIM8_UP_TIM13_IRQHandler
    .word TIM8_TRG_COM_TIM14_IRQHandler
    .word TIM8_CC_IRQHandler
    .word DMA1_Stream7_IRQHandler
    .word FSMC_IRQHandler
    .word SDIO_IRQHandler
    .word TIM5_IRQHandler
    .word SPI3_IRQHandler
    .word UART4_IRQHandler
    .word UART5_IRQHandler
    .word TIM6_DAC_IRQHandler
    .word TIM7_IRQHandler
    .word DMA2_Stream0_IRQHandler
    .word DMA2_Stream1_IRQHandler
    .word DMA2_Stream2_IRQHandler
    .word DMA2_Stream3_IRQHandler
    .word DMA2_Stream4_IRQHandler
    .word ETH_IRQHandler
    .word ETH_WKUP_IRQHandler
    .word CAN2_TX_IRQHandler
    .word CAN2_RX0_IRQHandler
    .word CAN2_RX1_IRQHandler
    .word CAN2_SCE_IRQHandler
    .word OTG_FS_IRQHandler
    .word DMA2_Stream5_IRQHandler
    .word DMA2_Stream6_IRQHandler
    .word DMA2_Stream7_IRQHandler
    .word USART6_IRQHandler
    .word I2C3_EV_IRQHandler
    .word I2C3_ER_IRQHandler
    .word OTG_HS_EP1_OUT_IRQHandler
    .word OTG_HS_EP1_IN_IRQHandler
    .word OTG_HS_WKUP_IRQHandler
    .word OTG_HS_IRQHandler
    .word DCMI_IRQHandler
    .word HASH_RNG_IRQHandler
    .word FPU_IRQHandler

/* External Symbols */
.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss
.extern SystemInit
.extern main

/* Reset Handler */
.section .text.Reset_Handler, "ax", %progbits
.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:

    /* Copy .data from Flash to RAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
1:  cmp r0, r1
    bge 2f
    ldr r3, [r2]
    str r3, [r0]
    add r0, r0, #4
    add r2, r2, #4
    b 1b
2:

    /* Clear .bss section */
    ldr r0, =_sbss
    ldr r1, =_ebss
    movs r2, #0
3:  cmp r0, r1
    bge 4f
    str r2, [r0]
    add r0, r0, #4
    b 3b
4:

    /* System init */
    bl SystemInit

    /* Call main */
    bl main

inf_loop:
    b inf_loop
.size Reset_Handler, . - Reset_Handler

/* Default Exception Handlers */
.section .text.Default_Handler, "ax", %progbits
Default_Handler:
    b Default_Handler

/* Core Handlers (weak) */
.weak NMI_Handler
.weak HardFault_Handler
.weak MemManage_Handler
.weak BusFault_Handler
.weak UsageFault_Handler
.weak SVC_Handler
.weak DebugMon_Handler
.weak PendSV_Handler
.weak SysTick_Handler

.set NMI_Handler,        Default_Handler
.set HardFault_Handler,  Default_Handler
.set MemManage_Handler,  Default_Handler
.set BusFault_Handler,   Default_Handler
.set UsageFault_Handler, Default_Handler
.set SVC_Handler,        vPortSVCHandler
.set DebugMon_Handler,   Default_Handler
.set PendSV_Handler,     xPortPendSVHandler
.set SysTick_Handler,    xPortSysTickHandler

/* Peripheral Handlers (Weak) */
.weak WWDG_IRQHandler
.weak PVD_IRQHandler
.weak TAMP_STAMP_IRQHandler
.weak RTC_WKUP_IRQHandler
.weak FLASH_IRQHandler
.weak RCC_IRQHandler
.weak EXTI0_IRQHandler
.weak EXTI1_IRQHandler
.weak EXTI2_IRQHandler
.weak EXTI3_IRQHandler
.weak EXTI4_IRQHandler
.weak DMA1_Stream0_IRQHandler
.weak DMA1_Stream1_IRQHandler
.weak DMA1_Stream2_IRQHandler
.weak DMA1_Stream3_IRQHandler
.weak DMA1_Stream4_IRQHandler
.weak DMA1_Stream5_IRQHandler
.weak DMA1_Stream6_IRQHandler
.weak ADC_IRQHandler
.weak CAN1_TX_IRQHandler
.weak CAN1_RX0_IRQHandler
.weak CAN1_RX1_IRQHandler
.weak CAN1_SCE_IRQHandler
.weak EXTI9_5_IRQHandler
.weak TIM1_BRK_TIM9_IRQHandler
.weak TIM1_UP_TIM10_IRQHandler
.weak TIM1_TRG_COM_TIM11_IRQHandler
.weak TIM1_CC_IRQHandler
.weak TIM2_IRQHandler
.weak TIM3_IRQHandler
.weak TIM4_IRQHandler
.weak I2C1_EV_IRQHandler
.weak I2C1_ER_IRQHandler
.weak I2C2_EV_IRQHandler
.weak I2C2_ER_IRQHandler
.weak SPI1_IRQHandler
.weak SPI2_IRQHandler
.weak USART1_IRQHandler
.weak USART2_IRQHandler
.weak USART3_IRQHandler
.weak EXTI15_10_IRQHandler
.weak RTC_Alarm_IRQHandler
.weak OTG_FS_WKUP_IRQHandler
.weak TIM8_BRK_TIM12_IRQHandler
.weak TIM8_UP_TIM13_IRQHandler
.weak TIM8_TRG_COM_TIM14_IRQHandler
.weak TIM8_CC_IRQHandler
.weak DMA1_Stream7_IRQHandler
.weak FSMC_IRQHandler
.weak SDIO_IRQHandler
.weak TIM5_IRQHandler
.weak SPI3_IRQHandler
.weak UART4_IRQHandler
.weak UART5_IRQHandler
.weak TIM6_DAC_IRQHandler
.weak TIM7_IRQHandler
.weak DMA2_Stream0_IRQHandler
.weak DMA2_Stream1_IRQHandler
.weak DMA2_Stream2_IRQHandler
.weak DMA2_Stream3_IRQHandler
.weak DMA2_Stream4_IRQHandler
.weak ETH_IRQHandler
.weak ETH_WKUP_IRQHandler
.weak CAN2_TX_IRQHandler
.weak CAN2_RX0_IRQHandler
.weak CAN2_RX1_IRQHandler
.weak CAN2_SCE_IRQHandler
.weak OTG_FS_IRQHandler
.weak DMA2_Stream5_IRQHandler
.weak DMA2_Stream6_IRQHandler
.weak DMA2_Stream7_IRQHandler
.weak USART6_IRQHandler
.weak I2C3_EV_IRQHandler
.weak I2C3_ER_IRQHandler
.weak OTG_HS_EP1_OUT_IRQHandler
.weak OTG_HS_EP1_IN_IRQHandler
.weak OTG_HS_WKUP_IRQHandler
.weak OTG_HS_IRQHandler
.weak DCMI_IRQHandler
.weak HASH_RNG_IRQHandler
.weak FPU_IRQHandler

/* Weak handler alias mappings */
.set WWDG_IRQHandler,           Default_Handler
.set PVD_IRQHandler,            Default_Handler
.set TAMP_STAMP_IRQHandler,     Default_Handler
.set RTC_WKUP_IRQHandler,       Default_Handler
.set FLASH_IRQHandler,          Default_Handler
.set RCC_IRQHandler,            Default_Handler
.set EXTI0_IRQHandler,          Default_Handler
.set EXTI1_IRQHandler,          Default_Handler
.set EXTI2_IRQHandler,          Default_Handler
.set EXTI3_IRQHandler,          Default_Handler
.set EXTI4_IRQHandler,          Default_Handler
.set DMA1_Stream0_IRQHandler,   Default_Handler
.set DMA1_Stream1_IRQHandler,   Default_Handler
.set DMA1_Stream2_IRQHandler,   Default_Handler
.set DMA1_Stream3_IRQHandler,   Default_Handler
.set DMA1_Stream4_IRQHandler,   Default_Handler
.set DMA1_Stream5_IRQHandler,   Default_Handler
.set DMA1_Stream6_IRQHandler,   Default_Handler
.set ADC_IRQHandler,            Default_Handler
.set CAN1_TX_IRQHandler,        Default_Handler
.set CAN1_RX0_IRQHandler,       Default_Handler
.set CAN1_RX1_IRQHandler,       Default_Handler
.set CAN1_SCE_IRQHandler,       Default_Handler
.set EXTI9_5_IRQHandler,        Default_Handler
.set TIM1_BRK_TIM9_IRQHandler,  Default_Handler
.set TIM1_UP_TIM10_IRQHandler,  Default_Handler
.set TIM1_TRG_COM_TIM11_IRQHandler, Default_Handler
.set TIM1_CC_IRQHandler,        Default_Handler
.set TIM2_IRQHandler,           Default_Handler
.set TIM3_IRQHandler,           Default_Handler
.set TIM4_IRQHandler,           Default_Handler
.set I2C1_EV_IRQHandler,        Default_Handler
.set I2C1_ER_IRQHandler,        Default_Handler
.set I2C2_EV_IRQHandler,        Default_Handler
.set I2C2_ER_IRQHandler,        Default_Handler
.set SPI1_IRQHandler,           Default_Handler
.set SPI2_IRQHandler,           Default_Handler
.set USART1_IRQHandler,         Default_Handler
.set USART2_IRQHandler,         Default_Handler
.set USART3_IRQHandler,         Default_Handler
.set EXTI15_10_IRQHandler,      Default_Handler
.set RTC_Alarm_IRQHandler,      Default_Handler
.set OTG_FS_WKUP_IRQHandler,    Default_Handler
.set TIM8_BRK_TIM12_IRQHandler, Default_Handler
.set TIM8_UP_TIM13_IRQHandler,  Default_Handler
.set TIM8_TRG_COM_TIM14_IRQHandler, Default_Handler
.set TIM8_CC_IRQHandler,        Default_Handler
.set DMA1_Stream7_IRQHandler,   Default_Handler
.set FSMC_IRQHandler,           Default_Handler
.set SDIO_IRQHandler,           Default_Handler
.set TIM5_IRQHandler,           Default_Handler
.set SPI3_IRQHandler,           Default_Handler
.set UART4_IRQHandler,          Default_Handler
.set UART5_IRQHandler,          Default_Handler
.set TIM6_DAC_IRQHandler,       Default_Handler
.set TIM7_IRQHandler,           Default_Handler
.set DMA2_Stream0_IRQHandler,   Default_Handler
.set DMA2_Stream1_IRQHandler,   Default_Handler
.set DMA2_Stream2_IRQHandler,   Default_Handler
.set DMA2_Stream3_IRQHandler,   Default_Handler
.set DMA2_Stream4_IRQHandler,   Default_Handler
.set ETH_IRQHandler,            Default_Handler
.set ETH_WKUP_IRQHandler,       Default_Handler
.set CAN2_TX_IRQHandler,        Default_Handler
.set CAN2_RX0_IRQHandler,       Default_Handler
.set CAN2_RX1_IRQHandler,       Default_Handler
.set CAN2_SCE_IRQHandler,       Default_Handler
.set OTG_FS_IRQHandler,         Default_Handler
.set DMA2_Stream5_IRQHandler,   Default_Handler
.set DMA2_Stream6_IRQHandler,   Default_Handler
.set DMA2_Stream7_IRQHandler,   Default_Handler
.set USART6_IRQHandler,         Default_Handler
.set I2C3_EV_IRQHandler,        Default_Handler
.set I2C3_ER_IRQHandler,        Default_Handler
.set OTG_HS_EP1_OUT_IRQHandler, Default_Handler
.set OTG_HS_EP1_IN_IRQHandler,  Default_Handler
.set OTG_HS_WKUP_IRQHandler,    Default_Handler
.set OTG_HS_IRQHandler,         Default_Handler
.set DCMI_IRQHandler,           Default_Handler
.set HASH_RNG_IRQHandler,       Default_Handler
.set FPU_IRQHandler,            Default_Handler
