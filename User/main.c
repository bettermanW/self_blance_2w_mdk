
#include "stm32f10x.h"
#include "sys.h"


int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);   
	delay_init();
	led_init();
	key_init();
	usart1_init();
	vin_init();
	buzzer_init();
	
//	buzzer_on();
//	delay_ms(100);
	mpu6050_init();
	buzzer_off();
	oled_init();
	delay_ms(10);
	motor_init();
	encoder_a_init();
	encoder_b_init();
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	
	printf("hello\n");
//	I2C_Scan();
//	

	
	while (1) 
	{
		oled_show();
		delay_ms(100);
			
	}
}
