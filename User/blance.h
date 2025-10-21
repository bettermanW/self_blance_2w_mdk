#ifndef BLANCE_H
#define BLANCE_H

#include "sys.h"

#define  PI           3.1416     //Բ����PI
#define  PID_RATE     100         //PIDƵ��
#define  WHEEL_DIAMETER	      0.066	  //����ֱ��
#define  WHEEL_BASE           0.167	  //�־࣬�����ֵľ���
#define  WHEEL_RESOLUTION     2496.0  //�������ֱ���(13��),���ٱ�30,13x30x4=1560
#define  WHEEL_SCALE          (PI *WHEEL_DIAMETER*PID_RATE/WHEEL_RESOLUTION)  //�����ٶ�m/s�������ת��ϵ��

void oled_show(void);
#endif

