#include "stm32f407.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 --> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT FUNCTION MODE: 5
 * */

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPI_Pins = {0} ;
	SPI_Pins.pGPIOx = GPIOB;

	SPI_Pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPI_Pins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPI_Pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPI_Pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPI_Pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCLCK
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_13;
	GPIO_Init(&SPI_Pins);

	// MOSI
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_15;
	GPIO_Init(&SPI_Pins);

	// MISO
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_14;
	GPIO_Init(&SPI_Pins);

	// NSS
	// SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_12;
	// GPIO_Init(&SPI_Pins);
}

void SPI2_INITS(void)
{
	SPI_Handle_t SPI2handle  = {0};
	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8; // Generates sclk of 2mhz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN; // Software slave management enabled for nss pin

	SPI_Init(&SPI2handle);
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

	GPIO_Handle_t GPIO_LED2 = {0};
	GPIO_LED2.pGPIOx = GPIOD;
	GPIO_LED2.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_14;
	GPIO_LED2.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIO_LED2.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_LED2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	//GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&GPIO_LED2);
}
void delay()
{
	for (uint32_t i = 0; i < 500000 / 2; i++)
		;
}


char user_data[] = "hello, world!";
char received_data[40];


int main()
{
	
	uint32_t len = strlen(user_data);
	// this function is used to initialize the gpio pins to behave as SPI2 pins
	SPI2_GPIOInits();
	// this function is used to initialize the SPI2 peripheral parameters
	SPI2_INITS();

	// initiate the led and button pins
	GPIO_Pin_Inits();

	SPI_SSIConfig(SPI2, ENABLE);

	while (1)
	{

		while (!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NUM_0)); //wait til the button is pressed
		delay();
		while (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NUM_0)); // wait til the button is released
		delay(); // Debounce
		// enable spi2 peripheral
		SPI_Peripheral_control(SPI2, ENABLE);
		for (volatile uint32_t i =0; i< len; i++){
		// send the data
		SPI_SendData(SPI2, (uint8_t *)&user_data[i], 1);

		SPI_ReceiveData(SPI2, (uint8_t *)&received_data[i], 1);
		}
		while (SPI_GetFlagStatus(SPI2, SPI_TXE_FLAG) == FLAG_RESET);
		// used to confirm that SPI is not busy
		while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));
		// disable the SPI2 peripheral
		SPI_Peripheral_control(SPI2, DISABLE);

		if (memcmp(user_data, received_data, strlen(user_data)))	//error case
		{
			for (volatile uint8_t i = 0; i < 5; i++)
			{
				GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NUM_15);
				delay();
			}
		}
		else
		{
				GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NUM_14);	//sucess case
		}
	}

		return 0;
}
