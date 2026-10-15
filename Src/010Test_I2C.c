#include "stm32f407.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SLAVE_ADDR 0x68

/* configs for I2C2:
 * PB6 -> SCL
 * PB7 -> SDA
 */

I2C_Handle_t I2C1Handle;
uint8_t Some_Data [] = {"testte teste test teste aaa"};

void I2C1_GPIOInits(void)
{
    GPIO_PeriClockControl(GPIOB, ENABLE);
    GPIO_Handle_t I2C_Pins;
    //SCL
    I2C_Pins.pGPIOx = GPIOB;
    I2C_Pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2C_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_6;
    I2C_Pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
    I2C_Pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    I2C_Pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;
    I2C_Pins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;

    GPIO_Init(&I2C_Pins);
    //SDA
    I2C_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_7;
    GPIO_Init(&I2C_Pins);
}

void I2C2_INITS(void)
{
    I2C1Handle.pI2Cx = I2C1;
    I2C1Handle.I2C_Config.I2C_ACKControl = ACK_ENABLE;
    I2C1Handle.I2C_Config.I2C_Device_Address = 0x61;
    I2C1Handle.I2C_Config.I2CFMDutyCycle = I2C_FM_DUTY_2;
    I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

    I2C_Init(&I2C1Handle);

}
void GPIO_Pin_Inits()
{
	GPIO_Handle_t GPIO_Button = {0};
	GPIO_Button.pGPIOx = GPIOA;
	GPIO_Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_0;
	GPIO_Button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INP;
	GPIO_Button.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Button.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&GPIO_Button);

	GPIO_Handle_t GPIO_LED = {0};
	GPIO_LED.pGPIOx = GPIOD;
	GPIO_LED.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_15;
	GPIO_LED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_LED.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_LED.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&GPIO_LED);

}

void delay()
{
    for (uint32_t i = 0; i < 500000 / 2; i++)
        ;
}

int main()
{
    GPIO_Pin_Inits();
    I2C1_GPIOInits();
    I2C2_INITS();
    I2C_Peripheral_control(I2C1Handle.pI2Cx, ENABLE);

    //send the data
    I2C_MasterSendData(&I2C1Handle,(char*)Some_Data,strlen(Some_Data),SLAVE_ADDR);
    while (1);

    return 0;
}
