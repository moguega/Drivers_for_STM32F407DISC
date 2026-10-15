#include "stm32f407.h"

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);

/*********************************************************************
 * @fn      		  - SPI_Peripheral_control
 *
 * @brief             - Enables or disables the SPI peripheral (SPE bit in CR1)
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[in]         - ENorDI: ENABLE or DISABLE macro
 *
 * @return            - None
 *
 * @note              - None
 */
void SPI_Peripheral_control(SPI_RegDef_t *pSPIx, uint8_t ENorDI)
{
    if (ENorDI == ENABLE)
    {
        pSPIx->SPI_CR1 |= (1 << SPI_CR1_SPE);
    }
    else
    {
        pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SPE);
    }
}

/*********************************************************************
 * @fn      		  - SPI_SSIConfig
 *
 * @brief             - Configures the Internal Slave Select (SSI) bit in CR1 register
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[in]         - ENorDI: ENABLE or DISABLE macro
 *
 * @return            - None
 *
 * @note              - Relevant when SSM (Software Slave Management) is enabled
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t ENorDI)
{
    if (ENorDI == ENABLE)
    {
        pSPIx->SPI_CR1 |= (1 << SPI_CR1_SSI);
    }
    else
    {
        pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SSI);
    }
}

/*********************************************************************
 * @fn      		  - SPI_SSOEConfig
 *
 * @brief             - Enables or disables Slave Select Output Enable (SSOE) in CR2 register
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[in]         - ENorDI: ENABLE or DISABLE macro
 *
 * @return            - None
 *
 * @note              - In master mode, enabling SSOE drives the NSS pin low automatically when SPE=1
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t ENorDI)
{
    if (ENorDI == ENABLE)
    {
        pSPIx->SPI_CR2 |= (1 << SPI_CR2_SSOE);
    }
    else
    {
        pSPIx->SPI_CR2 &= ~(1 << SPI_CR2_SSOE);
    }
}

/*********************************************************************
 * @fn      		  - SPI_PeriClockControl
 *
 * @brief             - Enables or disables peripheral clock for the given SPI peripheral
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[in]         - EnorDi: ENABLE or DISABLE macro
 *
 * @return            - None
 *
 * @note              - None
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
        if (pSPIx == SPI1)
        {
            SPI1_PCLCK_EN();
        }
        else if (pSPIx == SPI2)
        {
            SPI2_PCLCK_EN();
        }
        else if (pSPIx == SPI3)
        {
            SPI3_PCLCK_EN();
        }
    }
    else
    {
        if (pSPIx == SPI1)
        {
            SPI1_PCLCK_DI();
        }
        else if (pSPIx == SPI2)
        {
            SPI2_PCLCK_DI();
        }
        else if (pSPIx == SPI3)
        {
            SPI3_PCLCK_DI();
        }
    }
}

/*********************************************************************
 * @fn      		  - SPI_Init
 *
 * @brief             - Initializes the SPI peripheral according to the specified parameters in the handle structure
 *
 * @param[in]         - pSPIx: Pointer to SPI handle structure containing configuration settings
 *
 * @return            - None
 *
 * @note              - None
 */
