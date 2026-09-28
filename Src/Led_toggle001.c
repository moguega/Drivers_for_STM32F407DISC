
#include "stm32f407.h"
#include "stm32f407_GPIO_driver.h"

void delay(){
	for (uint32_t i = 0; i < 500000; i++);
		
	
	
}

int main(){

GPIO_Handle_t GPIO_Led1;
GPIO_Led1.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_12;
GPIO_Led1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
GPIO_Led1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
GPIO_Led1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
GPIO_Led1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
GPIO_PeriClockControl(&GPIO_Led1,ENABLE);
GPIO_Init(&GPIO_Led1);

while (1)
{
	GPIO_ToggleOutputPort(GPIOD,GPIO_PIN_NUM_12);
	delay();
}



	return 0;
}
