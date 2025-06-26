#include "headfile.h"

extern char ai;
float kp=3;//0.6
float kd=800;//2.50
float out=0;
float ec;
/*�������΢�ֿ���*/
float k=0;
extern float dongtaiwucha;
extern int sdlift,sdright;
float pidinit()
{
    static float error=0;
    static float error_1=0;
	if(ai==0)
	{
		kp=3;
		kd=420;
	}
	else if(ai==1)
	{
		kp=35;
		kd=520;
	}
	error=dongtaiwucha*(dongtaiwucha*dongtaiwucha/1250.0+2)/10;
	ec=error-error_1;
    out = kp*error+kd*ec;
    error_1=error;
    return out;  
}
extern uint16 seep_lift;		//��������趨�ٶ�
extern uint16 seep_right;		//�����趨
float kpdj=0.5,outdj=0;  		//kpdj:������������� outdj:���������������
extern int16 smotor_angle1;		//����Ƕ�
extern int16 zhidaosudu;		//��ģֱ�������ٶ�

void chashu()
{
	static float error=0;
	error=smotor_angle1;
	outdj=kpdj*error;
	if(outdj>=sudu)				//����ֵ�޷� suduΪ�������õĺ궨�� #define sudu  	40	
	{
		outdj=sudu;
	}
	else if(outdj<=-sudu)
	{
		outdj=-sudu;
	}	
	seep_lift=sudu-outdj+zhidaosudu;	//�����趨�ٶ�
	seep_right=sudu+outdj+zhidaosudu;
}

/*�����������*/
float kp1=0;//0.6
float ki1=0.0;
float out1=0;
float n=0.0,n22=0.015;
uint16 pid_sudu_lift(uint16 a,uint16 c,uint16 b)  //�趨 ʵ�� ��ֵ
{
    static float pre_error=0;
	static float pre_d_error=0;
	static float d_error=0,dd_error=0;
	static float error=0;	
	error = a - c ;    //�ٶȲ�ֵ (����ֵ - ��ʵ���ٶ�)
	ki1 = error*error*n22;
	out1 += ki1*error;
	if(ki1>=500)
	{
		ki1=500;
	}
	if(out1>=50000)
	{
		out1=50000;
	}
	else if(out1<=0)
	{
		out1=0;
	}
    return (uint16)out1;  
}

/*�����������*/
float kp2=0;//0.6
float ki2=0;
float out2=0;
float n1=0.0,n11=0.015;
uint16 pid_sudu2_right(uint16 a,uint16 c,uint16 b)  //�趨 ʵ�� ��ֵ
{
    static float pre_error=0;
	static float pre_d_error=0;
	static float d_error=0,dd_error=0;
	static float error=0;	
	error = a - c ;    //�ٶȲ�ֵ (����ֵ - ��ʵ���ٶ�)
	ki2 = error*error*n11;
	out2 += ki2*error;
	if(ki2>=500)
	{
		ki2=500;
	}	
	if(out2>=50000)
	{
		out2=50000;
	}
	else if(out2<=0)
	{
		out2=0;
	}
    return (uint16)out2;  
}