void SPI_Init(SPI_Handle_t *pSPIx)
{
    // 1. Configure the SPI_CR1 register
    uint32_t tempreg = 0;

    // Peripheral clock enable
    SPI_PeriClockControl(pSPIx->pSPIx, ENABLE);

    // Configure the device mode (Master/Slave)
    tempreg |= pSPIx->SPIConfig.SPI_DeviceMode << 2;

    // Configure bus configuration
    if (pSPIx->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
    {
        // Bidirectional mode should be cleared (2-line unidirectional data mode)
        tempreg &= ~(1 << 15);
    }
    else if (pSPIx->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
    {
        // Bidirectional mode should be set (1-line bidirectional data mode)
        tempreg |= (1 << 15);
    }
    else if (pSPIx->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_S_RXONLY)
    {
        // Bidirectional mode should be cleared
        tempreg &= ~(1 << 15);
        // RXONLY bit should be set for simplex receive-only
        tempreg |= (1 << 10);
    }
    else if (pSPIx->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_S_TXONLY)
    {
    }
    // Configure the SPI serial clock speed (baud rate prescaler)
    tempreg |= (pSPIx->SPIConfig.SPI_SclkSpeed << SPI_CR1_SCLK_SPEED);
    // Configure the data frame format (DFF)
    tempreg |= (pSPIx->SPIConfig.SPI_DFF << SPI_CR1_DFF);
    // Configure the clock polarity (CPOL)
    tempreg |= (pSPIx->SPIConfig.SPI_CPOL << SPI_CR1_CPOL);
    // Configure the clock phase (CPHA)
    tempreg |= (pSPIx->SPIConfig.SPI_CPHA << SPI_CR1_CPHA);
    // Configure the software slave management (SSM)
    tempreg |= (pSPIx->SPIConfig.SPI_SSM << SPI_CR1_SSM);

    pSPIx->pSPIx->SPI_CR1 = tempreg;
}

/*********************************************************************
 * @fn      		  - SPI_DeInit
 *
 * @brief             - Resets the SPI peripheral registers
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 *
 * @return            - None
 *
 * @note              - None
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
    if (pSPIx == SPI1)
    {
        SPI1_REG_RESET();
    }
    else if (pSPIx == SPI2)
    {
        SPI2_REG_RESET();
    }
    else if (pSPIx == SPI3)
    {
        SPI3_REG_RESET();
    }
}

/*********************************************************************
 * @fn      		  - SPI_GetFlagStatus
 *
 * @brief             - Checks whether a specific SPI status flag is set or reset
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[in]         - FlagName: Status flag to check (e.g., SPI_TXE_FLAG, SPI_RXNE_FLAG, SPI_BUSY_FLAG)
 *
 * @return            - FLAG_SET or FLAG_RESET
 *
 * @note              - None
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
    if (pSPIx->SPI_SR & FlagName) // Test whether the flag bit is set in the SPI status register
    {
        return FLAG_SET;
    }
    return FLAG_RESET;
}

/*********************************************************************
 * @fn      		  - SPI_SendData
 *
 * @brief             - Transmits data over SPI peripheral in blocking mode
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[in]         - pTxBuffer: Pointer to transmit data buffer
 * @param[in]         - len: Number of bytes to transmit
 *
 * @return            - 0 on completion
 *
 * @note              - Blocking (polling) API; waits until TXE is set before writing to DR
 */
uint8_t SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len)
{
    // Wait until TXE is set
    while (len > 0)
    {
        while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET)
            ;
        // Check the DFF (Data Frame Format) bit in CR1
        if (pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF))
        {
            // 16-bit DFF
            pSPIx->SPI_DR = *((uint16_t *)pTxBuffer); // Dereference and load 16-bit data into DR
            len -= 2;
            pTxBuffer += 2; // Advance buffer pointer by 2 bytes
        }
        else
        {
            // 8-bit DFF
            pSPIx->SPI_DR = *pTxBuffer; // Load 8-bit data into DR
            len--;
            pTxBuffer++; // Advance buffer pointer by 1 byte
        }
    }
    return 0;
}

/*********************************************************************
 * @fn      		  - SPI_SendDataIT
 *
 * @brief             - Transmits data over SPI in non-blocking / interrupt mode
 *
 * @param[in]         - pSPIHandle: Pointer to SPI handle structure
 * @param[in]         - pTxBuffer: Pointer to transmit data buffer
 * @param[in]         - len: Number of bytes to transmit
 *
 * @return            - Transmission state (SPI_READY, SPI_BUSY_IN_TX, or SPI_BUSY_IN_RX)
 *
 * @note              - Non-blocking API; enables TXEIE interrupt to handle transmission in ISR
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len)
{
    uint8_t state = pSPIHandle->TxState;

    if (state != SPI_BUSY_IN_TX)
    {
        // 1. Save the Tx buffer address and length in the handle structure
        pSPIHandle->pTxBuffer = pTxBuffer;
        pSPIHandle->TxLen = len;
        // 2. Mark the SPI state as busy in transmission so that
        // no other code can take over same SPI peripheral until transmission is over
        pSPIHandle->TxState = SPI_BUSY_IN_TX;

        // 3. Enable the TXEIE control bit to get the interrupt whenever TXE flag is set in SR
        pSPIHandle->pSPIx->SPI_CR2 |= (1 << SPI_CR2_TXEIE);
    }
    // 4. Data transmission will be handled by the ISR code
    return state;
}

/*********************************************************************
 * @fn      		  - SPI_ReceiveDataIT
 *
 * @brief             - Receives data over SPI in non-blocking / interrupt mode
 *
 * @param[in]         - pSPIHandle: Pointer to SPI handle structure
 * @param[in]         - pRxBuffer: Pointer to receive data buffer
 * @param[in]         - len: Number of bytes to receive
 *
 * @return            - Reception state (SPI_READY, SPI_BUSY_IN_TX, or SPI_BUSY_IN_RX)
 *
 * @note              - Non-blocking API; enables RXNEIE interrupt to handle reception in ISR
 */
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len)
{
    uint8_t state = pSPIHandle->RxState;

    if (state != SPI_BUSY_IN_RX)
    {
        // 1. Save the Rx buffer address and length in the handle structure
        pSPIHandle->pRxBuffer = pRxBuffer;
        pSPIHandle->RxLen = len;
        // 2. Mark the SPI state as busy in reception so that
        // no other code can take over same SPI peripheral until reception is over
        pSPIHandle->RxState = SPI_BUSY_IN_RX;

        // 3. Enable the RXNEIE control bit to get the interrupt whenever RXNE flag is set in SR
        pSPIHandle->pSPIx->SPI_CR2 |= (1 << SPI_CR2_RXNEIE);
    }
    // 4. Data reception will be handled by the ISR code
    return state;
}

