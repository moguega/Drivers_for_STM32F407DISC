#include "stm32f407.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* configs for SPI2:
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 --> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT FUNCTION MODE: 5
 * configs for SPI1:
 * SPI1_MISO: PA6
 * SPI1_MOSI: PA7
 * SPI1_SCLK: PA5
 * SPI1_NSS	: PA4
 * 
 * */
SPI_Handle_t SPI2handle  = {0}; // global handle
SPI_Handle_t SPI1handle  = {0};//global handle
char user_data[] = "hello, world!";
char received_data[40];
volatile uint8_t TX_DONE = 0,RX_DONE = 0;

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPI_Pins = {0} ;
	SPI_Pins.pGPIOx = GPIOB;

	SPI_Pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPI_Pins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPI_Pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPI_Pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPI_Pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCLCK SPI2
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_13;
	GPIO_Init(&SPI_Pins);

	// MOSI SPI2
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_15;
	GPIO_Init(&SPI_Pins);

	// MISO SPI2
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_14;
	GPIO_Init(&SPI_Pins);

	// NSS SPI2
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_12;
	GPIO_Init(&SPI_Pins);

}


void SPI2_INITS(void)
{
	
	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_SLAVE;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8; // Generates sclk of 2mhz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN; // Software slave management enabled for nss pin
	SPI_Init(&SPI2handle);
	SPI_SSIConfig(SPI2,DISABLE);
}
void SPI1_GPIOInits(void)
{
	GPIO_Handle_t SPI_Pins = {0} ;
	SPI_Pins.pGPIOx = GPIOA;

	SPI_Pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPI_Pins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPI_Pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPI_Pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPI_Pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCLCK SPI1
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_5;
	GPIO_Init(&SPI_Pins);

	// MOSI SPI1
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_7;
	GPIO_Init(&SPI_Pins);

	// MISO SPI1
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_6;
	GPIO_Init(&SPI_Pins);

	// NSS SPI1
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_4;
	GPIO_Init(&SPI_Pins);

}

void SPI1_INITS(void)
{
	
	SPI1handle.pSPIx = SPI1;
	SPI1handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI1handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI1handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV128; // Generates sclk of 2mhz
	SPI1handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI1handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI1handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI1handle.SPIConfig.SPI_SSM = SPI_SSM_EN; // Software slave management enabled for nss pin
	SPI_Init(&SPI1handle);
	SPI_SSIConfig(SPI1,ENABLE);

	
	
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

}
void delay()
{
	for (uint32_t i = 0; i < 500000 / 2; i++)
		;
}






void SPI1_IRQHandler(){
	SPI_IRQHandling(&SPI1handle);
}
void SPI2_IRQHandler(){
	SPI_IRQHandling(&SPI2handle);
}

void SPI_ApplicationEventCallback(SPI_Handle_t *Spi_handle, uint8_t APP_Event){
	if(APP_Event == SPI_EVENT_TX_CMPLT){
		TX_DONE = 1;
	}
	else if (APP_Event == SPI_EVENT_RX_CMPLT){
		RX_DONE = 1;
	}
	else if (APP_Event == SPI_EVENT_OVR_ERROR){
		printf("\n overrun error has ocurred\n");
	}
}
int main()
{
	uint32_t len = sizeof(user_data);

	SPI1_INITS();
	GPIO_Pin_Inits();
	SPI1_GPIOInits();
	SPI2_INITS();
	SPI2_GPIOInits();
	SPI_IRQInterruptConfig(IRQ_SPI1,ENABLE);
	SPI_IRQInterruptConfig(IRQ_SPI2,ENABLE);
	
	while (1)
	{
		
		
		
		
		while (!GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NUM_0));
		delay();
		while (GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NUM_0));
		delay();
		//enable spi peripheral
		SPI_Peripheral_control(SPI2,ENABLE); // enable the slave first
		SPI_Peripheral_control(SPI1,ENABLE);
		printf("\ndado a ser enviado: %s\n", user_data);
		SPI_ReceiveDataIT(&SPI2handle,(uint8_t*)received_data,len);
		SPI_SendDataIT(&SPI1handle,(uint8_t*)user_data,len);
		while (!TX_DONE || !RX_DONE);
		while (SPI_GetFlagStatus(SPI1,SPI_BUSY_FLAG) == FLAG_SET);
		SPI_Peripheral_control(SPI1,DISABLE);
		SPI_Peripheral_control(SPI2,DISABLE);
		TX_DONE = 0;
		RX_DONE = 0;

		printf("\n dado recebido: %s\n", received_data);
		
		
		
	}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		

	
	


		return 0;
}
