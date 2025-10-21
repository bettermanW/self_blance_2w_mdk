#include "vin.h"

#define ADC_REVISE  99.9

void vin_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 	// ģ������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*����ADC*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	
	/*ģʽ����*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	// һ��ADC�ǵ�ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; // ��ͨ���ر�
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // ����Զ�ת�����ǵ���ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 	// ����Զ�����
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 
	ADC_InitStructure.ADC_NbrOfChannel = 1; // ת��ͨ��
	ADC_Init(ADC2, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	// ADC��Ƶ���ӣ���󲻳���14M
	
	// ����ADCͨ����ת��˳�򣬺Ͳ���ʱ�� 239.5��
	ADC_RegularChannelConfig(ADC2, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);
	
	ADC_Cmd(ADC2, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���
	ADC_ResetCalibration(ADC2);
	// �ȴ��Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC2));
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC2);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC2));
		
}

uint16_t get_vol(void)
{
	// �������ת������
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)); // ת������
	uint16_t val = ADC_GetConversionValue(ADC2);
	uint16_t Input_Vol = (uint16_t)((3.3*11.0*ADC_REVISE * val)/4095);	
	
	return Input_Vol;
	
}
