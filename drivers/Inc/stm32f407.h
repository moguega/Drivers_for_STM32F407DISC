/**
 * @file stm32f407.h
 * @brief MCU-specific header file for STM32F407x microcontrollers.
 *        Contains register definitions, memory base addresses, peripheral
 *        base addresses, clock enable/disable macros, and IRQ definitions.
 *
 * @date Jul 20, 2026
 * @author Bernardo
 */

#include <stdint.h>
#ifndef STM32F407_H_
#define STM32F407_H_

#define __weak  __attribute__ ((weak))

/*
 * ARM Cortex-M4 Processor NVIC ISERx (Interrupt Set-Enable Register) addresses
 */
#define NVIC_ISER0              ((volatile uint32_t*)0XE000E100)
#define NVIC_ISER1              ((volatile uint32_t*)0XE000E104)
#define NVIC_ISER2              ((volatile uint32_t*)0XE000E108)
#define NVIC_ISER3              ((volatile uint32_t*)0XE000E10C)

/*
 * ARM Cortex-M4 Processor NVIC ICERx (Interrupt Clear-Enable Register) addresses
 */
#define NVIC_ICER0              ((volatile uint32_t*)0XE000E180)
#define NVIC_ICER1              ((volatile uint32_t*)0XE000E184)
#define NVIC_ICER2              ((volatile uint32_t*)0XE000E188)
#define NVIC_ICER3              ((volatile uint32_t*)0XE000E18C)

/*
 * ARM Cortex-M4 Processor Priority Register (NVIC_IPR) base address & implemented priority bits
 */
#define NVIC_PR_BASE_ADDR       ((volatile uint32_t*)0xE000E400)
#define NUM_PRI_BIT_IMPLEMENTED     4   

/*
 * Base addresses of Flash and SRAM memories
 */
#define BASE_FLASH_ADDR         0x08000000U
#define BASE_SRAM1_ADDR         0X20000000U
#define BASE_SRAM2_ADDR         0x2001C000U
#define BASE_ROM_ADDR           0x1FFF0000U

/*
 * Base addresses of Bus domains
 */
#define BASE_AHB1_ADDR          0x40020000U
#define BASE_AHB2_ADDR          0x50000000U
#define BASE_APB1_ADDR          0x40000000U
#define BASE_APB2_ADDR          0x40010000U

/*
 * Base addresses of peripherals hanging on AHB1 bus
 */
#define GPIOA_BASE_ADDR        (BASE_AHB1_ADDR + 0x000)
#define GPIOB_BASE_ADDR        (BASE_AHB1_ADDR + 0x400)
#define GPIOC_BASE_ADDR        (BASE_AHB1_ADDR + 0x800)
#define GPIOD_BASE_ADDR        (BASE_AHB1_ADDR + 0xC00)
#define GPIOE_BASE_ADDR        (BASE_AHB1_ADDR + 0x1000)
#define GPIOF_BASE_ADDR        (BASE_AHB1_ADDR + 0x1400)
#define GPIOG_BASE_ADDR        (BASE_AHB1_ADDR + 0x1800)
#define GPIOH_BASE_ADDR        (BASE_AHB1_ADDR + 0x1C00)
#define GPIOI_BASE_ADDR        (BASE_AHB1_ADDR + 0x2000)
#define GPIOJ_BASE_ADDR        (BASE_AHB1_ADDR + 0x2400)
#define GPIOK_BASE_ADDR        (BASE_AHB1_ADDR + 0x2800)
#define RCC_BASE_ADDR          (BASE_AHB1_ADDR + 0x3800)

/*
 * Base addresses of peripherals hanging on APB1 bus
 */
#define I2C1_BASE_ADDR         (BASE_APB1_ADDR + 0x5400)
#define I2C2_BASE_ADDR         (BASE_APB1_ADDR + 0x5800)
#define I2C3_BASE_ADDR         (BASE_APB1_ADDR + 0x5C00)
#define SPI2_BASE_ADDR         (BASE_APB1_ADDR + 0x3800)
#define SPI3_BASE_ADDR         (BASE_APB1_ADDR + 0x3C00)
#define USART2_BASE_ADDR       (BASE_APB1_ADDR + 0x4400)
#define USART3_BASE_ADDR       (BASE_APB1_ADDR + 0x4800)
#define UART4_BASE_ADDR        (BASE_APB1_ADDR + 0x4C00)
#define UART5_BASE_ADDR        (BASE_APB1_ADDR + 0x5000)

