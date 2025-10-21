#include "blance.h"


int16_t balance_angle_ctl(int16_t target, float angle, float gyro);
int16_t balance_velocity_ctl(int16_t t_vx, float r_vx);

// �����˲�ϵ��
#define ALPHA                 0.98f  // ʹ��f��׺��ȷΪfloat����
#define DT                    0.01f  // 10ms�������ڶ�Ӧ������
#define M_PI                  3.1415926535f  // ���Բ���ʾ���

float acc[3], gyro[3];
float acc_angle, car_angle; // ���ٶȼƽǶȣ�С����б�Ƕ�

// �����˲���ȫ��״̬����
static float last_angle = 0.0f;  // ������һ�εĽǶȹ���ֵ

/**
  * @brief  �����˲�������
  * @param  acc_angle: ���ٶȼƼ���ĽǶȣ��ȣ�
  * @param  gyro_rate: �����ǵĽ��ٶȣ���/�룩
  * @param  dt: ����ʱ�䣨�룩
  * @retval �˲���ĽǶȹ���ֵ���ȣ�
  */
float complementary_filter(float acc_angle, float gyro_rate, float dt)
{
    // ���Ĺ�ʽ���Ƕ� = �� �� (��һ�Ƕ� + �����ǻ���) + (1-��) �� ���ٶȼƽǶ�
    last_angle = ALPHA * (last_angle + gyro_rate * dt) + (1 - ALPHA) * acc_angle;
    return last_angle;
}

/**
  * @brief  EXTI9_5�жϷ������������棩
  */
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
    {		
//				printf("world\n");
			// ����жϱ�־λ
			EXTI_ClearITPendingBit(EXTI_Line5);
			GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);
			
			// ��ȡMPU6050ԭʼ����
			mpu6050_read_data(acc, gyro);
			
			// ������ٶȼƽǶȣ�atan2����˳�������ϵ������أ������ʵ�ʵ�����
			// ע�⣺�������Y���Z����ƽ��ƽ���ڣ��������
			acc_angle = atan2(acc[1], acc[2]) * 180.0f / M_PI;
			
			// ʹ�û����˲����ں����ݣ�ע�����������ݵĵ�λת����
			// ����������ԭʼ������ת��Ϊ��/�룬��Ϊԭʼ�������������ת��
			car_angle = complementary_filter(acc_angle, gyro[0], DT);
		
		
			//		//ͨ����������ȡ����ʵʱת��m/s��ǰ���������˸�
			int16_t wheel_vel_l = (float)-((int16_t)encoder_a_get()*WHEEL_SCALE);  //����ת��
			encoder_a_set(0);
			int16_t wheel_vel_r = (float) ((int16_t)encoder_b_get()*WHEEL_SCALE);  //����ת��
			encoder_a_set(0);	
			int16_t wheel_vel = (wheel_vel_l + wheel_vel_r)/2;	//����С���ٶ�	
        
			 int16_t balance_out =  balance_angle_ctl(0, car_angle, gyro[0]);
			 int16_t velocity_out = balance_velocity_ctl(0, wheel_vel);   //��ʻ�ٶȿ���
			 
			int16_t wheel_pwm_l  = balance_out + velocity_out;
			int16_t wheel_pwm_r  = balance_out + velocity_out;
			 
			 ma_set_speed(-wheel_pwm_l);
			 mb_set_speed(wheel_pwm_r);
			 
    }
}

char buff[30];

/**
  * @brief  OLED��ʾ����
  */
void oled_show(void)
{
		printf("Angle: %.1f\n��", car_angle);
		uint16_t vol =   get_vol() / 100;
    
    OLED_NewFrame();
    sprintf(buff, "Angle: %.1f��", car_angle);  // ��ʾһλС������ȷ
    OLED_PrintASCIIString(0, 0, buff, &afont8x6, 0);
		sprintf(buff, "Vol: %df��", vol);
		OLED_PrintASCIIString(0, 10, buff, &afont8x6, 0);
		OLED_DrawLine(0, 20, 0, 30, 0);
    OLED_ShowFrame();
}



// ֱ�������ȶ�
#define balance_kp 150 // �����Դ�ʱ��Ӧ���ʵ���ͬʱ��̬���С
#define balance_kd 5	// ���������С

int16_t balance_angle_ctl(int16_t target, float angle, float gyro)
{ 
	
	float angle_bias,gyro_bias;  //�ǶȺͽ��ٶ�ƫ��
	int16_t p_out,d_out,out;    //PID���ֵ
	
	//����ƽ��Ƕ�ƫ��
	angle_bias = target - angle;  
	
	//����ƽ����ٶ�ƫ��
	gyro_bias  = 0 - gyro;  
	
	//����ƽ��Ƕȿ���PWM�����pk��pdΪϵ����out = kp*e(k) + kd*[e(k)-e(k-1)]
	p_out = balance_kp * angle_bias;
	d_out = balance_kd * gyro_bias;
	
	//���������ֵ
	out =  p_out + d_out;
	
	//�������ݲ鿴
	//printf("@%d %d %d \r\n ",out, p_out, d_out); 
	printf("@%d %d \r\n ",target, (int)angle); 
	
	return out;
}



#define velocity_kp 8000
#define velocity_ki 4000
//
/**
  * @brief   �ٶȻ� ��֤��ֹ��һ���ط�
  * @param  t_vx: Ŀ���ٶ�
  * @param  r_vx: ʵʱ�ٶ�
  * @retval PID�������ֵ���PWMֵ
  */
int16_t balance_velocity_ctl(int16_t t_vx, float r_vx)
{ 
	
	static float velocity_bias = 0, velocity_bias_update;
	static float velocity_integral = 0;
	float  velocity_target;  //�ٶ�Ŀ��ֵ
	int16_t p_out,i_out,out; //PID���ֵ
	
	//Ŀ���ٶ��޷�
	if(t_vx > 700) t_vx = 700;
	else if(t_vx <-700) t_vx = -700;
	
	//����˲ʱ�ٶ�ƫ�Ŀ���ٶ�Ϊ0
	velocity_bias_update = 0 - r_vx;
	
	//һ�׵�ͨ�˲���, ��С�ٶȲ�����ƽ�����Ӱ��
	velocity_bias = (velocity_bias*0.6) + (velocity_bias_update*0.4); 
	
	//�ٶȻ��ּ����λ�ƣ���λm������ʱ��10ms
	velocity_integral += velocity_bias*0.01f;  
	
	//����Ŀ���ٶȣ���λΪm/s��,vx��λΪmm/s,����ʱ��0.01s
	velocity_target = t_vx*0.001f*0.01f;
	
	//����Ŀ���ٶȿ��ƣ�������ʻ�ٶ�
	velocity_integral += velocity_target; 
	
	
	//���ƻ��ֱ���
	if(velocity_integral > 1.5)  velocity_integral = 1.5;
	else if(velocity_integral < -1.5)  velocity_integral = -1.5;

	//�����ٶȿ���PWM�����pk��piΪϵ��
	p_out =  -velocity_kp * velocity_bias;
	i_out =  -velocity_ki * velocity_integral;
	
	//���������ֵ
	out =  p_out + i_out;
	
	
	return out;

}

