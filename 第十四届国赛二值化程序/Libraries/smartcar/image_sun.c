#include "headfile.h"


int sun_track_width[Row]={5,6,7,8,9,10,11,16,19,20,
                      23,25,26,28,30,32,34,36,38,40,
                      43,44,46,48,50,52,54,55,57,58,
                      60,63,65,67,68,70,71,73,75,77,
                      79,81,83,85,87,88,90,92,94,96,
                      98,99,100,102,104,106,107,109,110,110};

//图像腐蚀算法
void corrosion(void)
{
  int point1=0,point2=0,point3=0,point4=0;
  for(uint8 hang=0;hang<Row;hang++)
  {
    //左腐蚀
    point1=0,point2=0;
    for(uint8 lie=0;lie<Col-1;lie++)
    {
      if(Pixle[hang][lie]==0&&Pixle[hang][lie+1]==1)
      {
        point1=lie;
        break;
      }
    }
    if(point1!=0)
    {
      for(uint8 lie=point1;lie<Col-1;lie++)
      {
        if(Pixle[hang][lie]==1&&Pixle[hang][lie+1]==0)
        {
          point2=lie+1;
          break;
        }
      }
    }
    if(point2!=0)
    {
      if(point2-point1<30)
      {
        for(uint8 lie=point1;lie<point2;lie++)
          Pixle[hang][lie]=0;
      }
    }
    //右腐蚀
    point3=0,point4=0;
    for(uint8 lie=Col-1;lie>1;lie--)
    {
      if(Pixle[hang][lie]==0&&Pixle[hang][lie-1]==1)
      {
        point3=lie;
        break;
      }
    }
    if(point3!=0)
    {
      for(uint8 lie=point3;lie>1;lie--)
      {
        if(Pixle[hang][lie]==1&&Pixle[hang][lie-1]==0)
        {
          point4=lie-1;
          break;
        }
      }
    }
    if(point4!=0)
    {
      if(point3-point4<30)
      {
        for(uint8 lie=point4;lie<point3;lie++)
          Pixle[hang][lie]=0;
      }
    }
    
  }
}


int sun_cnt=0;
int sunflag=0;
int sunstep=0;
int sun_leftlineflag[Row],sun_rightlineflag[Row],sun_leftline[Row],sun_rightline[Row];
//阳光处理
void sunshine(void)
{
  
  sun_cnt=0;
  sunflag=0;
  
  //复制
  for(uint8 hang=59;hang>endline;hang--)
  {
    sun_leftline[hang]=leftline[hang];
    sun_rightline[hang]=rightline[hang];
    sun_rightlineflag[hang]=rightlineflag[hang];
    sun_leftlineflag[hang]=leftlineflag[hang];
  }
  
  //找线  
  for(uint8 hang=59;hang>endline;hang--)
  {
    //右丢左不丢 左向右找 右边线重新找
    if(sun_leftlineflag[hang]==1&&sun_rightlineflag[hang]==0)
    {
      for(uint8 lie=sun_leftline[hang];lie<Col-1;lie++)
      {
        if(Pixle[hang][lie]==1&&Pixle[hang][lie+1]==0)
        {
          sun_rightline[hang]=lie;
          sun_rightlineflag[hang]=1;
          break;
        }
      }
    }
    //左丢右不丢 右找左 左边线重新找
    else if(sun_leftlineflag[hang]==0&&sun_rightlineflag[hang]==1)
    {
      for(uint8 lie=sun_rightline[hang];lie>1;lie--)
      {
        if(Pixle[hang][lie]==1&&Pixle[hang][lie-1]==0)
        {
          sun_leftline[hang]=lie;
          sun_leftlineflag[hang]=1;
          break;
        }
      }
    }
    //左右都不丢  一边找对  一边不对情况  用赛道宽度
    else if(sun_leftlineflag[hang]==1&&sun_rightlineflag[hang]==1)
    {
      for(uint8 lie=leftline[hang];lie<Col-1;lie++)
      {
        if(Pixle[hang][lie]==1&&Pixle[hang][lie+1]==0)
        {
          sun_rightline[hang]=lie;
          sun_rightlineflag[hang]=1;
          break;
        }
      }
      for(uint8 lie=rightline[hang];lie>1;lie--)
      {
        if(Pixle[hang][lie]==1&&Pixle[hang][lie-1]==0)
        {
          sun_leftline[hang]=lie;
          sun_leftlineflag[hang]=1;
          break;
        }
      }
    }
  }
  
//  for(uint8 hang=58;hang>endline;hang--)
//  {
//    if(hang>40)
//      sunstep=sun_track_width[hang]-20;
//    else if(hang<=40&&hang>30)
//      sunstep=sun_track_width[hang]-15;
//    else if(hang<=30&&hang>20)
//      sunstep=sun_track_width[hang]-10;
//    else if(hang<=20&&hang>10)
//      sunstep=sun_track_width[hang]-10;
//    else if(hang<=10)
//      sunstep=5;
//    
//    if(sun_leftlineflag[hang]==1&&sun_rightlineflag[hang]==1)
//      if(sun_rightline[hang]>sun_leftline[hang])
//        if(sun_rightline[hang]-sun_leftline[hang]<sunstep)
//        {
//          leftlineflag[hang]=1;
//          rightlineflag[hang]=1;
//          leftline[hang]=leftline[hang+1];
//          rightline[hang]=rightline[hang+1];
//          sunflag=1;
//        }
//  }
//  
//  if(sun_cnt>=3)
//    sunflag=1;
}