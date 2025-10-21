#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define MA_GPIO_PORT GPIOB
#define MA_IN1_PIN GPIO_Pin_6
#define MA_IN2_PIN GPIO_Pin_7

#define MB_GPIO_PORT GPIOB
#define MB_IN1_PIN GPIO_Pin_8
#define MB_IN2_PIN GPIO_Pin_9


void motor_init(void);
void ma_set_speed(int16_t speed);
void mb_set_speed(int16_t speed);
#endif
