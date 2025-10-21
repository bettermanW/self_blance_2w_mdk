#include "key.h"

void key_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(KEY_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 输入模式
	
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

uint8_t key_scan()
{
	// 由原理图可知按下为低电平
	if (GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == 0)
	{
//		while (GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == SET);
		return 1;
		
	}
	return 0;
		

}

