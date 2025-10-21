#include "motor.h"

int16_t pwm_cycle  =  3600;



void motor_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 

	//GPIO功能时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	//配置IO口为复用功能-定时器通道
	GPIO_InitStructure.GPIO_Pin =  MA_IN1_PIN| MA_IN2_PIN | MB_IN1_PIN | MB_IN2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
	GPIO_Init(MB_GPIO_PORT, &GPIO_InitStructure);

	//TIM时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//Time base configuration
	TIM_TimeBaseStructure.TIM_Period = 3600-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//PWM1 Mode configuration: Channel1 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;	    //占空比初始化
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	//PWM1 Mode configuration: Channel2
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	//PWM1 Mode configuration: Channel3
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	//PWM1 Mode configuration: Channel4
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);

	//TIM enable counter
	TIM_Cmd(TIM4, ENABLE);   

	//使能MOE位
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	
}

void ma_set_speed(int16_t speed)
{
	if (speed > pwm_cycle)
	{
		speed = pwm_cycle;
	}
	if (speed < -pwm_cycle)
	{
		speed = -pwm_cycle;
	}
	
	if (speed > 0) // 正转
	{
		TIM_SetCompare1(TIM4, pwm_cycle);
		TIM_SetCompare2(TIM4, pwm_cycle - speed);
	}
	else
	{
		TIM_SetCompare1(TIM4, pwm_cycle + speed);
		TIM_SetCompare2(TIM4, pwm_cycle);
		
	}
}

void mb_set_speed(int16_t speed)
{
	if (speed > pwm_cycle)
	{
		speed = pwm_cycle;
	}
	if (speed < -pwm_cycle)
	{
		speed = -pwm_cycle;
	}
	
	if (speed > 0) // 正转
	{
		TIM_SetCompare3(TIM4, pwm_cycle);
		TIM_SetCompare4(TIM4, pwm_cycle - speed);
	}
	else
	{
		TIM_SetCompare3(TIM4, pwm_cycle + speed);
		TIM_SetCompare4(TIM4, pwm_cycle);
		
	}

}
