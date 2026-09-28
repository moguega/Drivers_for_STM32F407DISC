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

void SPI2_GPIOInits(void){
	GPIO_Handle_t SPI_Pins;
	SPI_Pins.pGPIOx = GPIOB;

	SPI_Pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPI_Pins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPI_Pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPI_Pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPI_Pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLCK
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_13;
	GPIO_Init(&SPI_Pins);

	//MOSI
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_15;
	GPIO_Init(&SPI_Pins);

	//MISO
	//SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_14;
	//GPIO_Init(&SPI_Pins);

	//NSS
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_12;
	GPIO_Init(&SPI_Pins);
}

	void SPI2_INITS(void){
		SPI_Handle_t SPI2handle;
		SPI2handle.pSPIx = SPI2;
		SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
		SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
		SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;// Generates sclk of 2mhz
		SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
		SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
		SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
		SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI; //Hardware slave management enabled for nss pin

		SPI_Init(&SPI2handle);
	}

void GPIO_Button_Inits(){
GPIO_Handle_t GPIO_Button;
GPIO_Button.pGPIOx = GPIOA;
GPIO_Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_0;
GPIO_Button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INP;
GPIO_Button.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
GPIO_Button.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
GPIO_PeriClockControl(&GPIO_Button,ENABLE);
GPIO_Init(&GPIO_Button);
}
void delay(){
	for (uint32_t i = 0; i < 500000/2; i++);	
}
int main(){
	char user_data[] = "hello, world!";
	
	//this function is used to initialize the gpio pins to behave as SPI2 pins
	SPI2_GPIOInits();
	// this function is used to initialize the SPI2 peripheral parameters
	SPI2_INITS();
	
	SPI_SSOEConfig(SPI2,ENABLE);

	while (1)
	{
		
		while (!GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NUM_0));
		delay();
		//enable spi2 peripheral
		SPI_Peripheral_control(SPI2,ENABLE);
		//firts send the length information
		uint8_t data_len = strlen(user_data);
		SPI_SendData(SPI2,&data_len,1);
		//send the data
		SPI_SendData(SPI2,(uint8_t*)user_data,strlen(user_data));
		//used to confirm that SPI is not busy
		while (SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG));
		//disable the SPI2 peripheral
		SPI_Peripheral_control(SPI2,DISABLE);
	}
	while (1)





	return 0;
}
