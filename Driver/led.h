#ifndef __LED_H
#define __LED_H

#include "sys.h"

// ÂÌÉ«PA8, ºìÉ«PC4

#define LED_GREEN_PORT GPIOA
#define LED_GREEN_CLK RCC_APB2Periph_GPIOA
#define LED_GREEN_PIN GPIO_Pin_8

#define LED_RED_PORT GPIOC
#define LED_RED_CLK RCC_APB2Periph_GPIOC
#define LED_RED_PIN GPIO_Pin_4



void led_init(void);

#endif
