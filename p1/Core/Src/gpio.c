#include "gpio.h"

//initializes PA1 for high/low gpio
void gpio_init(){
	//GPIO PA0 to TIM2_CH1
	GPIOA->MODER &= ~(0x01u);
	GPIOA->MODER |= (0x02); //PA0 alternative mode
	GPIOA->AFR[0] |= (0x01);  //alternative function 1 selected
}

int detect_edge(){
	int x = (GPIOA->IDR & (1u)) ? 1:0;
	return x;
}
