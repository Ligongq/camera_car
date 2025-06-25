#include "headfile.h"
/*********************************************************************
障碍处理
超声波判断 稳定
zuo 0
you 1

C0 普通gpio置1 
C1 ADC采集 adc15

坡道判断  赛道变宽 斜率变小 延时清标志  时间微调

障碍 普通情况  偏差范围 坡道误判用五个点为黑判断
固定打角 出障碍无电感线 入有电感线 走一段距离清
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

//距离获取滤波
void distance_get(void)
{
  //存数据
  distance=0;
  for(uint8 hang=0;hang<20;hang++)
    distance+=adc_once(ADC0_SE15,ADC_10bit);
 
  distance=distance/20;
}
//普通障碍寻找
void obstruct_find_plain(void)
{
  distance_get();
  //横断判断
  if(Error<=10&&Error>=-10&&Pixle[15][60]==0&&Pixle[17][50]==0&&Pixle[17][70]==0&&Pixle[13][50]==0&&Pixle[13][70]==0)
  {
    if(distance>=200&&distance<=800&&reobstruct==0)
    {
      obstructflag=1;
//      reobstruct=1;
    }
  }
}
//障碍处理
void obstruct_deal(void)
{
  if(obstruct_direction==0)
  {
    //入障碍
    if(obstructflag==1)
    {
      if(zuo<100&&you<100)
        obstructflag=2;
    }
    //出障碍
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
    //入障碍
    if(obstructflag==1)
    {
      if(zuo<100&&you<100)
        obstructflag=2;
    }
    //出障碍
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

//坡道识别
void ramp_find(void)
{
  ramp_cnt=0;
  //直道
  if(endline<20)
  {
    for(uint8 hang=20;hang<30;hang++)
    {
      if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
       if(leftline[hang] >= leftline[hang+2]&&rightline[hang] <= rightline[hang+2]&&leftline[hang]>leftline[55]&&rightline[hang]<rightline[55])
       {
         //赛道变宽
         if(rightline[hang]-leftline[hang]>track_width[hang]+5)
           ramp_cnt++;
       }
    }
    //斜率变小
    ramp_B_L=regression(20,30,leftline);
    ramp_B_R=regression(20,30,rightline);
  }
  
  if(ramp_cnt==10&&Error<=15&&Error>=-15&&ramp_B_L<=0&&ramp_B_L>=-10&&ramp_B_R>=0&&ramp_B_R<=10)
  {
    rampflag=1;
  }
}
//坡道清标
void ramp_deal(void)
{
  if(rampflag==0)
    ramp_time=0;

  if(rampflag==1)
  {
    ramp_time++;
    //延时清
    if(ramp_time>=ramptime)  //40
      rampflag=0;
  }
}

