#include "blance.h"


int16_t balance_angle_ctl(int16_t target, float angle, float gyro);
int16_t balance_velocity_ctl(int16_t t_vx, float r_vx);

// 互补滤波系数
#define ALPHA                 0.98f  // 使用f后缀明确为float类型
#define DT                    0.01f  // 10ms采样周期对应的秒数
#define M_PI                  3.1415926535f  // 提高圆周率精度

float acc[3], gyro[3];
float acc_angle, car_angle; // 加速度计角度，小车倾斜角度

// 互补滤波器全局状态变量
static float last_angle = 0.0f;  // 保存上一次的角度估计值

/**
  * @brief  互补滤波器函数
  * @param  acc_angle: 加速度计计算的角度（度）
  * @param  gyro_rate: 陀螺仪的角速度（度/秒）
  * @param  dt: 采样时间（秒）
  * @retval 滤波后的角度估计值（度）
  */
float complementary_filter(float acc_angle, float gyro_rate, float dt)
{
    // 核心公式：角度 = α × (上一角度 + 陀螺仪积分) + (1-α) × 加速度计角度
    last_angle = ALPHA * (last_angle + gyro_rate * dt) + (1 - ALPHA) * acc_angle;
    return last_angle;
}

/**
  * @brief  EXTI9_5中断服务函数（修正版）
  */
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
    {		
//				printf("world\n");
			// 清除中断标志位
			EXTI_ClearITPendingBit(EXTI_Line5);
			GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);
			
			// 读取MPU6050原始数据
			mpu6050_read_data(acc, gyro);
			
			// 计算加速度计角度（atan2参数顺序和坐标系定义相关，请根据实际调整）
			// 注意：这里假设Y轴和Z轴在平衡平面内，计算倾角
			acc_angle = atan2(acc[1], acc[2]) * 180.0f / M_PI;
			
			// 使用互补滤波器融合数据（注意陀螺仪数据的单位转换）
			// 假设陀螺仪原始数据已转换为度/秒，若为原始数据需根据量程转换
			car_angle = complementary_filter(acc_angle, gyro[0], DT);
		
		
			//		//通过编码器获取车轮实时转速m/s，前进正，后退负
			int16_t wheel_vel_l = (float)-((int16_t)encoder_a_get()*WHEEL_SCALE);  //左轮转速
			encoder_a_set(0);
			int16_t wheel_vel_r = (float) ((int16_t)encoder_b_get()*WHEEL_SCALE);  //右轮转速
			encoder_a_set(0);	
			int16_t wheel_vel = (wheel_vel_l + wheel_vel_r)/2;	//计算小车速度	
        
			 int16_t balance_out =  balance_angle_ctl(0, car_angle, gyro[0]);
			 int16_t velocity_out = balance_velocity_ctl(0, wheel_vel);   //行驶速度控制
			 
			int16_t wheel_pwm_l  = balance_out + velocity_out;
			int16_t wheel_pwm_r  = balance_out + velocity_out;
			 
			 ma_set_speed(-wheel_pwm_l);
			 mb_set_speed(wheel_pwm_r);
			 
    }
}

char buff[30];

/**
  * @brief  OLED显示函数
  */
void oled_show(void)
{
		printf("Angle: %.1f\n°", car_angle);
		uint16_t vol =   get_vol() / 100;
    
    OLED_NewFrame();
    sprintf(buff, "Angle: %.1f°", car_angle);  // 显示一位小数更精确
    OLED_PrintASCIIString(0, 0, buff, &afont8x6, 0);
		sprintf(buff, "Vol: %df°", vol);
		OLED_PrintASCIIString(0, 10, buff, &afont8x6, 0);
		OLED_DrawLine(0, 20, 0, 30, 0);
    OLED_ShowFrame();
}



// 直立环，稳定
#define balance_kp 150 // 经测试此时反应最适当，同时稳态误差小
#define balance_kd 5	// 抖动情况最小

int16_t balance_angle_ctl(int16_t target, float angle, float gyro)
{ 
	
	float angle_bias,gyro_bias;  //角度和角速度偏差
	int16_t p_out,d_out,out;    //PID输出值
	
	//计算平衡角度偏差
	angle_bias = target - angle;  
	
	//计算平衡角速度偏差
	gyro_bias  = 0 - gyro;  
	
	//计算平衡角度控制PWM输出，pk，pd为系数，out = kp*e(k) + kd*[e(k)-e(k-1)]
	p_out = balance_kp * angle_bias;
	d_out = balance_kd * gyro_bias;
	
	//计算总输出值
	out =  p_out + d_out;
	
	//调试数据查看
	//printf("@%d %d %d \r\n ",out, p_out, d_out); 
	printf("@%d %d \r\n ",target, (int)angle); 
	
	return out;
}



#define velocity_kp 8000
#define velocity_ki 4000
//
/**
  * @brief   速度环 保证静止在一个地方
  * @param  t_vx: 目标速度
  * @param  r_vx: 实时速度
  * @retval PID控制输出值电机PWM值
  */
int16_t balance_velocity_ctl(int16_t t_vx, float r_vx)
{ 
	
	static float velocity_bias = 0, velocity_bias_update;
	static float velocity_integral = 0;
	float  velocity_target;  //速度目标值
	int16_t p_out,i_out,out; //PID输出值
	
	//目标速度限幅
	if(t_vx > 700) t_vx = 700;
	else if(t_vx <-700) t_vx = -700;
	
	//计算瞬时速度偏差，目标速度为0
	velocity_bias_update = 0 - r_vx;
	
	//一阶低通滤波器, 减小速度波动对平衡控制影响
	velocity_bias = (velocity_bias*0.6) + (velocity_bias_update*0.4); 
	
	//速度积分计算出位移，单位m，积分时间10ms
	velocity_integral += velocity_bias*0.01f;  
	
	//计算目标速度（单位为m/s）,vx单位为mm/s,积分时间0.01s
	velocity_target = t_vx*0.001f*0.01f;
	
	//加入目标速度控制，控制行驶速度
	velocity_integral += velocity_target; 
	
	
	//限制积分饱和
	if(velocity_integral > 1.5)  velocity_integral = 1.5;
	else if(velocity_integral < -1.5)  velocity_integral = -1.5;

	//计算速度控制PWM输出，pk，pi为系数
	p_out =  -velocity_kp * velocity_bias;
	i_out =  -velocity_ki * velocity_integral;
	
	//计算总输出值
	out =  p_out + i_out;
	
	
	return out;

}

