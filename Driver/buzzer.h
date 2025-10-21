#ifndef __BUZZER_H
#define __BUZZER_H

#include "sys.h"

#define BUZZER_PIN_PORT GPIOC
#define BUZZER_PIN GPIO_Pin_13

void buzzer_init(void);

#define buzzer_on() GPIO_SetBits(BUZZER_PIN_PORT, BUZZER_PIN)
#define buzzer_off() GPIO_ResetBits(BUZZER_PIN_PORT, BUZZER_PIN)
#endif


