#include "stm32f407.h"

/*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - Enables or disables the peripheral clock for the given GPIO port
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 * @param[in]         - EnorDi: ENABLE or DISABLE macro
 *
 * @return            - None
 *
 * @note              - None
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
        if (pGPIOx == GPIOA)
        {
            GPIOA_PCLK_EN();
        }
        else if (pGPIOx == GPIOB)
        {
            GPIOB_PCLK_EN();
        }
        else if (pGPIOx == GPIOC)
        {
            GPIOC_PCLK_EN();
        }
        else if (pGPIOx == GPIOD)
        {
            GPIOD_PCLK_EN();
        }
        else if (pGPIOx == GPIOE)
        {
            GPIOE_PCLK_EN();
        }
        else if (pGPIOx == GPIOF)
        {
            GPIOF_PCLK_EN();
        }
        else if (pGPIOx == GPIOG)
        {
            GPIOG_PCLK_EN();
        }
        else if (pGPIOx == GPIOH)
        {
            GPIOH_PCLK_EN();
        }
        else if (pGPIOx == GPIOI)
        {
            GPIOI_PCLK_EN();
        }
    }
    else
    {
        if (pGPIOx == GPIOA)
        {
            GPIOA_PCLK_DI();
        }
        else if (pGPIOx == GPIOB)
        {
            GPIOB_PCLK_DI();
        }
        else if (pGPIOx == GPIOC)
        {
            GPIOC_PCLK_DI();
        }
        else if (pGPIOx == GPIOD)
        {
            GPIOD_PCLK_DI();
        }
        else if (pGPIOx == GPIOE)
        {
            GPIOE_PCLK_DI();
        }
        else if (pGPIOx == GPIOF)
        {
            GPIOF_PCLK_DI();
        }
        else if (pGPIOx == GPIOG)
        {
            GPIOG_PCLK_DI();
        }
        else if (pGPIOx == GPIOH)
        {
            GPIOH_PCLK_DI();
        }
        else if (pGPIOx == GPIOI)
        {
            GPIOI_PCLK_DI();
        }
    }
}

/*********************************************************************
 * @fn      		  - GPIO_Init
 *
 * @brief             - Initializes the GPIO pin according to the specified parameters in the handle structure
 *
 * @param[in]         - pGPIOHandle: Pointer to the GPIO handle structure containing configuration information
 *
 * @return            - None
 *
 * @note              - Enables the peripheral clock for the given port automatically
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{

    // Enable the peripheral clock
    GPIO_PeriClockControl(pGPIOHandle->pGPIOx,ENABLE);

    // 1. Configure the mode of the GPIO pin
    uint32_t temp = 0; // Temporary register
    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        // Non-interrupt mode
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); // Clearing
        pGPIOHandle->pGPIOx->MODER |= temp;                                                 // Setting
    }
    else
    {
        // Interrupt mode
        if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
        {
            // Configure the FTSR (Falling Trigger Selection Register)
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            // Clear the corresponding RTSR bit
            EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }
        else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
        {
            // Configure the RTSR (Rising Trigger Selection Register)
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            // Clear the corresponding FTSR bit
            EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }
        else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
        {
            // Configure both FTSR and RTSR for falling and rising edge triggers
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            // Set the corresponding FTSR bit
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }
        // 2. Configure the GPIO port selection in SYSCFG_EXTICR
            uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber /4;
            uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber %4;
            uint8_t portcode = GPIOB_BASE_ADDR_TO_CODE(pGPIOHandle->pGPIOx);
            SYSCFG_PCLCK_EN();
            SYSCFG->SYSCFG_EXTICR[temp1] = portcode << (temp2 * 4);
        // 3. Enable the EXTI interrupt delivery using IMR
        EXTI->IMR |= 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
    }
    temp = 0;
    // 2. Configure the speed
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); // Clearing
    pGPIOHandle->pGPIOx->OSPEEDR |= temp;
    // 3. Configure the pull-up/pull-down settings
    temp = 0;
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); // Clearing
    pGPIOHandle->pGPIOx->PUPDR |= temp;
    // 4. Configure the output type
    temp = 0;
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    pGPIOHandle->pGPIOx->OTYPER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Clearing
    pGPIOHandle->pGPIOx->OTYPER |= temp;
    // 5. Configure the alternate functionality
    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
    {
        // Configure the alternate function registers
        uint32_t temp1, temp2;
        temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
        temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
        pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0x0F << (4 * temp2)); // Clear the 4 bits of AF
        pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));
    }
}

/*********************************************************************
 * @fn      		  - GPIO_DeInit
 *
 * @brief             - Resets the registers of the given GPIO port to their default reset values
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 *
 * @return            - None
 *
 * @note              - Uses RCC AHB1 peripheral reset register
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{

    if (pGPIOx == GPIOA)
    {
        GPIOA_REG_RESET();
    }
    else if (pGPIOx == GPIOB)
    {
        GPIOB_REG_RESET();
    }
    else if (pGPIOx == GPIOC)
    {
        GPIOC_REG_RESET();
    }
    else if (pGPIOx == GPIOD)
    {
        GPIOD_REG_RESET();
    }
    else if (pGPIOx == GPIOE)
    {
        GPIOE_REG_RESET();
    }
    else if (pGPIOx == GPIOF)
    {
        GPIOF_REG_RESET();
    }
    else if (pGPIOx == GPIOG)
    {
        GPIOG_REG_RESET();
    }
    else if (pGPIOx == GPIOH)
    {
        GPIOH_REG_RESET();
    }
    else if (pGPIOx == GPIOI)
    {
        GPIOI_REG_RESET();
    }
}
/*********************************************************************
 * @fn      		  - GPIO_ReadFromInputPin
 *
 * @brief             - Reads the logic level from a specific input pin
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 * @param[in]         - PinNumber: Pin number to be read (0 to 15)
 *
 * @return            - 0 or 1 representing the logic state of the pin
 *
 * @note              - None
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    uint8_t value;
    value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
    return value;
}

/*********************************************************************
 * @fn      		  - GPIO_ReadFromInputPort
 *
 * @brief             - Reads the logic levels of all 16 pins of a GPIO port at once
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 *
 * @return            - 16-bit value representing the state of all port pins
 *
 * @note              - None
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
    uint16_t value;
    value = (uint16_t)pGPIOx->IDR;
    return value;
}

/*********************************************************************
 * @fn      		  - GPIO_WriteToOutputPin
 *
 * @brief             - Writes a logic value (high or low) to a specified output pin
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 * @param[in]         - PinNumber: Number of the pin to be written (0 to 15)
 * @param[in]         - Value: Value to write (GPIO_PIN_SET or GPIO_PIN_RESET)
 *
 * @return            - None
 *
 * @note              - Uses BSRR register for atomic bit set/reset operations
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
      if (Value == GPIO_PIN_SET)
    {
        pGPIOx->BSRR = (1 << PinNumber);          // Atomically set the pin
    }
    else
    {
        pGPIOx->BSRR = (1 << (PinNumber + 16));   // Atomically reset the pin
    }
}

/*********************************************************************
 * @fn      		  - GPIO_WriteToOutputPort
 *
 * @brief             - Writes a value to the entire GPIO port output data register
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 * @param[in]         - Value: Value to write to the port output data register
 *
 * @return            - None
 *
 * @note              - None
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint8_t Value)
{
    pGPIOx->ODR = Value;
}

/*********************************************************************
 * @fn      		  - GPIO_ToggleOutputPin
 *
 * @brief             - Inverts (toggles) the logic state of the specified output pin
 *
 * @param[in]         - pGPIOx: Base address of the GPIO peripheral
 * @param[in]         - PinNumber: Number of the pin to toggle (0 to 15)
 *
 * @return            - None
 *
 * @note              - None
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
    pGPIOx->ODR ^= (1 << PinNumber);
}

/*********************************************************************
 * @fn      		  - GPIO_IRQInterruptConfig
 *
 * @brief             - Enables or disables a specific interrupt in the processor NVIC
 *
 * @param[in]         - IRQNumber: Interrupt request number on the NVIC vector table
 * @param[in]         - EnorDi: ENABLE or DISABLE macro
 *
 * @return            - None
 *
 * @note              - None
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber,  uint8_t EnorDi) {
    
    if(EnorDi == ENABLE){
        if(IRQNumber <= 31){
            // Program ISER0 Register
            *NVIC_ISER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64){
            // Program ISER1 Register
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        }
         else if(IRQNumber > 64 && IRQNumber < 95){
            // Program ISER2 Register
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
        }
    } else{
        if(IRQNumber <= 31){
            // Program ICER0 Register
            *NVIC_ICER0 |= (1 << (IRQNumber));
        }
        else if(IRQNumber > 31 && IRQNumber < 64){
            // Program ICER1 Register
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96){
            // Program ICER2 Register
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
        }
    }

}

/*********************************************************************
 * @fn      		  - GPIO_IRQPriorityConfig
 *
 * @brief             - Configures the priority of a given interrupt in the processor NVIC
 *
 * @param[in]         - IRQNumber: Interrupt request number
 * @param[in]         - IRQPriority: Priority level to be configured (0 to 15)
 *
 * @return            - None
 *
 * @note              - Only upper 4 bits of each priority field are implemented in STM32F407x
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,  uint32_t IRQPriority){

    // 1. Determine the target IPR register and section
    uint8_t iprx = IRQNumber / 4; // Each IPR register holds priorities for 4 IRQs
    uint8_t iprx_section = IRQNumber % 4; // Section within the 32-bit IPR register (0, 1, 2, or 3)
    uint8_t shift_amount = (8 * iprx_section) + (8 - NUM_PRI_BIT_IMPLEMENTED);
    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

/*********************************************************************
 * @fn      		  - GPIO_IRQHandling
 *
 * @brief             - Handles an interrupt triggered by a GPIO pin by clearing the EXTI pending flag
 *
 * @param[in]         - PinNumber: Number of the pin that generated the interrupt
 *
 * @return            - None
 *
 * @note              - Must be called inside the corresponding ISR
 */
void GPIO_IRQHandling(uint8_t PinNumber) {
    // Clear the EXTI PR register bit corresponding to the pin number
    if(EXTI->PR & (1<< PinNumber)){
        // Clearing is performed by writing 1 to the pending register bit
        EXTI->PR |= (1<<PinNumber);
    }
}
