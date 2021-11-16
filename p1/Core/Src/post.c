#include "../inc/post.h"

int post(){
	timer_init();
	uint32_t cnt = TIM2->CCR1;
	while((TIM2->CNT <= cnt + 100000) && !(TIM2->SR & TIM_SR_CC1IF)){
			continue;
	}
	if((TIM2->SR & TIM_SR_CC1IF)){
			return 1;	
	}
	return 0;
}

//stops check for 
void disable_post_timer(){
	TIM2->CR1 &= ~(TIM_CR1_CEN); //disables the timer for config.
	TIM2->CCER &= ~(TIM_CCER_CC1E); //disables input capture mode for config
}

