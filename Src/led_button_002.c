
#include "stm32f407.h"
#include "stm32f407_GPIO_driver.h"

#define HIGH 1
#define BTN_PRESSED HIGH

void delay(){
	for (uint32_t i = 0; i < 500000/2; i++);
		
	
	
}

int main(){

GPIO_Handle_t GPIO_Led1, GPIO_Button;
GPIO_Led1.pGPIOx = GPIOD;
GPIO_Led1.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_12;
GPIO_Led1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
GPIO_Led1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
GPIO_Led1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
GPIO_Led1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
GPIO_PeriClockControl(&GPIO_Led1,ENABLE);
GPIO_Init(&GPIO_Led1);

// GPIO_Button config
GPIO_Button.pGPIOx = GPIOA;
GPIO_Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_0;
GPIO_Button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INP;
GPIO_Button.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
//GPIO_Button.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; Aplicable just if is a output mode
GPIO_Button.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
GPIO_PeriClockControl(&GPIO_Button,ENABLE);
GPIO_Init(&GPIO_Button);

while (1)
{
    if(GPIO_ReadFromInputPin(&GPIO_Button,GPIO_PIN_NUM_0) == BTN_PRESSED)
    delay(); // to work properly
	GPIO_ToggleOutputPort(GPIOD,GPIO_PIN_NUM_12);
}



	return 0;
}
