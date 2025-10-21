#ifndef __MPU6050_H
#define __MPU6050_H

#include "sys.h"

/*MPU6050常用寄存器*/

#define MPU6050_SAMPLE_RATE_REG		0X19	//采样频率分频器陀螺仪采样率 （典型值0x07 125Hz）
#define MPU6050_CFG_REG				0X1A	//配置寄存器 低通滤波频率，典型值0x06(5Hz)
#define MPU6050_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器 陀螺仪自检 0x18 不自检
#define MPU6050_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器 加速度自检 0x00 不自检

#define MPU6050_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU6050_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU6050_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU6050_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU6050_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU6050_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器
#define MPU6050_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define MPU6050_TEMP_OUTL_REG		0X42	//温度值低8位寄存器
#define MPU6050_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU6050_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU6050_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU6050_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU6050_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU6050_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU6050_PWR_MGMT1_REG		0X6B	//电源管理寄存器1 0x00 正常启用
#define MPU6050_PWR_MGMT2_REG		0X6C	//电源管理寄存器2

#define MPU6050_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define MPU6050_INT_EN_REG			0X38	//中断使能寄存器

#define	MPU6050_ADDR    0x68    //MPU6050地址
#define  AX_DLPF_ACC21_GYRO20   4 //加速度带宽21Hz 陀螺仪带宽20Hz

void mpu6050_init(void);
float mpu6050_read_temp(void);
void mpu6050_read_data(float acc[3], float gyro[3]);

uint8_t mpu6050_read_id(void);
#endif