/*********************************************************************
 * @fn      		  - SPI_IRQHandling
 *
 * @brief             - Find out wich event caused the interrupt and address it
 *
 * @param[in]         - pHandle: handle structure fot SPI peripheral
 *
 * @return            - it will handle some tx or rx events or an error
 *
 * @note              - none
 */
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
    uint8_t temp1, temp2;
    // first check the TXE
    temp1 = pHandle->pSPIx->SPI_SR & (1 << SPI_SR_TXE);
    temp2 = pHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_TXEIE);

    if (temp1 && temp2)
    {
        // handle TXE
        spi_txe_interrupt_handle(pHandle);
    }

    //check for the rxne

    temp1 = pHandle->pSPIx->SPI_SR & (1 << SPI_SR_RXNE);
    temp2 = pHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_RXNEIE);
    if(temp1 && temp2){
        spi_rxne_interrupt_handle(pHandle);
    }

    // check the overrun flag
    temp1 = pHandle->pSPIx->SPI_SR & (1 << SPI_SR_OVR);
    temp2 = pHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_ERRIE);

    if (temp1 && temp2)
    {
        spi_ovr_err_interrupt_handle(pHandle);
    }
}

/*********************************************************************
 * @fn      		  - SPI_ReceiveData
 *
 * @brief             - Receives data over SPI peripheral in blocking mode
 *
 * @param[in]         - pSPIx: Base address of the SPI peripheral
 * @param[in]         - pRxBuffer: Pointer to receive data buffer
 * @param[in]         - len: Number of bytes to receive
 *
 * @return            - 0 on completion
 *
 * @note              - Blocking (polling) API; waits until RXNE is set before reading from DR
 */
uint16_t SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len)
{
    while (len > 0)
    {
        while (SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET)
            ;
        // Check the DFF (Data Frame Format) bit in CR1
        if (pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF))
        {
            // 16-bit DFF
            *((uint16_t *)pRxBuffer) = pSPIx->SPI_DR; // Dereference and load 16-bit data from DR
            len--;
            len--;
            pRxBuffer += 2; // Advance buffer pointer by 2 bytes
        }
        else
        {
            // 8-bit DFF
            *pRxBuffer = pSPIx->SPI_DR; // Read 8-bit data from DR
            len--;
            pRxBuffer++; // Advance buffer pointer by 1 byte
        }
    }
    return 0;
}

