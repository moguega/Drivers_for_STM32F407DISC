#include "STM32F407xx_RCC_Driver.h"
uint16_t AHB_PRESCALER[8] = {2, 4, 8, 16, 64, 128, 256, 512};
uint16_t APB1_PRESCALER[4] = {2, 4, 8, 16};


uint32_t RCC_GetPLLOutputClock(void)
{
    // not implemented because we dont use PLL on this course
    return 0;
}

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

uint32_t RCC_GetPCLK2Value(void)
{
    uint32_t pclk2, SystemClk;
    uint8_t clksrc, temp, ahbp, apb2p;

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
    // to discover the apb2 prescaler
    temp = (RCC->RCC_CFGR >> 13) & 0x7;
    if (temp < 4)
    {
        apb2p = 1;
    }
    else
    {
        apb2p = APB1_PRESCALER[temp - 4];
    }

    pclk2 = (SystemClk / ahbp) / apb2p;
    return pclk2;
}