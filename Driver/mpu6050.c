#include "mpu6050.h"

static void mpu6050_write_reg(uint8_t reg_address, uint8_t data);
static void mpu6050_read_reg(uint8_t reg_address, uint8_t *pdata, uint16_t len);


void mpu6050_init(void)
{
	
	soft_i2c_init();
	
	//����MPU6050�Ĵ���  
	mpu6050_write_reg(MPU6050_PWR_MGMT1_REG,0x00);     //�������״̬

	mpu6050_write_reg(MPU6050_GYRO_CFG_REG,0x18);      //���������� Ĭ��2000deg/s
	mpu6050_write_reg(MPU6050_ACCEL_CFG_REG,0x00);     //���ټ����� Ĭ��2g	
	mpu6050_write_reg(MPU6050_INTBP_CFG_REG,0x80);      //INT���ŵ͵�ƽ��Ч
	mpu6050_write_reg(MPU6050_PWR_MGMT1_REG,0x01);      //����CLKSEL,PLL X��Ϊ�ο�
	mpu6050_write_reg(MPU6050_PWR_MGMT2_REG,0x00); 	    //���ٶ��������Ƕ�����
	// �������ֵ�ͨ�˲���������Լ42Hz��ͬʱ�趨�������������Ϊ1kHz
	mpu6050_write_reg(0x1A, 0x03);
	// �������Ƶ��Ϊ100Hz (SMPLRT_DIV = 9)
	mpu6050_write_reg(0x19, 9);
	delay_ms(50);  //�ȴ��������ȶ�	
	
	
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	//��ʼ��MPU6050�ж�
	//GPIO����ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	//����IO��Ϊ���ù���-��ʱ��ͨ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	//����IRQ�ж�
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);//����GPIO�ܽŵ��ж���·

	EXTI_InitStructure.EXTI_Line = EXTI_Line5;//LINE
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE
	EXTI_Init(&EXTI_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
}





/**
  * @��  ��  MPU6050��ȡ�������¶�ֵ
  * @��  ��  ��	  
  * @����ֵ  �������¶�ֵ��
  */
float mpu6050_read_temp(void)
{	
	uint8_t buf[2];
	int16_t tmp;

	mpu6050_read_reg(MPU6050_TEMP_OUTH_REG,buf,2);

	tmp = (buf[0]<<8)| buf[1];
	
	return ( 36.53f + ((double)tmp/340.0f) );	
}


void mpu6050_read_data(float acc[3], float gyro[3]) 
{
    uint8_t acc_buf[6], gyro_buf[6];
    mpu6050_read_reg(MPU6050_ACCEL_XOUTH_REG, acc_buf, 6);
    mpu6050_read_reg(MPU6050_GYRO_XOUTH_REG, gyro_buf, 6);

    // ���ٶ�ת��Ϊ g
    acc[0] = (int16_t)((acc_buf[0] << 8) | acc_buf[1]) / 16384.0f;
    acc[1] = (int16_t)((acc_buf[2] << 8) | acc_buf[3]) / 16384.0f;
    acc[2] = (int16_t)((acc_buf[4] << 8) | acc_buf[5]) / 16384.0f;

    // ������ת��Ϊ ��/s���ٶ�
    gyro[0] = (int16_t)((gyro_buf[0] << 8) | gyro_buf[1]) / 16.4f;
    gyro[1] = (int16_t)((gyro_buf[2] << 8) | gyro_buf[3]) / 16.4f;
    gyro[2] = (int16_t)((gyro_buf[4] << 8) | gyro_buf[5]) / 16.4f;
	

}


uint8_t mpu6050_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, const uint8_t *data)
{
		int i;
    if (!IIC_Start())
        return 0;
    IIC_Send_Byte(dev_addr << 1 ); // д
    if (!IIC_Wait_Ack()) {
        IIC_Stop();
        return 0;
    }
    IIC_Send_Byte(reg_addr); // 
    IIC_Wait_Ack();
		for (i = 0; i < len; i++) {
        IIC_Send_Byte(data[i]);
        if (!IIC_Wait_Ack()) {
            IIC_Stop();
            return 1;
        }
    }
    IIC_Stop();
    return 1;
}


uint8_t mpu6050_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, uint8_t *data)
{
    if (!IIC_Start()) // �ȴ�
        return 0;
    IIC_Send_Byte(dev_addr << 1); // ����һλ�ã���ʾ���λΪ0 д����
    if (!IIC_Wait_Ack()) { // û��Ӧ��
        IIC_Stop();
        return 0;
    }
    IIC_Send_Byte(reg_addr); // ���ͼĴ�����ַ
    IIC_Wait_Ack();	// �ȴ���Ӧ
    IIC_Start();	// ���¿�ʼ����ΪҪ����ģʽ
    IIC_Send_Byte((dev_addr << 1)+1);	// ������
    IIC_Wait_Ack();
    while (len) {
        if (len == 1)
					*data = IIC_Read_Byte(0); // �ǲ������һ���ֽڣ�����ACK����
        else
            *data = IIC_Read_Byte(1);	// ���Ǽ���
        data++;
        len--;
    }
    IIC_Stop();
    return 1;
}



/**
  * @��  ��  MPU6050д�Ĵ�����
  * @��  ��  ��	  
  * @����ֵ  ��
  */
static void mpu6050_write_reg(uint8_t reg_address, uint8_t data)
{
	mpu6050_i2c_write(MPU6050_ADDR,reg_address,1,&data);
}

/**
  * @��  ��  ��ʼ��������ٶȺ����������ǣ�MPU6050����
  * @��  ��  ��	  
  * @����ֵ  ��
  */
static void mpu6050_read_reg(uint8_t reg_address, uint8_t *pdata, uint16_t len)
{
	mpu6050_i2c_read(MPU6050_ADDR,reg_address,len,pdata);
}


uint8_t mpu6050_read_id(void) {
    uint8_t who_am_i;
    mpu6050_i2c_read(MPU6050_ADDR, 	0x75, 1, &who_am_i);
    return who_am_i;
}

