#ifndef _IMAGE_RING_H_
#define _IMAGE_RING_H_

void ring_find(void);
void ring_ru(void);
void ring_chu(void);

float threexielv_right(int one,int two,int three);
float threexielv_left(int one,int two,int three);
void ring_previous_find(void);

extern int ringflag;
extern int right_B;
extern int smallbigflag;
extern int ring_previous;
void ring_start(void);

#endif 