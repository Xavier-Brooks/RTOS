#include "main.h"

uint8_t USART_Read (USART_TypeDef * USARTx) {
// SR_RXNE (Read data register not empty) bit is set by hardware
while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
// USART resets the RXNE flag automatically after reading DR
return ((uint8_t)(USARTx->RDR & 0xFF));
// Reading USART_DR automatically clears the RXNE flag
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// A byte to be transmitted is written to the TDR (transmit data egister), and the TXE (transmit empty) bit is cleared.
	// The TDR is copied to an output shift register for serialization when that register is empty, and the TXE bit is set.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE))
;    	// wait until TXE (TX empty) bit is set
		USARTx->TDR = buffer[i] & 0xFF; // writing USART_TDR automatically clears the TXE flag
		}
	while (!(USARTx->ISR & USART_ISR_TC))
		;  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}
