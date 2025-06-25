#ifndef _MOTOR_CONTROL_H_
#define _MOTOR_CONTROL_H_

void Track_Line(void);
void Control_Speed(void);

extern int High_Speed;
extern int Max_Speed;      
extern int Min_Speed;      
extern int cutroute_speed;    
extern int cutroute_time;
extern int obstruct_speed;

#endif  