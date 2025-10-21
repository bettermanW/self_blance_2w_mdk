
#include "led.h"

void led_init()
{
	 /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/*�����ⲿʱ��*/
	RCC_APB2PeriphClockCmd(LED_GREEN_CLK | LED_RED_CLK, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;
	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 /*������������Ϊ50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(LED_GREEN_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED_RED_PIN;
	GPIO_Init(LED_RED_PORT, &GPIO_InitStructure);
	
	// �ر�LED
	GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);
	GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);
	

}
