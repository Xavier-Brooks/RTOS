#include "../inc/usart.h"

void usart2_init(){
	// enable USART2 CLK
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	// enable GPIOA CLK
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED2_Msk | GPIO_OSPEEDR_OSPEED3_Msk);
	GPIOA->OSPEEDR |= GPIO_SPEED_FREQ_VERY_HIGH << GPIO_OSPEEDR_OSPEED2_Pos;
	GPIOA->OSPEEDR |= GPIO_SPEED_FREQ_VERY_HIGH << GPIO_OSPEEDR_OSPEED3_Pos;

	/* Configure the IO Output Type to Push-Pull */
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT2_Msk | GPIO_OTYPER_OT3_Msk);

	/* Set the Pull-up/down to none */
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2_Msk | GPIO_PUPDR_PUPD3_Msk);

	/* Configure Alternate functions */
	GPIOA->AFR[0] &=  ~(GPIO_AFRL_AFSEL2_Msk | GPIO_AFRL_AFSEL3_Msk);
	GPIOA->AFR[0] |=  GPIO_AF7_USART2 << GPIO_AFRL_AFSEL2_Pos;
	GPIOA->AFR[0] |=  GPIO_AF7_USART2 << GPIO_AFRL_AFSEL3_Pos;

	/* Configure IO Direction mode to Alternate */
	GPIOA->MODER &= ~(GPIO_MODER_MODE2_Msk  | GPIO_MODER_MODE3_Msk);
	GPIOA->MODER |= GPIO_MODE_AF_PP << GPIO_MODER_MODE2_Pos;
	GPIOA->MODER |= GPIO_MODE_AF_PP << GPIO_MODER_MODE3_Pos;
	
	//disable usart for configuration
	USART2->CR1 &= ~USART_CR1_UE;
	
	//8 bit word, no parity, disable interrupts, transmitter, and receiver
	USART2->CR1 &= ~(USART_CR1_M1|USART_CR1_M0|USART_CR1_PCE|USART_CR1_TE|USART_CR1_RE);
	USART2->CR1 &= ~(0x1F0u);	
	
	//one stop bit
	USART2->CR2 &= ~(3u << 12);
	
	//baud rate - 115200
	USART2->BRR = (uint32_t)(SystemCoreClock/115200);
	
  /* In asynchronous mode, the following bits must be kept cleared:
	 *  - LINEN and CLKEN bits in the USART_CR2 register
	 *  - SCEN, HDSEL and IREN  bits in the USART_CR3 register.*/
	USART2->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
	USART2->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
	
	//re-enable transmitter and receiver
	USART2->CR1 |= (USART_CR1_TE|USART_CR1_RE);
	
	//lastly re-enable usart
	USART2->CR1 |= USART_CR1_UE;
}
	
//prints a single character over serial
void usart2_putc(char ch){
		while (!(USART2->ISR & USART_ISR_TXE)){continue;}
		USART2->TDR = ch;
}

//prints an entire string to serial
void usart2_put(char* str){
	/*use putchar to print string*/
	while(*str){
		usart2_putc(*str);
		str++;
	}	
}

//gets a single character over serial
char usart2_getc(){
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USART2->ISR & USART_ISR_RXNE)){continue;}  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return (char)(USART2->RDR);
}

//wait indefinitely until user presses any key
void wait(){
	char c = 0x00;
	usart2_put("press any key to re-run the program...");
	while (c == 0x00){
		c = usart2_getc();
	}
	usart2_put("\r\n ");
}

