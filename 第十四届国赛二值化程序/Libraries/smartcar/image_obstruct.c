#include "headfile.h"
/*********************************************************************
�ϰ�����
�������ж� �ȶ�
zuo 0
you 1

C0 ��ͨgpio��1 
C1 ADC�ɼ� adc15

�µ��ж�  ������� б�ʱ�С ��ʱ���־  ʱ��΢��

�ϰ� ��ͨ���  ƫ�Χ �µ������������Ϊ���ж�
�̶���� ���ϰ��޵���� ���е���� ��һ�ξ�����
**********************************************************************/
int obstructflag=0;
int distance=0;
int obstruct_direction=1;

int ramp_cnt=0;
int rampflag=0;
int ramp_time=0;
int ramp_B_L=0,ramp_B_R=0;
 
int reobstruct=0;

#define ramptime 50

//�����ȡ�˲�
void distance_get(void)
{
  //������
  distance=0;
  for(uint8 hang=0;hang<20;hang++)
    distance+=adc_once(ADC0_SE15,ADC_10bit);
 
  distance=distance/20;
}
//��ͨ�ϰ�Ѱ��
void obstruct_find_plain(void)
{
  distance_get();
  //����ж�
  if(Error<=10&&Error>=-10&&Pixle[15][60]==0&&Pixle[17][50]==0&&Pixle[17][70]==0&&Pixle[13][50]==0&&Pixle[13][70]==0)
  {
    if(distance>=200&&distance<=800&&reobstruct==0)
    {
      obstructflag=1;
//      reobstruct=1;
    }
  }
}
//�ϰ�����
void obstruct_deal(void)
{
  if(obstruct_direction==0)
  {
    //���ϰ�
    if(obstructflag==1)
    {
      if(zuo<100&&you<100)
        obstructflag=2;
    }
    //���ϰ�
    else if(obstructflag==2)
    {
      if((zuo<100&&you>200))  //100
        obstructflag=3;
    }
    else if(obstructflag==3)
    {
      if(distance_obstruct>200)
      {
        obstructflag=0;
        distance_obstruct=0;
      }
        
    } 
  }
  else if(obstruct_direction==1)
  {
    //���ϰ�
    if(obstructflag==1)
    {
      if(zuo<100&&you<100)
        obstructflag=2;
    }
    //���ϰ�
    else if(obstructflag==2)
    {
      if((zuo>200&&you<100))  //100
        obstructflag=3;
    }
    else if(obstructflag==3)
    {
      if(distance_obstruct>200)
      {
        obstructflag=0;
        distance_obstruct=0;
      }
    } 
  }
}

//�µ�ʶ��
void ramp_find(void)
{
  ramp_cnt=0;
  //ֱ��
  if(endline<20)
  {
    for(uint8 hang=20;hang<30;hang++)
    {
      if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
       if(leftline[hang] >= leftline[hang+2]&&rightline[hang] <= rightline[hang+2]&&leftline[hang]>leftline[55]&&rightline[hang]<rightline[55])
       {
         //�������
         if(rightline[hang]-leftline[hang]>track_width[hang]+5)
           ramp_cnt++;
       }
    }
    //б�ʱ�С
    ramp_B_L=regression(20,30,leftline);
    ramp_B_R=regression(20,30,rightline);
  }
  
  if(ramp_cnt==10&&Error<=15&&Error>=-15&&ramp_B_L<=0&&ramp_B_L>=-10&&ramp_B_R>=0&&ramp_B_R<=10)
  {
    rampflag=1;
  }
}
//�µ����
void ramp_deal(void)
{
  if(rampflag==0)
    ramp_time=0;

  if(rampflag==1)
  {
    ramp_time++;
    //��ʱ��
    if(ramp_time>=ramptime)  //40
      rampflag=0;
  }
}

