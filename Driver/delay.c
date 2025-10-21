#include "delay.h"

void delay_init()
{
	// ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8 9M
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

void delay_us(uint16_t us)
{
    uint32_t temp;
    
    SysTick->LOAD = (SystemCoreClock / 8000000) * us;         // ��������ֵ
    SysTick->VAL = 0x00;            // �����ǰ����ֵ
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // ���� SysTick
    
    do
    {
        temp = SysTick->CTRL;        // ��ȡ���ƼĴ���
    }
    while((temp & 0x01) && !(temp & (1<<16))); // �ȴ��������
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // �ر� SysTick
    SysTick->VAL = 0X00;            // �������ֵ
}

/**
  * @��  ��  ��������ʱ������
  * @��  ��  ms����ʱ���ȣ���λms	  	  
  * @����ֵ  ��
  * @˵  ��  ע��ms�ķ�Χ��SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:nTime<=0xffffff*8*1000/SYSCLK
  *          ��72M������,ms<=1864ms 
  */
static void Delay_ms(uint16_t ms)
{	 		  	  
	uint32_t temp;	
	
	SysTick->LOAD=(uint32_t)(SystemCoreClock / 8000) * ms;
	SysTick->VAL =0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	// ��ʱ��û�б��رգ���� SysTick �Ƿ��Ѿ� ?������ 0?
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;      
	SysTick->VAL =0X00;     		  		 	    
}

/**
  * @��  ��  ���������ʱ
  * @��  ��  ms����ʱ���ȣ���λms	  	 	  
  * @����ֵ  ��
  */
void delay_ms(uint16_t ms)
{
	uint8_t repeat=ms/500;																
	uint16_t remain=ms%500;
	
	while(repeat)
	{
		Delay_ms(500);
		repeat--;
	}
	
	if(remain)
	{
		Delay_ms(remain);
	}
}

