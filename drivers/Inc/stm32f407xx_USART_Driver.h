/*
 * stm32f407xx_USART_Driver.h
 *
 *  Created on: Oct 28, 2026
 *      Author: Bernardo
 */


#ifndef INC_STM32F407XX_USART_DRIVER_H_
#define INC_STM32F407XX_USART_DRIVER_H_
#include "stm32f407.h"

typedef struct 
{
    uint8_t USART_Mode;
    uint32_t Usart_Baud;
    uint8_t USART_NoOFStopBits;
    uint8_t USART_WordLenght;
    uint8_t USART_ParityControl;
    uint8_t USART_HWFlowControl;
} USART_Config_t;

typedef struct 
{
    USART_RegDef_t *pUSARTx;
    USART_Config_t USART_Config;
    uint8_t *pUSART_TX_BUFF;
    uint8_t *pUSART_RX_BUFF;
    uint32_t TX_Len;
    uint32_t RX_Len;
    uint8_t  Rx_Busy_State;
    uint8_t  Tx_Busy_State;
} USART_Handle_t;

/*
 *@USART_Mode
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX 0
#define USART_MODE_ONLY_RX 1
#define USART_MODE_TXRX  2

/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 					2000000
#define SUART_STD_BAUD_3M 					3000000


/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DISABLE   0

/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1     0
#define USART_STOPBITS_0_5   1
#define USART_STOPBITS_2     2
#define USART_STOPBITS_1_5   3

/*
 *@USART_HWFlowControl
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3

/*
 * TX and RX status
 */
#define USART_READY         0
#define USART_BUSY_IN_RX    1
#define USART_BUSY_IN_TX    2

/**
 * @defgroup I2C_Related_Flags USART Status Flags USART STATUS REGISTER 
 * @{
 */
#define USART_RXNE_FLAG         (1  << USART_SR_RXNE)
#define USART_TXE_FLAG          (1  << USART_SR_TXE)
#define USART_TC_FLAG           (1  << USART_SR_TC)

/*
 * Usart events callbacks macros
*/

#define USART_EVENT_TX_CMPLT   0
#define USART_EVENT_RX_CMPLT   1
#define USART_EVENT_IDLE       2
#define USART_EVENT_CTS        3
#define USART_EVENT_PE         4
#define USART_ERR_FE           5
#define USART_ERR_NE           6
#define USART_ERR_ORE          7

/*USART APIs*/

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTx_handle);
void USART_DeInit(USART_RegDef_t *pUSARTx);


void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

void USART_Peripheral_control(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

/*
 * Send and Receive data API
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/*
 * Send and Receive data using interrupts API
 */
uint8_t USART_SendData_IT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t Slave_Address, uint8_t Sr);
uint8_t USART_ReceiveData_IT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t Slave_Address, uint8_t Sr);

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t FlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName);

void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);

void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

void USART_ApplicationEventCallback(USART_Handle_t *pUSARTCHandle, uint8_t APP_Event);

#endif /* INC_STM32F407XX_USART_DRIVER_H_ */