/*********************************************************************
 * @fn      		  - SPI_IRQInterruptConfig
 *
 * @brief             - Enables or disables a specific SPI interrupt in the processor NVIC
 *
 * @param[in]         - IRQNumber: Interrupt request number on the NVIC vector table
 * @param[in]         - EnorDi: ENABLE or DISABLE macro
 *
 * @return            - None
 *
 * @note              - None
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
        if (IRQNumber <= 31)
        {
            // Program ISER0 Register
            *NVIC_ISER0 |= (1 << IRQNumber);
        }
        else if (IRQNumber > 31 && IRQNumber < 64)
        {
            // Program ISER1 Register
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        }
        else if (IRQNumber > 64 && IRQNumber < 95)
        {
            // Program ISER2 Register
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
        }
    }
    else
    {
        if (IRQNumber <= 31)
        {
            // Program ICER0 Register
            *NVIC_ICER0 |= (1 << (IRQNumber));
        }
        else if (IRQNumber > 31 && IRQNumber < 64)
        {
            // Program ICER1 Register
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        }
        else if (IRQNumber >= 64 && IRQNumber < 96)
        {
            // Program ICER2 Register
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
        }
    }
}

/*********************************************************************
 * @fn      		  - SPI_IRQPriorityConfig
 *
 * @brief             - Configures the priority of a given SPI interrupt in the processor NVIC
 *
 * @param[in]         - IRQNumber: Interrupt request number
 * @param[in]         - IRQPriority: Priority level to configure (0 to 15)
 *
 * @return            - None
 *
 * @note              - Only upper 4 bits of each priority field are implemented in STM32F407x
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    // 1. Determine the target IPR register and section
    uint8_t iprx = IRQNumber / 4;         // Each IPR register holds priorities for 4 IRQs
    uint8_t iprx_section = IRQNumber % 4; // Section within the 32-bit IPR register (0, 1, 2, or 3)
    uint8_t shift_amount = (8 * iprx_section) + (8 - NUM_PRI_BIT_IMPLEMENTED);
    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

// some helper functions implementations
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
    // Check the DFF (Data Frame Format) bit in CR1
    if (pSPIHandle->pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF))
    {
        // 16-bit DFF
        pSPIHandle->pSPIx->SPI_DR = *((uint16_t *)pSPIHandle->pTxBuffer); // Dereference and load 16-bit data into DR
        pSPIHandle->TxLen -= 2;
        pSPIHandle->pTxBuffer += 2; // Advance buffer pointer by 2 bytes
    }
    else
    {
        // 8-bit DFF
        pSPIHandle->pSPIx->SPI_DR = *pSPIHandle->pTxBuffer; // Load 8-bit data into DR
        pSPIHandle->TxLen--;
        pSPIHandle->pTxBuffer++; // Advance buffer pointer by 1 byte
    }

    // if txlen is zero, need to close the spi transmission and inform the application
    // that tx is over
    // this prevents interrupts from txe flag
    if (pSPIHandle->TxLen == 0)
    {
        SPI_CloseTransmission(pSPIHandle);
        SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
    }
}
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
    if (pSPIHandle->pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF))
    {
        // 16-bit DFF
        *((uint16_t *)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->SPI_DR; // Dereference and load 16-bit data from DR
        pSPIHandle->RxLen--;
        pSPIHandle->RxLen--;
        pSPIHandle->pRxBuffer += 2; // Advance buffer pointer by 2 bytes
    }
    else
    {
        // 8-bit DFF
        *pSPIHandle->pRxBuffer = pSPIHandle->pSPIx->SPI_DR; // Read 8-bit data from DR
        pSPIHandle->RxLen--;
        pSPIHandle->pRxBuffer++; // Advance buffer pointer by 1 byte
    }

    if (pSPIHandle->RxLen == 0)
    {
        SPI_CloseReception(pSPIHandle);
        SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
    }
}
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
    uint8_t temp;
    // 1. clear the ovr flag
    if (pSPIHandle->TxState != SPI_BUSY_IN_TX)
    {
        temp = pSPIHandle->pSPIx->SPI_DR;
        temp = pSPIHandle->pSPIx->SPI_SR;
    }
    (void)temp;
    // 2. Inform the application
    SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERROR);
}
void SPI_ClearOVRflag(SPI_RegDef_t *pSPIx)
{
  
}
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle){
    pSPIHandle->pSPIx->SPI_CR2 &= ~(1 << SPI_CR2_TXEIE);
    pSPIHandle->pTxBuffer = NULL;
    pSPIHandle->TxLen = 0;
    pSPIHandle->TxState = SPI_READY;
    SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
}
void SPI_CloseReception(SPI_Handle_t *pSPIHandle){
    pSPIHandle->pSPIx->SPI_CR2 &= ~(1 << SPI_CR2_RXNEIE);
    pSPIHandle->pRxBuffer = NULL;
    pSPIHandle->RxLen = 0;
    pSPIHandle->RxState = SPI_READY;
    SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
}


__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t APP_Event){

    //weak implementation

}
