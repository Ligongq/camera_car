#include "headfile.h"
/******************************************************************************
十字处理 
改良版十字 简化了思路
入十字，斜入十字不进行补线，通过找拐点，然后用拐点以下部分。
出十字通过找正常赛道赋值
2019/8/21
******************************************************************************/
//记录
int SZ_start_L=Row-1,SZ_start_R=Row-1,SZ_xie_L=Row-1,SZ_xie_R=Row-1,SZ_end_L=Row-1,SZ_end_R=Row-1;
//记标志
int SZ_start_L_Flag=0,SZ_start_R_Flag=0,SZ_xie_L_Flag=0,SZ_xie_R_Flag=0,SZ_end_L_Flag=0,SZ_end_R_Flag=0;
int ru_cross_flag=0,xie_cross_flag=0,chu_cross_flag=0;
int cross_white[Row];
int ru_num=0;
int ru_point=0;
int SZ_xie_point=0;

//三种情况判断
void cross_find(void)
{
  SZ_xie_L_Flag=0,SZ_xie_R_Flag=0;
  SZ_xie_L=Row-1,SZ_xie_R=Row-1;
  chu_cross_flag=0,xie_cross_flag=0,ru_cross_flag=0;
  ru_num=0;
  ru_point=59;
  for(uint8 hang=0;hang<Row;hang++)
  {
    cross_white[hang]=0;
  }
  
  //右斜点
  for(uint8 hang=Row-2;hang>DropRow+1;hang--)
  {
    if(leftline[hang]<5&&rightlineflag[hang]!=0&&rightlineflag[hang-1]!=0)
      if(rightline[hang-1]-rightline[hang]>1)
        if(rightlineflag[hang+2]!=0&&rightlineflag[hang+3]!=0&&rightlineflag[hang+4]!=0)
          if(rightline[hang+3]-rightline[hang]>0&&rightline[hang+3]-rightline[hang+2]>0&&rightline[hang+4]-rightline[hang+3]>0)
      {
        SZ_xie_R=hang;
        SZ_xie_R_Flag=1;
        break;
      }
  }
  //左斜点
  for(uint8 hang=Row-5;hang>DropRow+1;hang--)
  {
    if(leftlineflag[hang]!=0&&rightline[hang]>Col-6&&leftlineflag[hang-1]!=0)
      if(leftline[hang]-leftline[hang-1]>1)
        if(leftlineflag[hang+2]!=0&&leftlineflag[hang+3]!=0&&leftlineflag[hang+4]!=0)
          if(leftline[hang+2]-leftline[hang+3]>0&&leftline[hang+3]-leftline[hang+4]>0&&leftline[hang]-leftline[hang+3]>0)
      {
        SZ_xie_L=hang;
        SZ_xie_L_Flag=1;
        break;
      }
  }
  //存白点
  for(uint8 hang=Row-1;hang>DropRow;hang--)
  {
    for(uint8 lie=0;lie<Col;lie++)
    {
      if(Pixle[hang][lie]==white)
      {
        cross_white[hang]++;
      }
    }
    if(hang<50)
    {
      if(cross_white[hang]>110)
        ru_num++;
    }
  }
  //入十字起始行
  for(uint8 hang=50;hang>DropRow;hang--)
  {
    if(cross_white[hang]>110)
    {
      ru_point=hang;
      break;
    }
  }
  //底下三行全白 出十字
  if(leftlineflag[50]==0&&rightlineflag[50]==0&&leftlineflag[51]==0&&rightlineflag[51]==0&&leftlineflag[52]==0&&rightlineflag[52]==0&&endline<30)
  {
    chu_cross_flag=1;
  }
  //斜入判断
  if((SZ_xie_R_Flag==1&&SZ_xie_R>20&&chu_cross_flag==0)||(SZ_xie_L_Flag==1&&SZ_xie_L>20&&chu_cross_flag==0))
  {
    xie_cross_flag=1;
  }
  //入十字
  if(ru_num>=10&&ru_point<50&&xie_cross_flag==0&&chu_cross_flag==0&&endline<30)
  {
    ru_cross_flag=1;
  }
}
void cross_deal(void)
{
  SZ_end_L=Row-1,SZ_end_R=Row-1;
  SZ_end_L_Flag=0,SZ_end_R_Flag=0;
  SZ_xie_point=0;
  
  //入十字
  if(ru_cross_flag==1)
  {
    for(uint8 hang=Row-1;hang>DropRow;hang--)
    {
      if(leftlineflag[hang]==0)
      {
        leftline[hang]=1;
        leftlineflag[hang]=1;
      }
      if(rightlineflag[hang]==0)
      {
        rightline[hang]=Col-1;
        rightlineflag[hang]=1;
      }
    }
  }
  //斜入
   if(SZ_xie_R_Flag==1)
   {
     SZ_xie_point=SZ_xie_R;
   }
   if(SZ_xie_L_Flag==1)
   {
     SZ_xie_point=SZ_xie_L;
   }
  //出十字补线
  if(chu_cross_flag==1)
  {
    //找十字终点
    for(uint8 hang=Row-2;hang>endline+1;hang--)
    {
      if(leftlineflag[hang]==0&&leftlineflag[hang-1]!=0&&leftlineflag[hang-2]!=0)
      {
        SZ_end_L=hang-2;
        SZ_end_L_Flag=1;
        break;
      }
    }
    for(uint8 hang=Row-2;hang>endline+1;hang--)
    {
      if(rightlineflag[hang]==0&&rightlineflag[hang-1]!=0&&rightlineflag[hang-2]!=0)
      {
        SZ_end_R=hang-2;
        SZ_end_R_Flag=1;
        break;
      }
    }
    //如果找到左右终点
    if(SZ_end_L_Flag==1&&SZ_end_R_Flag==1)
    {
      //更为准确
      if(SZ_end_L>SZ_end_R)
        SZ_end_L=SZ_end_R;
      else if(SZ_end_L<SZ_end_R)
        SZ_end_R=SZ_end_L;
      //增加准确度
      SZ_end_L-=5;
      SZ_end_R-=5;
      //补线
      for(uint8 hang=SZ_end_L;hang<Row;hang++)
      {
        leftline[hang]=leftline[SZ_end_L];
        leftlineflag[hang]=2;
      }
      for(uint8 hang=SZ_end_R;hang<Row;hang++)
      {
        rightline[hang]=rightline[SZ_end_R];
        rightlineflag[hang]=2;
      }
    }
  }
}
