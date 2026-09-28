/**
 * @file stm32f407_GPIO_driver.h
 * @brief GPIO driver header file for STM32F407x MCU.
 *        Contains user configuration structures, pin configuration macros,
 *        and GPIO driver API function prototypes.
 *
 * @date Jul 31, 2026
 * @author Bernardo
 */

#ifndef STM32F407_GPIO_DRIVER_H_
#define STM32F407_GPIO_DRIVER_H_
#include "stm32f407.h"

/**
 * @brief Configuration structure for a GPIO pin
 */
typedef struct{
    uint8_t GPIO_PinNumber;        /*!< Pin number to configure. Possible values from @GPIO_PIN_NUMBER */
    uint8_t GPIO_PinMode;          /*!< Operating mode for the selected pin. Possible values from @GPIO_PIN_MODES */
    uint8_t GPIO_PinSpeed;         /*!< Output speed for the selected pin. Possible values from @GPIO_SPEED_MODES */
    uint8_t GPIO_PinPuPdControl;   /*!< Pull-up / pull-down configuration. Possible values from @GPIO_PUPD_MODES */
    uint8_t GPIO_PinOPType;        /*!< Output type configuration. Possible values from @GPIO_OPTYPE_MODES */
    uint8_t GPIO_PinAltFunMode;    /*!< Alternate function mode. Possible values from 0 to 15 */
}GPIO_PinConfig_t;

/**
 * @brief Handle structure for a GPIO pin
 */
typedef struct{
    GPIO_RegDef_t *pGPIOx;          /*!< Base address of the GPIO port to which the pin belongs */
    GPIO_PinConfig_t GPIO_PinConfig;/*!< GPIO pin configuration settings */
}GPIO_Handle_t;

/**
 * @defgroup GPIO_PIN_NUMBER GPIO Pin Numbers
 * @{
 */
#define GPIO_PIN_NUM_0  0
#define GPIO_PIN_NUM_1  1
#define GPIO_PIN_NUM_2  2
#define GPIO_PIN_NUM_3  3
#define GPIO_PIN_NUM_4  4
#define GPIO_PIN_NUM_5  5
#define GPIO_PIN_NUM_6  6
#define GPIO_PIN_NUM_7  7
#define GPIO_PIN_NUM_8  8
#define GPIO_PIN_NUM_9  9
#define GPIO_PIN_NUM_10 10
#define GPIO_PIN_NUM_11 11
#define GPIO_PIN_NUM_12 12
#define GPIO_PIN_NUM_13 13
#define GPIO_PIN_NUM_14 14
#define GPIO_PIN_NUM_15 15
/** @} */

/**
 * @defgroup GPIO_PIN_MODES GPIO Pin Possible Modes
 * @{
 */
#define GPIO_MODE_INP       0   /*!< Input mode */
#define GPIO_MODE_OUT       1   /*!< General purpose output mode */
#define GPIO_MODE_ALTFN     2   /*!< Alternate function mode */
#define GPIO_MODE_ANALOG    3   /*!< Analog mode */
#define GPIO_MODE_IT_FT     4   /*!< Interrupt with falling edge trigger */
#define GPIO_MODE_IT_RT     5   /*!< Interrupt with rising edge trigger */
#define GPIO_MODE_IT_RFT    6   /*!< Interrupt with rising and falling edge trigger */
/** @} */

/**
 * @defgroup GPIO_OPTYPE_MODES GPIO Pin Possible Output Types
 * @{
 */
#define GPIO_OP_TYPE_PP     0   /*!< Output push-pull */
#define GPIO_OP_TYPE_OD     1   /*!< Output open-drain */
/** @} */

/**
 * @defgroup GPIO_SPEED_MODES GPIO Pin Possible Output Speeds
 * @{
 */
#define GPIO_SPEED_LOW      0   /*!< Low speed */
#define GPIO_SPEED_MEDIUM   1   /*!< Medium speed */
#define GPIO_SPEED_FAST     2   /*!< Fast speed */
#define GPIO_SPEED_HIGH     3   /*!< High speed */
/** @} */

/**
 * @defgroup GPIO_PUPD_MODES GPIO Pin Pull-up and Pull-down Configuration Macros
 * @{
 */
#define GPIO_NO_PUPD    0       /*!< No pull-up, no pull-down */
#define GPIO_PU         1       /*!< Pull-up */
#define GPIO_PD         2       /*!< Pull-down */
/** @} */

/******************************************************************************************
 *                           APIs supported by this driver
 *             For more information about the APIs, check the function definitions
 ******************************************************************************************/

/*
 * Peripheral Clock setup
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/*
 * Data Read and Write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint8_t Value);

/*
 * Output Pin/Port Toggling
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
void GPIO_ToggleOutputPort(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ Configuration and ISR handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,  uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* STM32F407_GPIO_DRIVER_H_ */
