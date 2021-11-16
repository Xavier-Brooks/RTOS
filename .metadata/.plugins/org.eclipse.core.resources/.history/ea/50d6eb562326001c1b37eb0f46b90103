#include "../inc/timers.h"

unsigned char cnt = 0;

/*
 * Initilaizes TIM6 to interrupt every 100ms
 * 100ms delays as defined by requiremnets each loop
 * */
void timer6_init(){
	//enable the timer 6 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN; //enable the clock for timer 6
	
	//disable the timer
	TIM6->CR1 &= ~(TIM_CR1_CEN);
	
	TIM6->PSC = 1599999; //interrupt every 100ms(10Hz)
	TIM6->ARR = 0x01; //interrupt each 10ms
	TIM6->EGR |= 1;
	
	//generate update event in software, don't stop timer in event, arr not buffered
	TIM6->CR1 &= ~(TIM_CR1_OPM|TIM_CR1_UDIS|TIM_CR1_ARPE);
	TIM6->CR1 |= (TIM_CR1_URS);
	
	NVIC_EnableIRQ(TIM6_IRQn); //enables the irq for TIM6
	TIM6->DIER |= (TIM_DIER_UIE); //enables the interrupt
	TIM6->CR1 |= (TIM_CR1_CEN); //re-enable timer
}

/*
/initializes TIM2 and TIM3 to produce PWM signals
*/
void timer_init(void){
	//enable timer 2 module
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	GPIOA->MODER &= ~(0xC0C);
	GPIOA->MODER |= (2 << 2)|(2 << 10);   // PA1 alternative mode, PA5 Alt mode
	GPIOA->AFR[0] |= (1 << 4)|(1 << 20);  //PA1:TIM2_CH2, PA5 TIM2_CH1

	//disables the timer for config, may set the UG bit
	TIM2->CR1 &= ~(TIM_CR1_CEN|TIM_CR1_URS);
	
	//step 1.
	TIM2->CCMR1 &= ~(TIM_CCMR1_CC1S|TIM_CCMR1_CC2S); //select output mode for CH1/2
	TIM2->CCER &= ~(TIM_CCER_CC1NP|TIM_CCER_CC1P|TIM_CCER_CC2NP|TIM_CCER_CC2P); //polarity set

	//step 2. active till match, then active
	TIM2->CCMR1 |= (6 << 4)|(6 << 12); //PWM active till match, then inactive
	
	//step 3
	TIM2->ARR = (SystemCoreClock/50); //period 50Hz
	TIM2->CCR1 = ((SystemCoreClock)/50)/18; //50% duty cycle
	TIM2->CCR2 = ((SystemCoreClock)/50)/18; //50% duty cycle
	
	//sets output compare preload enable bitoh
	TIM2->CCMR1 |= (TIM_CCMR1_OC1PE|TIM_CCMR1_OC2PE);
	
	//enable the auto-reload of preload bit & up-counting
	TIM2->CR1 |= (TIM_CR1_ARPE|TIM_CR1_DIR);

	//enable the corresponding channel output bit in CCER
	TIM2->CCER |= (TIM_CCER_CC1E|TIM_CCER_CC2E);
	
	//force update event to store timer settings
	TIM2->EGR |= 1u;
	
	TIM2->CR1 |= (TIM_CR1_CEN); //re-enable timerbr
}

/*
 * This functions performs a delay for the user
 *
 * PARAMS: The number of 10ms delays to halt the program
 */
void delay(int time){
	while(!(TIM6->SR & TIM_SR_UIF)){continue;}
	return;
}

/*
*increments counter to determine number of 10ms period that have passed
*/
void TIM6_IRQHandler(){
	TIM6->SR &= ~(TIM_SR_UIF); //clears interrupt
	cnt++;
}

