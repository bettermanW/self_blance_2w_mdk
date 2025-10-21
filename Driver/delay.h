#ifndef __DELAY_H
#define __DELAY_H

#include "sys.h"

void delay_init(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
#endif
