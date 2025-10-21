#include "dr_uart.h"

// ������ջ�������״̬��������Ϊȫ�ֱ�����̬������
#define RX_BUF_SIZE 64
uint8_t USART_RX_BUF[RX_BUF_SIZE];
uint16_t USART_RX_STA = 0; // ״̬�Ĵ�������������¼����״̬�����ݳ���


static void NVIC_Configration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*Ƕ�������жϿ�������ѡ��*/
	
	
	 /* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = USART_IRQ;
	 /* �������ȼ�Ϊ2 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	/* �����ȼ�Ϊ1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	 /* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 /* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

void usart1_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// ��ʱ��
	RCC_APB2PeriphClockCmd(USART_CLK | USART_GPIO_CLK, ENABLE);
	
	
	// ����TX
	GPIO_InitStructure.GPIO_Pin = USART_GPIO_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//���츴��ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_GPIO_TX_PORT, &GPIO_InitStructure);
	// ����RX
	GPIO_InitStructure.GPIO_Pin = USART_GPIO_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // �������� ������ȫ���ⲿ�������
	GPIO_Init(USART_GPIO_RX_PORT, &GPIO_InitStructure);
	
	// ���ô���
	// ������
	USART_InitStructure.USART_BaudRate = USART_BUADRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);
	 
	// �����ж����ȼ�����
	NVIC_Configration();
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	// ʹ�ܴ���
	USART_Cmd(USARTx, ENABLE);
	
	
}

void USART1_IRQHandler(void)
{
	uint8_t received_data;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // ���յ�����Ϣ
  {
		received_data = USART_ReceiveData(USART1);
  // �����ٶ�ָ�0~255��Ӧռ�ձ�0%~100%��
		if (received_data >= 0 && received_data <= 255) 
		{
			uint16_t pwm_value = (received_data * 3599) / 255; // ӳ�䵽ARR��Χ
			TIM_SetCompare1(TIM4, pwm_value);  // ����PWMռ�ձ�
			TIM_SetCompare2(TIM4, 0);          // ����ͨ���رգ���������ƣ�
			
			// �ش���ǰ�ٶȣ���ѡ��
			USART_SendData(USART1, received_data);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		
		}
	}
}


// �����ض���
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
	
		// ��д����һ�ֽ�
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// �������ݼĴ����Ƿ��(�����ȴ�)
    return ch;
}
