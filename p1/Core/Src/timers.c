#include "timers.h"

/*
/initializes TIM2 for PA0 to generate an interrupt every ms
*/
void timer_init(){
	//enable timer 2 module
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	TIM2->CR1 &= ~(TIM_CR1_CEN); //disables the timer for config.
	TIM2->CCER &= ~(TIM_CCER_CC1E); //disables input capture mode for config
	
	//selects active input mode
	TIM2->CCMR1 |= (TIM_CCMR1_CC1S_0);

	//no filter
	TIM2->CCMR1 &= ~TIM_CCMR1_IC1F;

	TIM2->PSC =  15;

	//no prescalar
	TIM2->CCMR1 &= ~TIM_CCMR1_IC1PSC;
	
	//select edge of active transition in ccer with cc1p/np
	TIM2->CCER |= (TIM_CCER_CC1NP|TIM_CCER_CC1P);	

	//enables capture from the counter into the capture register
	TIM2->CCER |= (TIM_CCER_CC1E);
	
	TIM2->CR1 |= (TIM_CR1_CEN); //re-enable timer
}

void disable_timer(){
	//enables the interrupts
	NVIC_DisableIRQ(TIM2_IRQn);
	TIM2->DIER &= ~(TIM_DIER_CC1IE);
	TIM2->CR1 &= ~(TIM_CR1_CEN); //re-enable timer
}
