#ifndef _PID_H
#define _PID_H
#include "headfile.h"

extern float kp;
extern float kd;
extern float out;
extern uint16 zhongzhi;

float pidinit(float a);
uint16 pid_sudu_lift(uint16 a,uint16 c,uint16 b);
uint16 pid_sudu2_right(uint16 a,uint16 c,uint16 b);   //设定 实际 初值
void seep_pid(int seep_sj);
#endif






