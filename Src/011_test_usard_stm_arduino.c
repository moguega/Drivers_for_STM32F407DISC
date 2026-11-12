#include "stm32f407.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SLAVE_ADDR 0x68

/* configs for USART2:
 * PB6 -> SCL
 * PB7 -> SDA
 */

USART_Handle_t USART6Handle;
uint8_t Some_Data[] = {"testte teste test teste aaa.....\n"};
char rcv_buff[6];
char data_received[6];
uint8_t msg_len;

void USART6_GPIOInits(void)
{

    GPIO_Handle_t USART_Pins;

    USART_Pins.pGPIOx = GPIOC;
    USART_Pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    USART_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_6;
    USART_Pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
    USART_Pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    USART_Pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;
    USART_Pins.GPIO_PinConfig.GPIO_PinAltFunMode = 8;
    // TX
    GPIO_Init(&USART_Pins);

    USART_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_7;
    // RX
    GPIO_Init(&USART_Pins);
}

void USART6_INITS(void)
{
    USART6Handle.pUSARTx = USART6;
    USART6Handle.USART_Config.USART_Mode = USART_MODE_TXRX;
    USART6Handle.USART_Config.Usart_Baud = USART_STD_BAUD_9600;
    USART6Handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
    USART6Handle.USART_Config.USART_WordLenght = USART_WORDLEN_8BITS;
    USART6Handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    USART6Handle.USART_Config.USART_NoOFStopBits = USART_STOPBITS_1;

    USART_Init(&USART6Handle);
}
void GPIO_Pin_Inits()
{
    GPIO_Handle_t GPIO_Button = {0};
    GPIO_Button.pGPIOx = GPIOA;
    GPIO_Button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_0;
    GPIO_Button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INP;
    GPIO_Button.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIO_Button.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_Init(&GPIO_Button);

    GPIO_Handle_t GPIO_LED = {0};
    GPIO_LED.pGPIOx = GPIOD;
    GPIO_LED.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NUM_15;
    GPIO_LED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GPIO_LED.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIO_LED.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
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
    USART6_GPIOInits();
    USART6_INITS();
    USART_Peripheral_control(USART6Handle.pUSARTx, ENABLE);
    char cmd = 'G';
    volatile uint8_t i = 0;

    while (1)
    {
        // aguarda o botão ser clicado e solto
        while (!(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NUM_0)))
            ;
        delay();
        while ((GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NUM_0)))
            ;
        delay();

        // send the data
        // 1. Envia só o comando 'G' para o Arduino
        USART_SendData(&USART6Handle, (uint8_t *)&cmd, 1);
        //USART_SendData(&USART6Handle, (uint8_t*)&Some_Data,sizeof(Some_Data));
        // 2. Recebe exatamente os 5 bytes de "HELLO"
        USART_ReceiveData(&USART6Handle, (uint8_t *)rcv_buff,5);
        while ( i < 6)
        {
            data_received[i] = rcv_buff [i];
            i++;
        }
        
        // 3. Fecha a string no final das 5 letras
        rcv_buff[5] = '\0';
        GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NUM_15);
        delay();
        GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NUM_15);
        printf("%s\n", data_received);
    }

    return 0;
}
