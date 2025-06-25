#include "headfile.h"
/*********************************************************************
���PID������ʽPID�̶�PID�����ڸĶ�̬PID 

**********************************************************************/
int get_speed_zuo;
int get_speed_you;
int set_speed_zuo=300;
int set_speed_you=300;

float distance_obstruct=0;
float distance_scratch=0;

//��������ʼ��
void FTM_Init(void)
{  
  //�ҵ��
  ftm_pwm_init(ftm0,ftm_ch2,30000,0);          
  ftm_pwm_init(ftm0,ftm_ch5,30000,0);          
  //����
  ftm_pwm_init(ftm0,ftm_ch3,30000,0);          
  ftm_pwm_init(ftm0,ftm_ch4,30000,0);
  //���
  ftm_pwm_init(ftm3,ftm_ch3,50,875);
  //�������
  ftm_quad_init(ftm2); 
  //�ұ�����
  ftm_quad_init(ftm1);    
}
//����������
void Speed_Get(void)
{
  get_speed_zuo=2*ftm_quad_get(ftm1);  
  ftm_quad_clean(ftm1);
 
  get_speed_you=-2*ftm_quad_get(ftm2);
  ftm_quad_clean(ftm2);

  if(obstructflag==3)
  {
    distance_obstruct+=(float)get_speed_zuo*0.01+(float)get_speed_you*0.01;
  }
  if(distance_scratch<500)
    distance_scratch+=(float)get_speed_zuo*0.01+(float)get_speed_you*0.01;
}
//����
void EM_Control_zuo(void)
{
  int EM_pwm;
  int error_P_zuo,error_I_zuo,error_D_zuo;   
  static int error_now,error_last,error_previous; 
  float kp_zuo=5,ki_zuo=0,kd_zuo=1;      //6

  if(obstructflag==1)
    kp_zuo=8;
  
//������ʷƫ��
  error_previous=error_last;
  error_last=error_now;
  //pid����
  //��ȡ���ڵ�ƫ��
  error_now=set_speed_zuo-get_speed_zuo;
  //��ȡpi����
  error_P_zuo=error_now;
  error_I_zuo=error_now;
  error_D_zuo=2*error_last-error_now-error_previous;
  EM_pwm=(int)(kp_zuo*error_P_zuo+ki_zuo*error_I_zuo+kd_zuo*error_D_zuo);
        
  //�޷�
  if(EM_pwm>850)
    EM_pwm=850;
  else if(EM_pwm<-850)
    EM_pwm=-850;
  //�����ֵ
  if(EM_pwm>=0)
  {
    ftm_pwm_duty(ftm0,ftm_ch4,0);              //�ı���ռ�ձ�
    ftm_pwm_duty(ftm0,ftm_ch3,EM_pwm);
  }
  else
  {
    ftm_pwm_duty(ftm0,ftm_ch3,0);              //�ı���ռ�ձ�
    ftm_pwm_duty(ftm0,ftm_ch4,(-EM_pwm));
  }
}
//�ҵ��
void EM_Control_you(void)
{
  int EM_pwm;
  static int error_now,error_last,error_previous;
  int error_P_you,error_I_you,error_D_you;    
  float kp_you=5,ki_you=0,kd_you=2; 
  
  if(obstructflag==1)
    kp_you=8;
  
  //������ʷƫ��
  error_previous=error_last;
  error_last=error_now;
  //pid����
  //��ȡ���ڵ�ƫ��
  error_now=set_speed_you-get_speed_you;
  //��ȡpi����
  error_P_you=error_now;
  error_I_you=error_now;
  error_D_you=2*error_last-error_now-error_previous;
  EM_pwm=(int)(kp_you*error_P_you+ki_you*error_I_you+kd_you*error_D_you);
    
  //�޷�
  if(EM_pwm>850)
    EM_pwm=850;
  else if(EM_pwm<-850)
    EM_pwm=-850;
  //�����ֵ
  if(EM_pwm>=0)
  {
    ftm_pwm_duty(ftm0,ftm_ch2,0);              //�ı���ռ�ձ�
    ftm_pwm_duty(ftm0,ftm_ch5,EM_pwm);
  }
  else
  {
    ftm_pwm_duty(ftm0,ftm_ch5,0);              //�ı���ռ�ձ�
    ftm_pwm_duty(ftm0,ftm_ch2,(-EM_pwm));
  }
}

