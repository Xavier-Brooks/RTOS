#ifndef __STM32L476G_DISCOVERY_UART_H
#define __STM32L476G_DISCOVERY_UART_H

#include "stm32l476xx.h"
#include "stdint.h"

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes);
uint8_t USART_Read (USART_TypeDef * USARTx);
#endif /* __STM32L476G_DISCOVERY_UART_H */
