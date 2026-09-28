/**
 * @file SPI_Driver.h
 * @brief SPI driver header file for STM32F407x MCU.
 *        Contains user configuration structures, handle structures,
 *        configuration macros, and SPI driver API function prototypes.
 *
 * @date Aug 24, 2026
 * @author Bernardo
 */

#include <stdint.h>
#include <stdlib.h>
#ifndef INC_SPI_DRIVER_H_
#define INC_SPI_DRIVER_H_

/**
 * @brief Configuration structure for SPIx peripheral
 */
typedef struct 
{
    uint8_t SPI_DeviceMode; /*!< Master or Slave mode. Possible values from @SPI_DeviceMode */
    uint8_t SPI_BusConfig;  /*!< Bus configuration (Full-Duplex, Half-Duplex, Simplex). Possible values from @SPI_BusConfig */
    uint8_t SPI_SclkSpeed;  /*!< Serial clock baud rate prescaler. Possible values from @SPI_SclkSpeed */
    uint8_t SPI_DFF;        /*!< Data frame format (8-bit or 16-bit). Possible values from @SPI_DFF */
    uint8_t SPI_CPOL;       /*!< Clock polarity (idle low or idle high). Possible values from @SPI_CPOL */
    uint8_t SPI_CPHA;       /*!< Clock phase (first or second edge). Possible values from @SPI_CPHA */
    uint8_t SPI_SSM;        /*!< Software slave management enable/disable. Possible values from @SPI_SSM */
}SPI_Config_t;

/**
 * @brief Handle structure for SPIx peripheral
 */
typedef struct 
{
    SPI_RegDef_t *pSPIx;    /*!< Base address of the SPI peripheral */
    SPI_Config_t SPIConfig; /*!< SPI configuration settings */
    uint8_t *pTxBuffer;     /*!< Pointer to transmit data buffer */
    uint8_t *pRxBuffer;     /*!< Pointer to receive data buffer */
    uint32_t TxLen;         /*!< Transmit length in bytes */
    uint32_t RxLen;         /*!< Receive length in bytes */
    uint8_t TxState;        /*!< Transmission state (ready, busy). Possible values from @SPI_Application_States */
    uint8_t RxState;        /*!< Reception state (ready, busy). Possible values from @SPI_Application_States */
}SPI_Handle_t;

/**
 * @defgroup SPI_Application_States SPI Application States
 * @{
 */
#define SPI_READY                   0   /*!< SPI peripheral is ready for communication */
#define SPI_BUSY_IN_RX              1   /*!< SPI peripheral is busy in reception */
#define SPI_BUSY_IN_TX              2   /*!< SPI peripheral is busy in transmission */
/** @} */

/**
 * @defgroup Possible SPI Applications events
 * @{
 */
#define SPI_EVENT_TX_CMPLT          1   /*!< SPI transmission event was completed */
#define SPI_EVENT_RX_CMPLT          2   /*!< SPI reception event was completed */
#define SPI_EVENT_OVR_ERROR         3   /*!< SPI overrun error ocurred */
#define SPI_EVENT_CRC_ERROR         4   /*!< SPI crc error was ocurred */
/** @} */

/**
 * @defgroup SPI_DeviceMode SPI Device Modes
 * @{
 */
#define SPI_DEVICE_MODE_MASTER      1   /*!< Master mode */
#define SPI_DEVICE_MODE_SLAVE       0   /*!< Slave mode */
/** @} */

/**
 * @defgroup SPI_BusConfig SPI Bus Configurations
 * @{
 */
#define SPI_BUS_CONFIG_FD           1   /*!< Full-duplex */
#define SPI_BUS_CONFIG_HD           2   /*!< Half-duplex */
#define SPI_BUS_CONFIG_S_TXONLY     3   /*!< Simplex transmit-only */
#define SPI_BUS_CONFIG_S_RXONLY     4   /*!< Simplex receive-only */
/** @} */

/**
 * @defgroup SPI_SclkSpeed SPI Serial Clock Baud Rate Prescaler
 * @{
 */
#define SPI_SCLK_SPEED_DIV2         0   /*!< fPCLK / 2 */
#define SPI_SCLK_SPEED_DIV4         1   /*!< fPCLK / 4 */
#define SPI_SCLK_SPEED_DIV8         2   /*!< fPCLK / 8 */
#define SPI_SCLK_SPEED_DIV16        3   /*!< fPCLK / 16 */
#define SPI_SCLK_SPEED_DIV32        4   /*!< fPCLK / 32 */
#define SPI_SCLK_SPEED_DIV64        5   /*!< fPCLK / 64 */
#define SPI_SCLK_SPEED_DIV128       6   /*!< fPCLK / 128 */
#define SPI_SCLK_SPEED_DIV256       7   /*!< fPCLK / 256 */
/** @} */

/**
 * @defgroup SPI_DFF SPI Data Frame Format
 * @{
 */
#define SPI_DFF_8BITS               0   /*!< 8-bit data frame format */
#define SPI_DFF_16BITS              1   /*!< 16-bit data frame format */
/** @} */

/**
 * @defgroup SPI_CPOL SPI Clock Polarity
 * @{
 */
#define SPI_CPOL_HIGH               1   /*!< CK to 1 when idle */
#define SPI_CPOL_LOW                0   /*!< CK to 0 when idle */
/** @} */

/**
 * @defgroup SPI_CPHA SPI Clock Phase
 * @{
 */
#define SPI_CPHA_HIGH               1   /*!< The second clock transition is the first data capture edge */
#define SPI_CPHA_LOW                0   /*!< The first clock transition is the first data capture edge */
/** @} */

/**
 * @defgroup SPI_SSM SPI Software Slave Management
 * @{
 */
#define SPI_SSM_EN                  1   /*!< Software slave management enabled */
#define SPI_SSM_DI                  0   /*!< Software slave management disabled (Hardware NSS) */
/** @} */

/**
 * @defgroup SPI_Related_Flags SPI Status Flags
 * @{
 */
#define SPI_TXE_FLAG    (1 << SPI_SR_TXE)       /*!< Transmit buffer empty flag */
#define SPI_RXNE_FLAG   (1 << SPI_SR_RXNE)      /*!< Receive buffer not empty flag */
#define SPI_BUSY_FLAG   (1 << SPI_SR_BSY)       /*!< Busy flag */
/** @} */

/******************************************************************************************
 *                           APIs supported by this driver
 *             For more information about the APIs, check the function definitions
 ******************************************************************************************/

/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIx_handle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 * Data Send and Receive (Blocking / Polling based)
 */
uint8_t SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer,uint32_t len);
uint16_t SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer,uint32_t len);

/*
 * Data Send and Receive (Non-blocking / Interrupt based)
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len);

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber,  uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/*
 * Other SPI peripheral control APIs
 */
void SPI_Peripheral_control (SPI_RegDef_t *pSPIx, uint8_t ENorDI);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t ENorDI);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t ENorDI);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_ClearOVRflag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

/*
 * Application Callback
 */
 void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t APP_Event);

#endif /* INC_SPI_DRIVER_H_ */
