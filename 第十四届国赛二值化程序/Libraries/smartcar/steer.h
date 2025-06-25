#ifndef _STEER_H_
#define _STEER_H_

void get_deviation(void);
void DM_Control(void);
void differential_control(void);
void differential_control_ele(void);

extern float Error;

#endif 