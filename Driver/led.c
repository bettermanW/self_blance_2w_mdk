
#include "led.h"

void led_init()
{
	 /*定义一个GPIO_InitTypeDef类型的结构体*/
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启外部时钟*/
	RCC_APB2PeriphClockCmd(LED_GREEN_CLK | LED_RED_CLK, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;
	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 /*设置引脚速率为50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*调用库函数，初始化GPIO*/
	GPIO_Init(LED_GREEN_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED_RED_PIN;
	GPIO_Init(LED_RED_PORT, &GPIO_InitStructure);
	
	// 关闭LED
	GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);
	GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);
	

}
