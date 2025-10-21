#ifndef BLANCE_H
#define BLANCE_H

#include "sys.h"

#define  PI           3.1416     //圆周率PI
#define  PID_RATE     100         //PID频率
#define  WHEEL_DIAMETER	      0.066	  //轮子直径
#define  WHEEL_BASE           0.167	  //轮距，左右轮的距离
#define  WHEEL_RESOLUTION     2496.0  //编码器分辨率(13线),减速比30,13x30x4=1560
#define  WHEEL_SCALE          (PI *WHEEL_DIAMETER*PID_RATE/WHEEL_RESOLUTION)  //轮子速度m/s与编码器转换系数

void oled_show(void);
#endif

