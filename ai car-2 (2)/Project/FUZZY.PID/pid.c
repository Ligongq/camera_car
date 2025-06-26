#include "headfile.h"

float kp=0.0;//0.6
float ki=0;
float kd=0.0;//2.50
float out=0;

float ec;
/*舵机比例微分控制*/
float k=0;
extern float donngtaiwucha;

extern int sdlift,sdright;
float pidinit(float a)
{
    static float error=0;
    static float error_1=0;
    error=a;
    ec=error-error_1;
/*************************动态p*********************/
/*	error=donngtaiwucha*(donngtaiwucha*donngtaiwucha/1250.0+2)/10;*/
/***************************************************/
/************************模糊pd********************/
	seep_pid((sdlift+sdright)/2);
	kp=Fuzzy_Kp((int)error,(int)ec)+k;
	kd=Fuzzy_Kd((int)error,(int)ec);
/***************************************************/	
    out = kp*error+kd*ec;
    error_1=error;
    return out;  
}



/*电机比例控制*/
float kp1=0;//0.6
float ki1=0.0;
float out1=0;
float n=0.12,n22=0.0;
uint16 pid_sudu_lift(uint16 a,uint16 c,uint16 b)  //设定 实际 初值
{
    static float pre_error=0;
	static float pre_d_error=0;
	static float d_error=0,dd_error=0;
	static float error=0;	
	error = a - c ;    //速度差值 (给定值 - 车实际速度)
	d_error = error - pre_error;
	dd_error = d_error - pre_d_error;
	pre_error = error; //存储当前偏差
	pre_d_error = d_error;
	kp1 = error*error*n;//二次p
	ki1 = error*error*n22;
	out1 += kp1*d_error+ki1*error;
	if(out1>=50000)  //结果限幅
	{
		out1=50000;
	}
	else if(out1<=0)
	{
		out1=0;
	}
    return (uint16)out1;  
}

/*电机比例控制*/
float kp2=0;//0.6
float ki2=0;
float out2=0;
float n1=0.12,n11=0.0;
uint16 pid_sudu2_right(uint16 a,uint16 c,uint16 b)  //设定 实际 初值
{
    static float pre_error=0;
	static float pre_d_error=0;
	static float d_error=0,dd_error=0;
	static float error=0;	
	error = a - c ;    //速度差值 (给定值 - 车实际速度)
	d_error = error - pre_error;
	dd_error = d_error - pre_d_error;
	pre_error = error; //存储当前偏差
	pre_d_error = d_error;
	kp2 = error*error*n1;  //二次p
	ki2 = error*error*n11;
	out2 += kp2*d_error+ki2*error;
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


/*速度与模糊pd参数选取函数*/
extern int16 DMAX,DMIN;    //500
extern float UFF_D[7],UFF_P[7];
extern float kpdj;
extern int16 duojijiaodu;
void seep_pid(int seep_sj)
{
	if(seep_sj>=40)
	{
		kpdj=0.03;
		DMAX=120;
		DMIN=-120;	
				
		UFF_P[0]=0.09;
		UFF_P[1]=0.12;
		UFF_P[2]=0.15;
		UFF_P[3]=0.18;
		UFF_P[4]=0.21;
		UFF_P[5]=0.24;
		UFF_P[6]=0.27;	

		UFF_D[0]=0.4;
		UFF_D[1]=0.9;
		UFF_D[2]=1.4;
		UFF_D[3]=1.9;
		UFF_D[4]=2.4;
		UFF_D[5]=2.9;
		UFF_D[6]=3.4;			
	}
	else if(seep_sj<40)
	{
		kpdj=0.03;		
		DMAX=500;
		DMIN=-500;	
				
		UFF_P[0]=0.1;
		UFF_P[1]=0.13;
		UFF_P[2]=0.16;
		UFF_P[3]=0.19;
		UFF_P[4]=0.22;
		UFF_P[5]=0.25;
		UFF_P[6]=0.28;	

		UFF_D[0]=0.05;
		UFF_D[1]=0.08;
		UFF_D[2]=0.11;
		UFF_D[3]=0.14;
		UFF_D[4]=0.17;
		UFF_D[5]=0.2;
		UFF_D[6]=0.23;
	}
}
