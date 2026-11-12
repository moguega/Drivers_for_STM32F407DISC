/**
 * @file I2C_Driver.h
 * @brief I2C driver header file for STM32F407x MCU.
 *        Contains user configuration structures, handle structures,
 *        configuration macros, and I2C driver API function prototypes.
 *
 * @date Oct 02, 2026
 * @author Bernardo
 */


#ifndef INC_I2C_DRIVER_H_
#define INC_I2C_DRIVER_H_
#include "stm32f407.h"
typedef struct
{
    uint32_t I2C_SCLSpeed;
    uint8_t I2C_Device_Address;
    uint8_t I2C_ACKControl;
    uint8_t I2CFMDutyCycle;
} I2C_Config_t;

typedef struct
{
    I2C_RegDef_t *pI2Cx;
    I2C_Config_t I2C_Config;
    uint8_t *pTxBuffer;
    uint8_t *pRxBuffer;
    uint32_t TxLen;
    uint32_t RxLen;
    uint8_t TxRxState;
    uint8_t DevAddr;
    uint32_t RxSize;
    uint8_t Sr;
} I2C_Handle_t;

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM 100000
#define I2C_SCL_SPEED_FM4K 400000
#define I2C_SCL_SPEED_FM2K 200000
/*
 * @I2C_ACKControl
 */
#define ACK_ENABLE 1
#define ACK_DISABLE 0

/*
 * @I2CFMDutyCycle
 */
#define I2C_FM_DUTY_2 0
#define I2C_FM_DUTY_16_9 1

/*
 * @I2C Application States
 */
#define I2C_READY 0
#define I2C_BUSY_IN_RX 1
#define I2C_BUSY_IN_TX 2

/**
 * @defgroup I2C_Related_Flags i2c Status Flags I2C STATUS REGISTER 1
 * @{
 */
#define I2C_TXE_FLAG (1 << I2C_SR1_TXE)   /*!< Transmit buffer empty flag */
#define I2C_RXNE_FLAG (1 << I2C_SR1_RXNE) /*!< Receive buffer not empty flag */
#define I2C_SB_FLAG (1 << I2C_SR1_SB)
#define I2C_ADDR_FLAG (1 << I2C_SR1_ADDR)
#define I2C_BTF_FLAG (1 << I2C_SR1_BTF)
#define I2C_STOPF_FLAG (1 << I2C_SR1_STOPF)
#define I2C_BERR_FLAG (1 << I2C_SR1_BERR)
#define I2C_ARLO_FLAG (1 << I2C_SR1_ARLO)
#define I2C_AF_FLAG (1 << I2C_SR1_AF)
#define I2C_OVR_FLAG (1 << I2C_SR1_OVR)
#define I2C_PECERR_FLAG (1 << I2C_SR1_PEC_ERR)
#define I2C_TIMEOUT_FLAG (1 << I2C_SR1_TIMEOUT)

/*
 * Some generic macros
 */

#define I2C_DISABLE_SR RESET
#define I2C_ENABLE_SR SET

/*
 * I2C Event Application Macros
 */
#define I2C_EV_TX_COMPLETED     0
#define I2C_EV_RX_COMPLETED     1
#define I2C_EV_STOP             2
#define I2C_ERROR_BERR          3
#define I2C_ERROR_ARLO          4
#define I2C_ERROR_AF            5
#define I2C_ERROR_OVR           6
#define I2C_ERROR_TIMEOUT       7
#define I2C_EV_DATA_REQ         8
#define I2C_EV_DATA_RCV         9

/*
 * Peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void I2C_Init(I2C_Handle_t *pI2Cx_handle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/*
 * Send and Receive data API
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t Slave_Address);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t Slave_Address);

/*
 * Send and Receive data using interrupts API
 */
uint8_t I2C_MasterSendData_IT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t Slave_Address, uint8_t Sr);
uint8_t I2C_MasterReceiveData_IT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t Slave_Address, uint8_t Sr);
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);

// slave programming
void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx);

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);

/*
 * Other I2C peripheral control APIs
 */
void I2C_Peripheral_control(I2C_RegDef_t *pI2Cx, uint8_t ENorDI);

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*
 * Application Callback
 */
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t APP_Event);

#endif /* INC_I2C_DRIVER_H_ */
