#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H

#include "sys.h"


// PA11 SDA PA12 SCL

void soft_i2c_init(void);
int IIC_Start(void);
void IIC_Stop(void);
int IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(unsigned char ack);

void i2c_scan(void);
	
#endif
;
