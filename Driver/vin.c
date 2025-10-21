#include "vin.h"

#define ADC_REVISE  99.9

void vin_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 	// 模拟输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*配置ADC*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	
	/*模式配置*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	// 一个ADC是单模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 单通道关闭
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 多次自动转换还是单次转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 	// 软件自动触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 
	ADC_InitStructure.ADC_NbrOfChannel = 1; // 转换通道
	ADC_Init(ADC2, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	// ADC分频因子，最大不超过14M
	
	// 配置ADC通道、转换顺序，和采样时间 239.5个
	ADC_RegularChannelConfig(ADC2, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);
	
	ADC_Cmd(ADC2, ENABLE);
	
	// 初始化ADC 校准寄存器
	ADC_ResetCalibration(ADC2);
	// 等待寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC2));
	// ADC开始校准
	ADC_StartCalibration(ADC2);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC2));
		
}

uint16_t get_vol(void)
{
	// 软件启动转换功能
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)); // 转换结束
	uint16_t val = ADC_GetConversionValue(ADC2);
	uint16_t Input_Vol = (uint16_t)((3.3*11.0*ADC_REVISE * val)/4095);	
	
	return Input_Vol;
	
}
