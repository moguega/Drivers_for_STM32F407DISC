#include "stm32f407.h"
/*********************************************************************
 * @fn      		  - I2C_Peripheral_control
 *
 * @brief             - Enables or disables the I2C peripheral (SPE bit in CR1)
 *
 * @param[in]         - pI2Cx: Base address of the I2C peripheral
 * @param[in]         - ENorDI: ENABLE or DISABLE macro
 *
 * @return            - None
 *
 * @note              - None
 */

uint32_t RCC_GetPLLOutputClock(void)
{
    // not implemented because we dont use PLL on this course
    return 0;
}
uint16_t AHB_PRESCALER[8] = {2, 4, 8, 16, 64, 128, 256, 512};
uint16_t APB1_PRESCALER[4] = {2, 4, 8, 16};

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t Slave_Address);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t Slave_Address);
static void I2C_Clear_Addr_Flag(I2C_Handle_t *pI2CHandle);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);

uint32_t RCC_GetPCLK1Value(void)
{
    uint32_t pclk1, SystemClk;
    uint8_t clksrc, temp, ahbp, apb1p;

    clksrc = (RCC->RCC_CFGR >> 2) & 0X3;

    if (clksrc == 0)
    {
        SystemClk = 16000000;
    }
    else if (clksrc == 1)
    {
        SystemClk = 8000000;
    }
    else if (clksrc == 2)
    {
        SystemClk = RCC_GetPLLOutputClock();
    }
    // to discover the ahb prescaler
    temp = (RCC->RCC_CFGR >> 4) & 0xF;
    if (temp < 8)
    {
        ahbp = 1;
    }
    else
    {
        ahbp = AHB_PRESCALER[temp - 8];
    }
    // to discover the apb1 prescaler
    temp = (RCC->RCC_CFGR >> 10) & 0x7;
    if (temp < 4)
    {
        apb1p = 1;
    }
    else
    {
        apb1p = APB1_PRESCALER[temp - 4];
    }

    pclk1 = (SystemClk / ahbp) / apb1p;
    return pclk1;
}

void I2C_Peripheral_control(I2C_RegDef_t *pI2Cx, uint8_t ENorDI)
{
    if (ENorDI == ENABLE)
    {
        pI2Cx->I2C_CR1 |= (1 << I2C_CR1_PE);
    }
    else
    {
        pI2Cx->I2C_CR1 &= ~(1 << I2C_CR1_PE);
    }
}

