#include "buzzer.h"

void buzzer_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(BUZZER_PIN_PORT, &GPIO_InitStructure);
	
	GPIO_ResetBits(BUZZER_PIN_PORT, BUZZER_PIN);	// Ä¬ÈÏ¹Ø±Õ·äÃùÆ÷
}