/*
 * Base addresses of peripherals hanging on APB2 bus
 */
#define SPI1_BASE_ADDR         (BASE_APB2_ADDR + 0x3000)
#define USART1_BASE_ADDR       (BASE_APB2_ADDR + 0x1000)
#define USART6_BASE_ADDR       (BASE_APB2_ADDR + 0x1400)
#define EXTI_BASE_ADDR         (BASE_APB2_ADDR + 0x3C00)
#define SYSCFG_BASE_ADDR       (BASE_APB2_ADDR + 0x3800)

/*
 * SPI1 peripheral register base address offsets
 */
#define SPI1_CONTROL_REGISTER1_ADDR         (SPI1_BASE_ADDR +  0x00)
#define SPI1_CONTROL_REGISTER2_ADDR         (SPI1_BASE_ADDR +  0x04)
#define SPI1_STATUS_REGISTER_ADDR           (SPI1_BASE_ADDR +  0x08)
#define SPI1_DATA_REGISTER_ADDR             (SPI1_BASE_ADDR +  0x0C)


/*
 * Peripheral register definition structure for GPIO
 */
typedef struct{
volatile uint32_t MODER;         /*!< GPIO port mode register,                     Address offset: 0x00 */
volatile uint32_t OTYPER;        /*!< GPIO port output type register,              Address offset: 0x04 */
volatile uint32_t OSPEEDR;       /*!< GPIO port output speed register,             Address offset: 0x08 */
volatile uint32_t PUPDR;         /*!< GPIO port pull-up/pull-down register,        Address offset: 0x0C */
volatile uint32_t IDR;           /*!< GPIO port input data register,               Address offset: 0x10 */
volatile uint32_t ODR;           /*!< GPIO port output data register,              Address offset: 0x14 */
volatile uint32_t BSRR;          /*!< GPIO port bit set/reset register,            Address offset: 0x18 */
volatile uint32_t LCKR;          /*!< GPIO port configuration lock register,       Address offset: 0x1C */
volatile uint32_t AFR[2];        /*!< AFR[0]: Alternate function low reg, AFR[1]: Alternate function high reg, Address offset: 0x20-0x24 */
}GPIO_RegDef_t;


/*
 * Peripheral register definition structure for RCC (Reset and Clock Control)
 */
typedef struct{
volatile uint32_t  RCC_CR;
volatile uint32_t  RCC_PLLCFGR;
volatile uint32_t  RCC_CFGR;
volatile uint32_t  RCC_CIR;
volatile uint32_t  RCC_AHB1RSTR;
volatile uint32_t  RCC_AHB2RSTR;
volatile uint32_t  RCC_AHB3RSTR;
volatile uint32_t  Reserved1;
volatile uint32_t  RCC_APB1RSTR;
volatile uint32_t  RCC_APB2RSTR;
volatile uint32_t  Reserved2;
volatile uint32_t  Reserved3;
volatile uint32_t  RCC_AHB1ENR;
volatile uint32_t  RCC_AHB2ENR;
volatile uint32_t  RCC_AHB3ENR;
volatile uint32_t  Reserved4;
volatile uint32_t  RCC_APB1ENR;
volatile uint32_t  RCC_APB2ENR;
volatile uint32_t  Reserved5;
volatile uint32_t  Reserved6;
volatile uint32_t  RCC_AHB1LPENR;
volatile uint32_t  RCC_AHB2LPENR;
volatile uint32_t  RCC_AHB3LPENR;
volatile uint32_t  Reserved7;
volatile uint32_t  RCC_APB1LPENR;
volatile uint32_t  RCC_APB2LPENR;
volatile uint32_t  Reserved8;
volatile uint32_t  Reserved9;
volatile uint32_t  RCC_BDCR;
volatile uint32_t  RCC_CSR;
volatile uint32_t  ReservedA;
volatile uint32_t  ReservedB;
volatile uint32_t  RCC_SSCGR;
volatile uint32_t  RCC_PLLI2SCFGR;
volatile uint32_t  RCC_PLLSAICFGR;
volatile uint32_t  RCC_DCKCFGR;
}RCC_RegDef_t;

