
#include "stm32f407.h"
#include "stm32f407_GPIO_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void delay(){
	for (uint32_t i = 0; i < 500000; i++);
}

int main(){
GPIO_Handle_t Pin_Led, Pin_Button;
memset(&Pin_Led,0,sizeof(GPIO_Handle_t));
memset(&Pin_Button,0,sizeof(GPIO_Handle_t));

//Config of Pin_Led

Pin_Led.pGPIOx = GPIOD;
Pin_Led.GPIO_PinConfig.GPIO_PinNumber  = GPIO_PIN_NUM_12;
Pin_Led.GPIO_PinConfig.GPIO_PinMode  = GPIO_MODE_OUT;
Pin_Led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
Pin_Led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
Pin_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

GPIO_PeriClockControl(GPIOD,ENABLE);
GPIO_Init(&Pin_Led);

// Config of Pin_Button
Pin_Button.pGPIOx = GPIOA;
Pin_Button.GPIO_PinConfig.GPIO_PinNumber  = GPIO_PIN_NUM_0;
Pin_Button.GPIO_PinConfig.GPIO_PinMode  = GPIO_MODE_IT_RT;
Pin_Button.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
Pin_Button.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

GPIO_PeriClockControl(GPIOA,ENABLE);
GPIO_Init(&Pin_Button);

GPIO_IRQPriorityConfig(IRQ_NO_EXTI0,15);
GPIO_IRQInterruptConfig(IRQ_NO_EXTI0,ENABLE);

while (1)
{
	
	
}



	return 0;
}
void EXTI0_IRQHandler(void){
    GPIO_IRQHandling(GPIO_PIN_NUM_0);
	delay();
    GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NUM_12);
}
