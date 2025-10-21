#ifndef __DR_UART_H
#define __DR_UART_H

#include "sys.h"

#define USARTx										USART1
#define USART_CLK 							RCC_APB2Periph_USART1

#define USART_BUADRATE					115200

#define USART_GPIO_CLK					RCC_APB2Periph_GPIOA

#define USART_GPIO_RX_PORT 			GPIOA
#define USART_GPIO_RX 					GPIO_Pin_10
#define USART_GPIO_TX_PORT 			GPIOA
#define USART_GPIO_TX 					GPIO_Pin_9

#define USART_IRQ 							USART1_IRQn
#define USART_IRQHandler 				USART1_IRQHandler

void usart1_init(void);
#endif