/*
 * Peripheral register definition structure for EXTI (External Interrupt/Event Controller)
 */
typedef struct{
volatile uint32_t IMR;           /*!< Interrupt mask register,                     Address offset: 0x00 */
volatile uint32_t EMR;           /*!< Event mask register,                         Address offset: 0x04 */
volatile uint32_t RTSR;          /*!< Rising trigger selection register,           Address offset: 0x08 */
volatile uint32_t FTSR;          /*!< Falling trigger selection register,          Address offset: 0x0C */
volatile uint32_t SWIER;         /*!< Software interrupt event register,           Address offset: 0x10 */
volatile uint32_t PR;            /*!< Pending register,                            Address offset: 0x14 */
}EXTI_Reg_t;

/*
 * Peripheral register definition structure for SYSCFG (System Configuration Controller)
 */
typedef struct{
volatile uint32_t SYSCFG_MEMRMP;    /*!< SYSCFG memory remap register,             Address offset: 0x00 */
volatile uint32_t SYSCFG_PMC;       /*!< SYSCFG peripheral mode configuration,     Address offset: 0x04 */
volatile uint32_t SYSCFG_EXTICR[4]; /*!< SYSCFG external interrupt config reg 1-4, Address offset: 0x08-0x14 */
uint32_t RESERVED1[2];              /*!< Reserved,                                 Address offset: 0x18-0x1C */
volatile uint32_t SYSCFG_CMPCR;     /*!< Compensation cell control register,       Address offset: 0x20 */
uint32_t RESERVED2[2];              /*!< Reserved,                                 Address offset: 0x24-0x28 */
volatile uint32_t CFGR;             /*!< SYSCFG configuration register,            Address offset: 0x2C */
}SYSCFG_Reg_t;

/*
 * Peripheral register definition structure for SPI (Serial Peripheral Interface)
 */
typedef struct{
    volatile uint32_t SPI_CR1;      /*!< SPI control register 1,                   Address offset: 0x00 */
    volatile uint32_t SPI_CR2;      /*!< SPI control register 2,                   Address offset: 0x04 */
    volatile uint32_t SPI_SR;       /*!< SPI status register,                      Address offset: 0x08 */
    volatile uint32_t SPI_DR;       /*!< SPI data register,                        Address offset: 0x0C */
    volatile uint32_t SPI_CRCPR;    /*!< SPI CRC polynomial register,              Address offset: 0x10 */
    volatile uint32_t SPI_RXCRCR;   /*!< SPI RX CRC register,                      Address offset: 0x14 */
    volatile uint32_t SPI_TXCRCR;   /*!< SPI TX CRC register,                      Address offset: 0x18 */
    volatile uint32_t SPI_I2SCFGR;  /*!< SPI_I2S configuration register,           Address offset: 0x1C */
    volatile uint32_t SPI_I2SPR;    /*!< SPI_I2S prescaler register,               Address offset: 0x20 */
}SPI_RegDef_t;

/*
 * Peripheral definitions (Peripheral base addresses typecasted to RegDef_t)
 */
#define GPIOA                   ((GPIO_RegDef_t*)(GPIOA_BASE_ADDR))
#define GPIOB                   ((GPIO_RegDef_t*)(GPIOB_BASE_ADDR))
#define GPIOC                   ((GPIO_RegDef_t*)(GPIOC_BASE_ADDR))
#define GPIOD                   ((GPIO_RegDef_t*)(GPIOD_BASE_ADDR))
#define GPIOE                   ((GPIO_RegDef_t*)(GPIOE_BASE_ADDR))
#define GPIOF                   ((GPIO_RegDef_t*)(GPIOF_BASE_ADDR))
#define GPIOG                   ((GPIO_RegDef_t*)(GPIOG_BASE_ADDR))
#define GPIOH                   ((GPIO_RegDef_t*)(GPIOH_BASE_ADDR))
#define GPIOI                   ((GPIO_RegDef_t*)(GPIOI_BASE_ADDR))

