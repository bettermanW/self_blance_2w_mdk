#include "mpu6050.h"

static void mpu6050_write_reg(uint8_t reg_address, uint8_t data);
static void mpu6050_read_reg(uint8_t reg_address, uint8_t *pdata, uint16_t len);


void mpu6050_init(void)
{
	
	soft_i2c_init();
	
	//配置MPU6050寄存器  
	mpu6050_write_reg(MPU6050_PWR_MGMT1_REG,0x00);     //解除休眠状态

	mpu6050_write_reg(MPU6050_GYRO_CFG_REG,0x18);      //陀螺仪量程 默认2000deg/s
	mpu6050_write_reg(MPU6050_ACCEL_CFG_REG,0x00);     //加速计量程 默认2g	
	mpu6050_write_reg(MPU6050_INTBP_CFG_REG,0x80);      //INT引脚低电平有效
	mpu6050_write_reg(MPU6050_PWR_MGMT1_REG,0x01);      //设置CLKSEL,PLL X轴为参考
	mpu6050_write_reg(MPU6050_PWR_MGMT2_REG,0x00); 	    //加速度与陀螺仪都工作
	// 设置数字低通滤波器，带宽约42Hz，同时设定陀螺仪输出基率为1kHz
	mpu6050_write_reg(0x1A, 0x03);
	// 设置输出频率为100Hz (SMPLRT_DIV = 9)
	mpu6050_write_reg(0x19, 9);
	delay_ms(50);  //等待传感器稳定	
	
	
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	//初始化MPU6050中断
	//GPIO功能时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	//配置IO口为复用功能-定时器通道
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	//开启IRQ中断
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);//开启GPIO管脚的中断线路

	EXTI_InitStructure.EXTI_Line = EXTI_Line5;//LINE
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}





/**
  * @简  述  MPU6050获取传感器温度值
  * @参  数  无	  
  * @返回值  传感器温度值。
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

    // 加速度转换为 g
    acc[0] = (int16_t)((acc_buf[0] << 8) | acc_buf[1]) / 16384.0f;
    acc[1] = (int16_t)((acc_buf[2] << 8) | acc_buf[3]) / 16384.0f;
    acc[2] = (int16_t)((acc_buf[4] << 8) | acc_buf[5]) / 16384.0f;

    // 陀螺仪转换为 °/s角速度
    gyro[0] = (int16_t)((gyro_buf[0] << 8) | gyro_buf[1]) / 16.4f;
    gyro[1] = (int16_t)((gyro_buf[2] << 8) | gyro_buf[3]) / 16.4f;
    gyro[2] = (int16_t)((gyro_buf[4] << 8) | gyro_buf[5]) / 16.4f;
	

}


uint8_t mpu6050_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t len, const uint8_t *data)
{
		int i;
    if (!IIC_Start())
        return 0;
    IIC_Send_Byte(dev_addr << 1 ); // 写
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
    if (!IIC_Start()) // 先打开
        return 0;
    IIC_Send_Byte(dev_addr << 1); // 左移一位置，表示最低位为0 写操作
    if (!IIC_Wait_Ack()) { // 没有应答
        IIC_Stop();
        return 0;
    }
    IIC_Send_Byte(reg_addr); // 发送寄存器地址
    IIC_Wait_Ack();	// 等待回应
    IIC_Start();	// 重新开始，因为要更改模式
    IIC_Send_Byte((dev_addr << 1)+1);	// 读操作
    IIC_Wait_Ack();
    while (len) {
        if (len == 1)
					*data = IIC_Read_Byte(0); // 是不是最后一个字节，发送ACK结束
        else
            *data = IIC_Read_Byte(1);	// 不是继续
        data++;
        len--;
    }
    IIC_Stop();
    return 1;
}



/**
  * @简  述  MPU6050写寄存器。
  * @参  数  无	  
  * @返回值  无
  */
static void mpu6050_write_reg(uint8_t reg_address, uint8_t data)
{
	mpu6050_i2c_write(MPU6050_ADDR,reg_address,1,&data);
}

/**
  * @简  述  初始化三轴加速度和三轴陀螺仪（MPU6050）。
  * @参  数  无	  
  * @返回值  无
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

