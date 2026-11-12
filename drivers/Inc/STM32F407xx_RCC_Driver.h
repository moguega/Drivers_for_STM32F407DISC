/**
 * @file I2C_Driver.h
 * @brief I2C driver header file for STM32F407x MCU.
 *        Contains user configuration structures, handle structures,
 *        configuration macros, and I2C driver API function prototypes.
 *
 * @date Oct 02, 2026
 * @author Bernardo
 */


#ifndef INC_RCC_DRIVER_H_
#define INC_RCC_DRIVER_H_
#include "stm32f407.h"


uint32_t RCC_GetPCLK1Value(void);
uint32_t RCC_GetPCLK2Value(void);

#endif /* INC_RCC_DRIVER_H_ */