#define RCC                     ((RCC_RegDef_t*)(RCC_BASE_ADDR))
#define SYSCFG                  ((SYSCFG_Reg_t*)(SYSCFG_BASE_ADDR))
#define EXTI                    ((EXTI_Reg_t*)(EXTI_BASE_ADDR))

#define SPI1                    ((SPI_RegDef_t*)SPI1_BASE_ADDR)
#define SPI2                    ((SPI_RegDef_t*)SPI2_BASE_ADDR)
#define SPI3                    ((SPI_RegDef_t*)SPI3_BASE_ADDR)

/*
 * Clock Enable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_EN()     (RCC->RCC_AHB1ENR |= (1<<0))
#define GPIOB_PCLK_EN()     (RCC->RCC_AHB1ENR |= (1<<1))
#define GPIOC_PCLK_EN()     (RCC->RCC_AHB1ENR |= (1<<2))
#define GPIOD_PCLK_EN()     (RCC->RCC_AHB1ENR |= (1<<3))
#define GPIOE_PCLK_EN()     (RCC->RCC_AHB1ENR |= (1<<4))
#define GPIOF_PCLK_EN()     (RCC->RCC_AHB1ENR |= (1<<5))
#define GPIOG_PCLK_EN()     (RCC->RCC_AHB1ENR |= (1<<6))
#define GPIOH_PCLK_EN()     (RCC->RCC_AHB1ENR |= (1<<7))
#define GPIOI_PCLK_EN()     (RCC->RCC_AHB1ENR |= (1<<8))

/*
 * Clock Enable Macros for I2Cx peripherals
 */
#define I2C1_PCLCK_EN()     (RCC->RCC_APB1ENR |= (1<<21))
#define I2C2_PCLCK_EN()     (RCC->RCC_APB1ENR |= (1<<22))
#define I2C3_PCLCK_EN()     (RCC->RCC_APB1ENR |= (1<<23))

/*
 * Clock Enable Macros for SPIx peripherals
 */
#define SPI1_PCLCK_EN()     (RCC->RCC_APB2ENR |= (1<<12))
#define SPI2_PCLCK_EN()     (RCC->RCC_APB1ENR |= (1<<14))
#define SPI3_PCLCK_EN()     (RCC->RCC_APB1ENR |= (1<<15))

/*
 * Clock Enable Macros for USARTx peripherals
 */
#define USART1_PCLCK_EN()   (RCC->RCC_APB2ENR |= (1<<4))
#define USART2_PCLCK_EN()   (RCC->RCC_APB1ENR |= (1<<17))
#define USART3_PCLCK_EN()   (RCC->RCC_APB1ENR |= (1<<18))
#define USART4_PCLCK_EN()   (RCC->RCC_APB1ENR |= (1<<19))
#define USART5_PCLCK_EN()   (RCC->RCC_APB1ENR |= (1<<20))
#define USART6_PCLCK_EN()   (RCC->RCC_APB2ENR |= (1<<5))

/*
 * Clock Enable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLCK_EN()   (RCC->RCC_APB2ENR |= (1<<14))

/*
 * Clock Disable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_DI()     (RCC->RCC_AHB1ENR &= ~(1<<0))
#define GPIOB_PCLK_DI()     (RCC->RCC_AHB1ENR &= ~(1<<1))
#define GPIOC_PCLK_DI()     (RCC->RCC_AHB1ENR &= ~(1<<2))
#define GPIOD_PCLK_DI()     (RCC->RCC_AHB1ENR &= ~(1<<3))
#define GPIOE_PCLK_DI()     (RCC->RCC_AHB1ENR &= ~(1<<4))
#define GPIOF_PCLK_DI()     (RCC->RCC_AHB1ENR &= ~(1<<5))
#define GPIOG_PCLK_DI()     (RCC->RCC_AHB1ENR &= ~(1<<6))
#define GPIOH_PCLK_DI()     (RCC->RCC_AHB1ENR &= ~(1<<7))
#define GPIOI_PCLK_DI()     (RCC->RCC_AHB1ENR &= ~(1<<8))

/*
 * Clock Disable Macros for I2Cx peripherals
 */
