#ifndef __MPU6050_H
#define __MPU6050_H

#include "sys.h"

/*MPU6050���üĴ���*/

#define MPU6050_SAMPLE_RATE_REG		0X19	//����Ƶ�ʷ�Ƶ�������ǲ����� ������ֵ0x07 125Hz��
#define MPU6050_CFG_REG				0X1A	//���üĴ��� ��ͨ�˲�Ƶ�ʣ�����ֵ0x06(5Hz)
#define MPU6050_GYRO_CFG_REG		0X1B	//���������üĴ��� �������Լ� 0x18 ���Լ�
#define MPU6050_ACCEL_CFG_REG		0X1C	//���ٶȼ����üĴ��� ���ٶ��Լ� 0x00 ���Լ�

#define MPU6050_ACCEL_XOUTH_REG		0X3B	//���ٶ�ֵ,X���8λ�Ĵ���
#define MPU6050_ACCEL_XOUTL_REG		0X3C	//���ٶ�ֵ,X���8λ�Ĵ���
#define MPU6050_ACCEL_YOUTH_REG		0X3D	//���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU6050_ACCEL_YOUTL_REG		0X3E	//���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU6050_ACCEL_ZOUTH_REG		0X3F	//���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU6050_ACCEL_ZOUTL_REG		0X40	//���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU6050_TEMP_OUTH_REG		0X41	//�¶�ֵ�߰�λ�Ĵ���
#define MPU6050_TEMP_OUTL_REG		0X42	//�¶�ֵ��8λ�Ĵ���
#define MPU6050_GYRO_XOUTH_REG		0X43	//������ֵ,X���8λ�Ĵ���
#define MPU6050_GYRO_XOUTL_REG		0X44	//������ֵ,X���8λ�Ĵ���
#define MPU6050_GYRO_YOUTH_REG		0X45	//������ֵ,Y���8λ�Ĵ���
#define MPU6050_GYRO_YOUTL_REG		0X46	//������ֵ,Y���8λ�Ĵ���
#define MPU6050_GYRO_ZOUTH_REG		0X47	//������ֵ,Z���8λ�Ĵ���
#define MPU6050_GYRO_ZOUTL_REG		0X48	//������ֵ,Z���8λ�Ĵ���

#define MPU6050_PWR_MGMT1_REG		0X6B	//��Դ����Ĵ���1 0x00 ��������
#define MPU6050_PWR_MGMT2_REG		0X6C	//��Դ����Ĵ���2

#define MPU6050_INTBP_CFG_REG		0X37	//�ж�/��·���üĴ���
#define MPU6050_INT_EN_REG			0X38	//�ж�ʹ�ܼĴ���

#define	MPU6050_ADDR    0x68    //MPU6050��ַ
#define  AX_DLPF_ACC21_GYRO20   4 //���ٶȴ���21Hz �����Ǵ���20Hz

void mpu6050_init(void);
float mpu6050_read_temp(void);
void mpu6050_read_data(float acc[3], float gyro[3]);

uint8_t mpu6050_read_id(void);
#endif

