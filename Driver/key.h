#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

// ÓÃ»§°´¼üPB0

#define KEY_PORT GPIOB
#define KEY_PIN GPIO_Pin_0
#define KEY_CLK RCC_APB2Periph_GPIOB

void key_init(void);
uint8_t key_scan(void);

#endif