#define I2C1_PCLCK_DI()     (RCC->RCC_APB1ENR &= ~(1<<21))
#define I2C2_PCLCK_DI()     (RCC->RCC_APB1ENR &= ~(1<<22))
#define I2C3_PCLCK_DI()     (RCC->RCC_APB1ENR &= ~(1<<23))

/*
 * Clock Disable Macros for SPIx peripherals
 */
#define SPI1_PCLCK_DI()     (RCC->RCC_APB2ENR &= ~(1<<12))
#define SPI2_PCLCK_DI()     (RCC->RCC_APB1ENR &= ~(1<<14))
#define SPI3_PCLCK_DI()     (RCC->RCC_APB1ENR &= ~(1<<15))

/*
 * Clock Disable Macros for USARTx peripherals
 */
#define USART1_PCLCK_DI()   (RCC->RCC_APB2ENR &= ~(1<<4))
#define USART2_PCLCK_DI()   (RCC->RCC_APB1ENR &= ~(1<<17))
#define USART3_PCLCK_DI()   (RCC->RCC_APB1ENR &= ~(1<<18))
#define USART4_PCLCK_DI()   (RCC->RCC_APB1ENR &= ~(1<<19))
#define USART5_PCLCK_DI()   (RCC->RCC_APB1ENR &= ~(1<<20))
#define USART6_PCLCK_DI()   (RCC->RCC_APB2ENR &= ~(1<<5))

/*
 * Clock Disable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLCK_DI()   (RCC->RCC_APB2ENR &= ~(1<<14))

/*
 * IRQ (Interrupt Request) numbers of STM32F407x MCU EXTI lines
 */
#define IRQ_NO_EXTI0            6
#define IRQ_NO_EXTI1            7
#define IRQ_NO_EXTI2            8
#define IRQ_NO_EXTI3            9
#define IRQ_NO_EXTI4            10
#define IRQ_NO_EXTI9_5          23
#define IRQ_NO_EXTI15_10        40

/*
 * IRQ (Interrupt Request) numbers of STM32F407x MCU SPI peripherals
 */
#define IRQ_SPI1                35
#define IRQ_SPI2                36
#define IRQ_SPI3                51
#define IRQ_SPI4                84
#define IRQ_SPI5                85
#define IRQ_SPI6                86

/*
 * NVIC Interrupt Priority levels
 */
#define NVIC_IRQ_PRIORITY_0     1
#define NVIC_IRQ_PRIORITY_2     2
#define NVIC_IRQ_PRIORITY_3     3
#define NVIC_IRQ_PRIORITY_4     4
#define NVIC_IRQ_PRIORITY_5     5
#define NVIC_IRQ_PRIORITY_6     6
#define NVIC_IRQ_PRIORITY_7     7
#define NVIC_IRQ_PRIORITY_8     8
#define NVIC_IRQ_PRIORITY_9     9
#define NVIC_IRQ_PRIORITY_10    10
#define NVIC_IRQ_PRIORITY_11    11
#define NVIC_IRQ_PRIORITY_12    12
#define NVIC_IRQ_PRIORITY_13    13
#define NVIC_IRQ_PRIORITY_14    14
#define NVIC_IRQ_PRIORITY_15    15

/*
 * Macros to reset GPIOx peripherals using RCC AHB1 peripheral reset register
 * Note: do-while(0) is used to safely encapsulate multiple C statements within a single macro
 */
