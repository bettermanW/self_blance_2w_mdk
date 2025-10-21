#include "delay.h"

void delay_init()
{
	// 时钟固定为AHB时钟的1/8 9M
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

void delay_us(uint16_t us)
{
    uint32_t temp;
    
    SysTick->LOAD = (SystemCoreClock / 8000000) * us;         // 设置重载值
    SysTick->VAL = 0x00;            // 清除当前计数值
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 启动 SysTick
    
    do
    {
        temp = SysTick->CTRL;        // 读取控制寄存器
    }
    while((temp & 0x01) && !(temp & (1<<16))); // 等待计数完成
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭 SysTick
    SysTick->VAL = 0X00;            // 清除计数值
}

/**
  * @简  述  软件毫妙级延时函数。
  * @参  数  ms：延时长度，单位ms	  	  
  * @返回值  无
  * @说  明  注意ms的范围，SysTick->LOAD为24位寄存器,所以,最大延时为:nTime<=0xffffff*8*1000/SYSCLK
  *          对72M条件下,ms<=1864ms 
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
	}while((temp&0x01)&&!(temp&(1<<16)));	// 定时器没有被关闭，检查 SysTick 是否已经 ?倒数到 0?
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;      
	SysTick->VAL =0X00;     		  		 	    
}

/**
  * @简  述  软件毫秒延时
  * @参  数  ms：延时长度，单位ms	  	 	  
  * @返回值  无
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

