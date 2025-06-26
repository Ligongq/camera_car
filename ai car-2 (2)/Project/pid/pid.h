#ifndef _PID_H
#define _PID_H
#include "headfile.h"

uint16 pid_sudu_lift(uint16 a,uint16 c,uint16 b);
uint16 pid_sudu2_right(uint16 a,uint16 c,uint16 b);   //设定 实际 初值
void chashu(void);
float pidinit(void);
void seep_pid(int seep_sj);
#endif