#define GPIOA_REG_RESET()   do{(RCC->RCC_AHB1RSTR |= (1<<0)); (RCC->RCC_AHB1RSTR &= ~(1<<0));}while (0)
#define GPIOB_REG_RESET()   do{(RCC->RCC_AHB1RSTR |= (1<<1)); (RCC->RCC_AHB1RSTR &= ~(1<<1));}while (0)
#define GPIOC_REG_RESET()   do{(RCC->RCC_AHB1RSTR |= (1<<2)); (RCC->RCC_AHB1RSTR &= ~(1<<2));}while (0)
#define GPIOD_REG_RESET()   do{(RCC->RCC_AHB1RSTR |= (1<<3)); (RCC->RCC_AHB1RSTR &= ~(1<<3));}while (0)
#define GPIOE_REG_RESET()   do{(RCC->RCC_AHB1RSTR |= (1<<4)); (RCC->RCC_AHB1RSTR &= ~(1<<4));}while (0)
#define GPIOF_REG_RESET()   do{(RCC->RCC_AHB1RSTR |= (1<<5)); (RCC->RCC_AHB1RSTR &= ~(1<<5));}while (0)
#define GPIOG_REG_RESET()   do{(RCC->RCC_AHB1RSTR |= (1<<6)); (RCC->RCC_AHB1RSTR &= ~(1<<6));}while (0)
#define GPIOH_REG_RESET()   do{(RCC->RCC_AHB1RSTR |= (1<<7)); (RCC->RCC_AHB1RSTR &= ~(1<<7));}while (0)
#define GPIOI_REG_RESET()   do{(RCC->RCC_AHB1RSTR |= (1<<8)); (RCC->RCC_AHB1RSTR &= ~(1<<8));}while (0)
#define GPIOJ_REG_RESET()   do{(RCC->RCC_AHB1RSTR |= (1<<9)); (RCC->RCC_AHB1RSTR &= ~(1<<9));}while (0)

/*
 * Macro to map a GPIO port base address to a numeric code (0 to 7)
 * Used to configure the SYSCFG external interrupt configuration register (SYSCFG_EXTICR)
 */
#define GPIOB_BASE_ADDR_TO_CODE(x) ((x == GPIOA) ? 0 :\
                                    (x == GPIOB) ? 1 :\
                                    (x == GPIOC) ? 2 :\
                                    (x == GPIOD) ? 3 :\
                                    (x == GPIOE) ? 4 :\
                                    (x == GPIOF) ? 5 :\
                                    (x == GPIOG) ? 6 :\
                                    (x == GPIOH) ? 7 :0)

/*
 * Generic driver macros
 */
#define ENABLE              1
#define DISABLE             0

#define SET                 ENABLE
#define RESET               DISABLE

#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET

#define FLAG_RESET          RESET
#define FLAG_SET            SET

/*
 * Bit position definitions for SPI Control Register 1 (SPI_CR1)
 */
#define SPI_CR1_CPHA            0
#define SPI_CR1_CPOL            1
#define SPI_CR1_MSTR            2
#define SPI_CR1_SCLK_SPEED      3
#define SPI_CR1_SPE             6
#define SPI_CR1_LSB_FIRST       7
#define SPI_CR1_SSI             8
#define SPI_CR1_SSM             9
#define SPI_CR1_RX_ONLY         10
#define SPI_CR1_DFF             11
#define SPI_CR1_CRC_NEXT        12
#define SPI_CR1_CRC_EN          13
#define SPI_CR1_BIDI_OE         14
#define SPI_CR1_BIDI_MODE       15

/*
 * Bit position definitions for SPI Control Register 2 (SPI_CR2)
 */
#define SPI_CR2_RXDMAEN         0
#define SPI_CR2_TXDMAEN         1
#define SPI_CR2_SSOE            2
#define SPI_CR2_FRF             4
#define SPI_CR2_ERRIE           5
#define SPI_CR2_RXNEIE          6
#define SPI_CR2_TXEIE           7

/*
 * Bit position definitions for SPI Status Register (SPI_SR)
 */
#define SPI_SR_RXNE             0
#define SPI_SR_TXE              1
#define SPI_SR_CHSIDE           2
#define SPI_SR_UDR              3
#define SPI_SR_CRC_ERR          4
#define SPI_SR_MODF             5
#define SPI_SR_OVR              6
#define SPI_SR_BSY              7
#define SPI_SR_FRE              8

/*
 * Driver header inclusions
 */
#include "stm32f407_GPIO_driver.h"
#include "SPI_Driver.h"
#endif /* STM32F407_H_ */

