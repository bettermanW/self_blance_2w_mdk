#ifndef __ENCODER_H
#define __ENCODER_H

#include "sys.h"

#define ENCODER_A_PIN_A GPIO_Pin_15 // PA15
#define ENCODER_A_PIN_B GPIO_Pin_3	// PB3

#define ENCODER_B_PIN_A GPIO_Pin_4 // PB4
#define ENCODER_B_PIN_B GPIO_Pin_5	// PB5

void encoder_a_init(void);
void encoder_b_init(void);

void encoder_a_set(uint16_t counter);
uint16_t encoder_a_get(void);

void encoder_b_set(uint16_t counter);
uint16_t encoder_b_get(void);
#endif
