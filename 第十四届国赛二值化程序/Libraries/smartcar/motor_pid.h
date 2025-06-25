#ifndef _MOTOR_PID_H_
#define _MOTOR_PID_H_

void FTM_Init(void);
void Speed_Get(void);
void EM_Control_zuo(void);
void EM_Control_you(void);

extern int set_speed_zuo;
extern int set_speed_you;

extern int get_speed_zuo;
extern int get_speed_you;

extern float distance_obstruct;
extern float distance_scratch;
#endif  