/*********************************************************************
 * @fn      		  - I2C_PeriClockControl
 *
 * @brief             - Enables or disables peripheral clock for the given I2C peripheral
 *
 * @param[in]         - pI2Cx: Base address of the I2C peripheral
 * @param[in]         - EnorDi: ENABLE or DISABLE macro
 *
 * @return            - None
 *
 * @note              - None
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
        if (pI2Cx == I2C1)
        {
            I2C1_PCLCK_EN();
        }
        else if (pI2Cx == I2C2)
        {
            I2C2_PCLCK_EN();
        }
        else if (pI2Cx == I2C3)
        {
            I2C3_PCLCK_EN();
        }
    }
    else
    {
        if (pI2Cx == I2C1)
        {
            I2C1_PCLCK_DI();
        }
        else if (pI2Cx == I2C2)
        {
            I2C2_PCLCK_DI();
        }
        else if (pI2Cx == I2C3)
        {
            I2C3_PCLCK_DI();
        }
    }
}

/*********************************************************************
 * @fn      		  - I2C_Init
 *
 * @brief             - Initializes the I2C peripheral according to the specified parameters in the handle structure
 *
 * @param[in]         - pI2Cx: Pointer to I2C handle structure containing configuration settings
 *
 * @return            - None
 *
 * @note              - None
 */
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
    uint32_t tempreg = 0;

    // i2c clock configuration
    I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

    tempreg |= (pI2CHandle->I2C_Config.I2C_ACKControl << 10);
    pI2CHandle->pI2Cx->I2C_CR1 = tempreg;
    // now we have to implement the FREQ positions of the ccr2 register
    //  we have to discover the clock source, the prescaler of ahb and apb1
    tempreg = 0;
    tempreg |= RCC_GetPCLK1Value() / 1000000U;
    pI2CHandle->pI2Cx->I2C_CR2 = (tempreg & 0x3F);

    // program the device own addres
    tempreg = 0;
    tempreg |= pI2CHandle->I2C_Config.I2C_Device_Address << 1;
    tempreg |= (1 << 14); // reference manual sais that bit should be set 1 by the software
    pI2CHandle->pI2Cx->I2C_OAR1 = tempreg;

    // ccr calculations
    uint16_t ccr_value = 0;
    tempreg = 0;
    if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
    {
        // mode is standard mode
        ccr_value = (RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        tempreg |= (ccr_value & 0xFFF);
    }
    else
    {
        // mode is fast mode
        tempreg |= (1 << 15);
        tempreg |= (pI2CHandle->I2C_Config.I2CFMDutyCycle << 14);
        if (pI2CHandle->I2C_Config.I2CFMDutyCycle == I2C_FM_DUTY_2)
        {
            ccr_value = (RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        }
        else
        {
            ccr_value = (RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        }
        tempreg |= (ccr_value & 0xFFF);
    }
    pI2CHandle->pI2Cx->I2C_CCR = tempreg;

    // trise configuration
    if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
    {
        // mode is standard mode
        tempreg = (RCC_GetPCLK1Value() / 1000000U) + 1;
    }
    else
    {
        // mode is fast mode
        tempreg = (((uint64_t)RCC_GetPCLK1Value() * 300U) / 1000000000U) + 1;
    }
    pI2CHandle->pI2Cx->I2C_TRISE = (tempreg & 0x3F);
}

/*********************************************************************
 * @fn      		  - I2C_DeInit
 *
 * @brief             - Resets the I2C peripheral registers
 *
 * @param[in]         - pI2Cx: Base address of the I2C peripheral
 *
 * @return            - None
 *
 * @note              - None
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
    if (pI2Cx == I2C1)
    {
        I2C1_REG_RESET();
    }
    else if (pI2Cx == I2C2)
    {
        I2C2_REG_RESET();
    }
    else if (pI2Cx == I2C3)
    {
        I2C3_REG_RESET();
    }
}

/*********************************************************************
 * @fn      		  - I2C_GetFlagStatus
 *
 * @brief             - Checks whether a specific I2C status flag is set or reset
 *
 * @param[in]         - pI2Cx: Base address of the I2C peripheral
 * @param[in]         - FlagName: Status flag to check (e.g., I2C_TXE_FLAG, I2C_RXNE_FLAG, I2C_BUSY_FLAG)
 *
 * @return            - FLAG_SET or FLAG_RESET
 *
 * @note              - None
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
    if (pI2Cx->I2C_SR1 & FlagName) // Test whether the flag bit is set in the I2C status register
    {
        return FLAG_SET;
    }
    return FLAG_RESET;
}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t Slave_Address)
{
    // 1 generate the Start condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

    // 2 confirm that start generation is completed by checking the SB flag in the SR1
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SB_FLAG))
        ;
    // 3 send the address of hte slave with r/nw bit set to w(0) (total 8 bits)
    I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, Slave_Address);

    // 4 confirm that address phase is completed by checking the addr flag in the SR1
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG))
        ;
    // 5 clear the ADDR flag according to its software sequence
    // note: until ADDR is cleared SCL wiil be stretched (pull to low)
    I2C_Clear_Addr_Flag(pI2CHandle);

    // 6. send the data until Len becomes 0
    while (Len > 0)
    {
        while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TXE_FLAG))
            ; // Wait till TXE is set
        pI2CHandle->pI2Cx->I2C_DR = *pTxBuffer;
        pTxBuffer++;
        Len--;
    }
    // 7. when len becomes zero wait for TXE = 1 and BTF = 1 before generating the stop condition
    //  note: TXE=1 , BTF=1, means that both SR and DR are empty and next transmission should begin
    //  when BTF=1 SCL will be stretched (pulled to LOW)

    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TXE_FLAG))
        ;
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_BTF_FLAG))
        ;
    I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

    // 8. generate STOP condition and master need not to wait for the completion of stop condition
    // note: generating STOP, automatically clears the btf
}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t Slave_Address)
{
    // EV5: SB = 1, cleared by reading SR1 register followed by writing DR Register
    // 1 generate start condition
    I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
    // 2 -> confirm that start generation is completed by checking the SB flag in the SR1
    // note: Until SB is cleared SCL Will be stretched (pulled to low).
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SB_FLAG))
        ;
    // 3 send the address of the slave with r/nw bit set to R(1) (total 8 bits)
    I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, Slave_Address);
    // 4 wait untill address phase is completed by checking the addr flag in the SR1
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG))
        ;

    // procedure to read only 1 byte from slave

    if (Len == 1)
    {
        // disable acking
        I2C_ManageAcking(pI2CHandle->pI2Cx, ACK_DISABLE);
        // clear the addr flag
        I2C_Clear_Addr_Flag(pI2CHandle);
        // generate STOP condition (per RM0090 Method 1)
        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
        // wait untill rxne becomes 1
        while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_RXNE_FLAG))
            ;
        // read data in to buffer
        *pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
    }
    // procedure to read data from slave when  len > 1
    if (Len > 1)
    {
        // clear the addr flag
        I2C_Clear_Addr_Flag(pI2CHandle);
        // read the data untill len becomes zero
        for (uint32_t i = Len; i > 0; i--)
        {
            // wait untill rxne becomes 1
            while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_RXNE_FLAG))
                ;
            if (i == 2)
            { // if last 2 bytes are remaining
                // clear the ack bit
                I2C_ManageAcking(pI2CHandle->pI2Cx, ACK_DISABLE);
                // generate stop condition
                I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
            }
            *pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
            pRxBuffer++;
        }
    }

    // re-enable acking
    if (pI2CHandle->I2C_Config.I2C_ACKControl == ACK_ENABLE)
    {
        I2C_ManageAcking(pI2CHandle->pI2Cx, ACK_ENABLE);
    }
}

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
    pI2Cx->I2C_CR1 |= (1 << I2C_CR1_START);
}

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t Slave_Address)
{
    Slave_Address = Slave_Address << 1; // makes space for the Read and Write bits
    Slave_Address &= ~(1);              // the lsb in R/nW bit which must be set to 0 for write -> slave_addres + Read/Write bit
    pI2Cx->I2C_DR = Slave_Address;
}

static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t Slave_Address)
{
    Slave_Address = Slave_Address << 1; // makes space for the Read and Write bits
    Slave_Address |= 1;                 // the lsb in R/nW bit which must be set to 1 for read -> slave_addres + Read/Write bit
    pI2Cx->I2C_DR = Slave_Address;
}

static void I2C_Clear_Addr_Flag(I2C_Handle_t *pI2CHandle)
{
    uint32_t dummy_read;
    // check for device mode
    if (pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL))
    {
        // device is in master mode
        if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            if (pI2CHandle->RxSize == 1)
            {
                // first disable the ack
                I2C_ManageAcking(pI2CHandle->pI2Cx, ACK_DISABLE);
            }
        }
        // clear the addr flag(read SR1, read SR2)
        dummy_read = pI2CHandle->pI2Cx->I2C_SR1;
        dummy_read = pI2CHandle->pI2Cx->I2C_SR2;
        (void)dummy_read;
    }
    else
    {
        // device is in slave mode
        // clear the addr flag(read SR1, read SR2)
        dummy_read = pI2CHandle->pI2Cx->I2C_SR1;
        dummy_read = pI2CHandle->pI2Cx->I2C_SR2;
        (void)dummy_read;
    }
}
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
    pI2Cx->I2C_CR1 |= (1 << I2C_CR1_STOP);
}

uint8_t I2C_MasterSendData_IT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t Slave_Address, uint8_t Sr)
{
    uint8_t busystate = pI2CHandle->TxRxState;
    if ((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
    {
        pI2CHandle->pTxBuffer = pTxBuffer;
        pI2CHandle->TxLen = Len;
        pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
        pI2CHandle->DevAddr = Slave_Address;
        pI2CHandle->Sr = Sr;

        I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

        pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITBUFEN);
        pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITEVTEN);
        pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITERREN);
    }
    return busystate;
}

uint8_t I2C_MasterReceiveData_IT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t Slave_Address, uint8_t Sr)
{
    uint8_t busystate = pI2CHandle->TxRxState;
    if ((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
    {
        pI2CHandle->pRxBuffer = pRxBuffer;
        pI2CHandle->RxLen = Len;
        pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
        pI2CHandle->RxSize = Len;
        pI2CHandle->DevAddr = Slave_Address;
        pI2CHandle->Sr = Sr;

        // Generate START condition
        I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

        // Enable ITBUFEN, ITEVTEN, ITERREN control bits in CR2
        pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITBUFEN);
        pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITEVTEN);
        pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITERREN);
    }
    return busystate;
}

/*********************************************************************
 * @fn      		  - I2C_IRQInterruptConfig
 *
 * @brief             - Enables or disables a specific I2C interrupt in the processor NVIC
 *
 * @param[in]         - IRQNumber: Interrupt request number on the NVIC vector table
 * @param[in]         - EnorDi: ENABLE or DISABLE macro
 *
 * @return            - None
 *
 * @note              - None
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
        else if (IRQNumber >= 64 && IRQNumber < 96)
        {
            // Program ISER2 Register
            *NVIC_ISER2 |= (1 << (IRQNumber % 32));
        }
    }
    else
    {
        if (IRQNumber <= 31)
        {
            // Program ICER0 Register
            *NVIC_ICER0 |= (1 << IRQNumber);
        }
        else if (IRQNumber > 31 && IRQNumber < 64)
        {
            // Program ICER1 Register
            *NVIC_ICER1 |= (1 << (IRQNumber % 32));
        }
        else if (IRQNumber >= 64 && IRQNumber < 96)
        {
            // Program ICER2 Register
            *NVIC_ICER2 |= (1 << (IRQNumber % 32));
        }
    }
}

/*********************************************************************
 * @fn      		  - I2C_IRQPriorityConfig
 *
 * @brief             - Configures priority of a specific interrupt in NVIC
 *
 * @param[in]         - IRQNumber: Interrupt request number
 * @param[in]         - IRQPriority: Interrupt priority
 *
 * @return            - None
 */
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;
    uint8_t shift_amount = (8 * iprx_section) + (8 - NUM_PRI_BIT_IMPLEMENTED);
    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
    if (EnorDi == ACK_ENABLE)
    {
        // ENABLE THE ACK
        pI2Cx->I2C_CR1 |= (1 << I2C_CR1_ACK);
    }
    else
    {
        // disable the ack
        pI2Cx->I2C_CR1 &= ~(1 << I2C_CR1_ACK);
    }
}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
{
    if (pI2CHandle->TxLen > 0)
    {
        // 1 load the data into DR
        pI2CHandle->pI2Cx->I2C_DR = *(pI2CHandle->pTxBuffer);
        // 2. decrement the TxLen
        pI2CHandle->TxLen--;
        // 3 increment the buffer address
        pI2CHandle->pTxBuffer++;
    }
}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{
    // we have to do the data reception
    if (pI2CHandle->RxSize == 1)
    {
        *pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
        pI2CHandle->RxLen--;
    }
    if (pI2CHandle->RxSize > 1)
    {
        if (pI2CHandle->RxLen == 2)
        {
            I2C_ManageAcking(pI2CHandle->pI2Cx, ACK_DISABLE);
        }
        *pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
        pI2CHandle->pRxBuffer++;
        pI2CHandle->RxLen--;
    }
    if (pI2CHandle->RxLen == 0)
    {
        // close the i2c data reception and notify the application
        // generate the stop condition
        if (pI2CHandle->Sr == I2C_DISABLE_SR)
            I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
        // close the I2C rx
        I2C_CloseReceiveData(pI2CHandle);
        // notify the application
        I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_COMPLETED);
    }
}

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
    pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);
    pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);

    pI2CHandle->TxRxState = I2C_READY;
    pI2CHandle->pRxBuffer = NULL;
    pI2CHandle->RxLen  = 0;
    pI2CHandle->RxSize = 0;
    if(pI2CHandle->I2C_Config.I2C_ACKControl == ACK_ENABLE)
        I2C_ManageAcking(pI2CHandle->pI2Cx, ACK_ENABLE);
}

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
    pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);
    pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);

    pI2CHandle->TxRxState = I2C_READY;
    pI2CHandle->pTxBuffer = NULL;
    pI2CHandle->TxLen = 0;
}

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{
    uint32_t temp1, temp2, temp3;
    temp1 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITEVTEN);
    temp2 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITBUFEN);

    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_SB);

    // Handle for interrupt generated by sb event
    // note: sb flag is only applicable in master mode

    if (temp1 && temp3)
    {
        // THE interrupt is generated because of SB event
        // this block will not be executed in slave mode because for slave SB is always zero
        // in this block lets execute the address phase

        if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
        {
            I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
        }
        else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
        }
    }

    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_ADDR);
    // Handle for interrupt generated by ADDR event
    // note: when master mode: Address is sent
    //      when slave mode : Addres matched with own address
    if (temp1 && temp3)
    {
        // addr Flag  is set
        I2C_Clear_Addr_Flag(pI2CHandle);
    }
    // 3. handle for interrupt generated by BTF (byte transfer finished ) event
    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_BTF);
    // Handle for interrupt generated by BTF event

    if (temp1 && temp3)
    {
        if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
        {
            // MAKE sure that TXE is also set.
            if (pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_TXE))
            {
                // BTF,TXE = 1;
                if (pI2CHandle->TxLen == 0)
                {
                    // 1 Generate the stop condition
                    if (pI2CHandle->Sr == I2C_DISABLE_SR)
                    {
                        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
                    }
                    // 2 reset all the member elements of the handle structure
                    I2C_CloseSendData(pI2CHandle);
                    // 3 notify the application about transmission complete
                    I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_COMPLETED);
                }
            }
        }
        else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
        {
            ; // Nothing to do for BTF in RX mode
        }
    }
    // 4. handle for interrupt generated by STOPF event
    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_STOPF);
    // Handle for interrupt generated by STOPF event
    // note: Stop detection flag is only appicable in slave mode. For master this flag will never be set
    if (temp1 && temp3)
    {
        // STOPF Flag  is set
        // clear the STOPF 1. read sr1 and write to cr1 (procedure to clear the flag)
        pI2CHandle->pI2Cx->I2C_CR1 |= 0x0000;

        // notify the application that stop is detected
        I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);
    }

    // 5. Handling the TXE flag
    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_TXE);
    if (temp1 && temp2 && temp3)
    {
        if (pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL))
        {
            // Master mode: TXE Flag is set
            if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
            {
                I2C_MasterHandleTXEInterrupt(pI2CHandle);
            }
        }
        else
        {
            // Slave mode TX
        }
    }

    // 6. Handling the RXNE flag
    temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_RXNE);
    if (temp1 && temp2 && temp3)
    {
        if (pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL))
        {
            // Master mode: RXNE Flag is set
            if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
            {
                I2C_MasterHandleRXNEInterrupt(pI2CHandle);
            }
        }
        else
        {
            // Slave mode RX
        }
    }
}
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
{
    /*********************************************************************
 * @fn      		  - I2C_ER_IRQHandling
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 
 *\

	

    //Know the status of  ITERREN control bit in the CR2
	
/***********************Check for Bus error************************************/
    uint32_t temp1,temp2;
    temp2 = (pI2CHandle->pI2Cx->I2C_CR2) & ( 1 << I2C_CR2_ITERREN);

	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error
		
		//Implement the code to clear the buss error flag 
		pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_BERR);
		
		//Implement the code to notify the application about the error 
	   I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/ 
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error
		
		//Implement the code to clear the arbitration lost error flag
        pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_ARLO);
		
		//Implement the code to notify the application about the error
        I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_ARLO); 
	   
	}

/***********************Check for ACK failure  error************************************/ 

	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error
		
	    //Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_AF);
		//Implement the code to notify the application about the error
        I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_AF); 
	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun
		//Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_OVR);
		//Implement the code to notify the application about the error
        I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_OVR); 
	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error
		//Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_TIMEOUT);
		//Implement the code to notify the application about the error
        I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_TIMEOUT); 
	}

}

__weak void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t APP_Event)
{
    // This is a weak implementation. The user application may override this function.
}
