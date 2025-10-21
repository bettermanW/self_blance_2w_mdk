#include "dr_uart.h"

// 定义接收缓冲区和状态变量（需为全局变量或静态变量）
#define RX_BUF_SIZE 64
uint8_t USART_RX_BUF[RX_BUF_SIZE];
uint16_t USART_RX_STA = 0; // 状态寄存器，可用来记录接收状态和数据长度


static void NVIC_Configration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*嵌套向量中断控制器组选择*/
	
	
	 /* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = USART_IRQ;
	 /* 抢断优先级为2 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	/* 子优先级为1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	 /* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 /* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

void usart1_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// 打开时钟
	RCC_APB2PeriphClockCmd(USART_CLK | USART_GPIO_CLK, ENABLE);
	
	
	// 配置TX
	GPIO_InitStructure.GPIO_Pin = USART_GPIO_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//推挽复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_GPIO_TX_PORT, &GPIO_InitStructure);
	// 配置RX
	GPIO_InitStructure.GPIO_Pin = USART_GPIO_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入 数据完全由外部输入决定
	GPIO_Init(USART_GPIO_RX_PORT, &GPIO_InitStructure);
	
	// 配置串口
	// 波特率
	USART_InitStructure.USART_BaudRate = USART_BUADRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);
	 
	// 串口中断优先级配置
	NVIC_Configration();
	// 使能串口接收中断
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	// 使能串口
	USART_Cmd(USARTx, ENABLE);
	
	
}

void USART1_IRQHandler(void)
{
	uint8_t received_data;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收到了消息
  {
		received_data = USART_ReceiveData(USART1);
  // 解析速度指令（0~255对应占空比0%~100%）
		if (received_data >= 0 && received_data <= 255) 
		{
			uint16_t pwm_value = (received_data * 3599) / 255; // 映射到ARR范围
			TIM_SetCompare1(TIM4, pwm_value);  // 设置PWM占空比
			TIM_SetCompare2(TIM4, 0);          // 反向通道关闭（单方向控制）
			
			// 回传当前速度（可选）
			USART_SendData(USART1, received_data);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		
		}
	}
}


// 串口重定义
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);
	
		// 可写入下一字节
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 发送数据寄存器是否空(阻塞等待)
    return ch;
}
