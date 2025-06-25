#include "headfile.h"
/******************************************************************************
圆环函数：
圆环识别 （稳定） 可能大圆环识别不了 或者识别不稳定 需要加强
入圆环 （稳定） 入圆环截止行 补线打角需优化
出圆环 （稳定） 出打角需优化

******************************************************************************/
int R_lost_cnt=0,L_lost_cnt=0,R_find_cnt=0,L_find_cnt=0,ALL_find_cnt=0;
int ring_start_L=Row-1,ring_start_R=Row-1,ring_end_L=Row-1,ring_end_R=Row-1,ring_xie_L=Row-1,ring_xie_R=Row-1;
int ring_start_L_Flag=0,ring_start_R_Flag=0,ring_end_L_Flag=0,ring_end_R_Flag=0,ring_xie_L_Flag=0,ring_xie_R_Flag=0;
int ring_cnt=0;
int ringflag=0;
int ring_tiaobian=0;
int ring_white=0,ring_white_di=0;
int ring_B_L=0,ring_B_R=0;
int smallbigflag=0;
int ring_previous=0;
int ring_time=0;


int ringstarttime=0;
void ring_start(void)
{
  if(ringflag==0)
    ringstarttime=0;
  if(ringflag==6)
  {
    ringstarttime++;
    ring_start_L=Row-1,ring_start_R=Row-1;
    //左起始点
    for(uint8 hang=hstart-2;hang>DropRow+2;hang--)
    {
      if(leftlineflag[hang]!=0&&leftlineflag[hang-1]==0&&leftlineflag[hang-2]==0)
      {
        ring_start_L=hang;
        break;
      }
    }
    if(ringstarttime<=20)
    {
      if(ring_start_L>28&&ring_start_L<50&&Error>=-10&&Error<=10)  //调接行数来控制打角  zuo
      {
        ringflag=1;
      }
    }
    else
      ringflag=0;
  }
  if(ringflag==7)
  {
    ringstarttime++;
    ring_start_L=Row-1,ring_start_R=Row-1;
    //右起始点
    for(uint8 hang=hstart-2;hang>DropRow+2;hang--)
    {
      if(rightlineflag[hang]!=0&&rightlineflag[hang-1]==0&&rightlineflag[hang-2]==0)
      {
        ring_start_R=hang;
        break;
      }
    }
    if(ringstarttime<=20)
    {
      if(ring_start_R>28&&ring_start_R<50&&Error>=-10&&Error<=10)  //调接行数来控制打角  you
      {
        ringflag=2;
      }
    }
    else
      ringflag=0;
  }
}
//圆环识别
void ring_find(void)
{
  R_lost_cnt=0,L_lost_cnt=0,ALL_find_cnt=0,R_find_cnt=0,L_find_cnt=0;
  ring_B_L=0,ring_B_R=0;
  ring_cnt=0;
  ring_start_L=Row-1,ring_start_R=Row-1,ring_end_L=Row-1,ring_end_R=Row-1,ring_xie_L=Row-1,ring_xie_R=Row-1;
  ring_start_L_Flag=0,ring_start_R_Flag=0,ring_end_L_Flag=0,ring_end_R_Flag=0,ring_xie_L_Flag=0,ring_xie_R_Flag=0;
  ring_tiaobian=0;
  ring_white=0,ring_white_di=0;

  //左起始点
  for(uint8 hang=Row-2;hang>DropRow+2;hang--)
  {
    if(leftlineflag[hang]!=0&&leftlineflag[hang-1]==0&&leftlineflag[hang-2]==0)
    {
      ring_start_L=hang;
      ring_start_L_Flag=1;
      break;
    }
  }
  //左终点
  if(ring_start_L_Flag==1)
  {
    //左终点
    for(uint8 hang=ring_start_L;hang>DropRow;hang--)
    {
      if(leftlineflag[hang]==0&&leftlineflag[hang-1]==0&&leftlineflag[hang-2]!=0)
      {
        ring_end_L=hang;
        ring_end_L_Flag=1;
        break;
      }
    }
    //左斜点
    for(uint8 hang=Row-2;hang>ring_start_L;hang--)
    {
      if(leftlineflag[hang]!=0&&leftlineflag[hang-1]!=0)
       if(leftline[hang]-leftline[hang-1]>1)
       {
         ring_xie_L=hang;
         ring_xie_L_Flag=1;
         break;
       }
    }
  }
  //右起始点
  for(uint8 hang=Row-2;hang>DropRow+2;hang--)
  {
    if(rightlineflag[hang]!=0&&rightlineflag[hang-1]==0&&rightlineflag[hang-2]==0)
    {
      ring_start_R=hang;
      ring_start_R_Flag=1;
      break;
    }
  }
  //右终点
  if(ring_start_R_Flag==1)
  {
    //右终点
    for(uint8 hang=ring_start_R;hang>DropRow;hang--)
    {
      if(rightlineflag[hang]==0&&rightlineflag[hang-1]==0&&rightlineflag[hang-2]!=0)
      {
        ring_end_R=hang;
        ring_end_R_Flag=1;
        break;
      }
    }
    //右斜点
    for(uint8 hang=Row-2;hang>ring_start_R;hang--)
    {
      if(rightlineflag[hang]!=0&&rightlineflag[hang-1]!=0)
       if(rightline[hang-1]-rightline[hang]>1)
       {
         ring_xie_R=hang;
         ring_xie_R_Flag=1;
         break;
       }
    }
  }
  //左圆环 三点都找到
  if(ring_start_L_Flag==1&&ring_end_L_Flag==1&&ring_xie_L_Flag==1&&ring_start_R_Flag==0&&ring_end_R_Flag==0&&ring_xie_R_Flag==0&&Error>-10&&Error<10&&DropRow<20)
  {
    //入口左右丢线 左全丢右全不丢
    for(uint8 hang=ring_start_L;hang>ring_end_L;hang--)
    {
      if(rightlineflag[hang]==0)
        R_lost_cnt++;
      if(leftlineflag[hang]==1)
        L_find_cnt++;
      //最底行白点数较少 防止斜入十字误判
      for(uint8 lie=110;lie<Col-1;lie++)
      {
        if(Pixle[hang][lie]==white)
          ring_white++;
      }
    }
    //最底行白点数较少 防止斜入十字误判
    for(uint8 hang=45;hang>40;hang--)
    {
      for(uint8 lie=110;lie<Col-1;lie++)
      {
        if(Pixle[hang][lie]==white)
          ring_white_di++;
      }
    }
    //截止行向上找全线 防止小弯误判
    for(uint8 hang=ring_end_L;hang>DropRow;hang--)
    {
      if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
        ALL_find_cnt++;
    }
    //找跳变 白黑黑白跳变 圆环第一特征
    for(uint8 hang=ring_xie_L;hang>DropRow+2;hang--)
    {
      if(Pixle[hang][leftline[ring_xie_L]/2]==black&&Pixle[hang-1][leftline[ring_xie_L]/2]==white)
        ring_tiaobian++;
      if(Pixle[hang][leftline[ring_xie_L]/2]==white&&Pixle[hang-1][leftline[ring_xie_L]/2]==black)
        ring_tiaobian++;
    }
    //判断条件找小环标志位
    if(ring_tiaobian>=2&&R_lost_cnt<=1&&L_find_cnt<=2&&ring_white<15&&ring_white_di<40&&ring_start_L-ring_end_L>=2&&ALL_find_cnt>=5)
    {
      for(uint8 hang=ring_start_L;hang<ring_start_L+8;hang++)
      {
        if((leftline[hang]-leftline[hang+1])<0)
          if((leftline[hang]-leftline[hang+1])<=(leftline[hang+1]-leftline[hang+2]))
            ring_cnt++;
      }
      ring_B_R=regression(DropRow,Row-2,rightline);
      if(ring_cnt>4&&ring_B_R<30)
      {
        ringflag=6;
        smallbigflag=1;
      } 
    }
  }
  //大圆环
  else if(ring_start_L_Flag==1&&ring_end_L_Flag==0&&ring_xie_L_Flag==1&&ring_start_R_Flag==0&&ring_end_R_Flag==0&&ring_xie_R_Flag==0&&Error>-10&&Error<10&&DropRow<20)
  {
    //入口左右丢线 左全丢右全不丢
    for(uint8 hang=ring_start_L;hang>ring_start_L-5;hang--)
    {
      if(rightlineflag[hang]==0)
        R_lost_cnt++;
      if(leftlineflag[hang]==1)
        L_find_cnt++;
      //最底行白点数较少 防止斜入十字误判
      for(uint8 lie=110;lie<Col-1;lie++)
      {
        if(Pixle[hang][lie]==white)
          ring_white++;
      }
    }
    //最底行白点数较少 防止斜入十字误判
    for(uint8 hang=45;hang>40;hang--)
    {
      for(uint8 lie=110;lie<Col-1;lie++)
      {
        if(Pixle[hang][lie]==white)
          ring_white_di++;
      }
    }
    //截止行向上找全线 防止小弯误判
    for(uint8 hang=DropRow+10;hang>DropRow;hang--)
    {
      if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
        ALL_find_cnt++;
    }
    //找跳变 白黑黑白跳变 圆环第一特征
    for(uint8 hang=ring_xie_L;hang>DropRow+2;hang--)
    {
      if(Pixle[hang][leftline[ring_xie_L]/2]==black&&Pixle[hang-1][leftline[ring_xie_L]/2]==white)
        ring_tiaobian++;
      if(Pixle[hang][leftline[ring_xie_L]/2]==white&&Pixle[hang-1][leftline[ring_xie_L]/2]==black)
        ring_tiaobian++;
    }
    //判断条件找小环标志位
    if(ring_tiaobian>=2&&R_lost_cnt<=1&&L_find_cnt<=2&&ring_white<15&&ring_white_di<40&&ALL_find_cnt>=3)
    {
      for(uint8 hang=ring_start_L;hang<ring_start_L+8;hang++)
      {
        if((leftline[hang]-leftline[hang+1])<0)
          if((leftline[hang]-leftline[hang+1])<=(leftline[hang+1]-leftline[hang+2]))
            ring_cnt++;
      }
      ring_B_R=regression(DropRow,Row-2,rightline);
      if(ring_cnt>4&&ring_B_R<30)
      {
        ringflag=6;
        smallbigflag=2;
      } 
    }
  } 
  
  //右圆环
  if(ring_start_L_Flag==0&&ring_end_L_Flag==0&&ring_xie_L_Flag==0&&ring_start_R_Flag==1&&ring_end_R_Flag==1&&ring_xie_R_Flag==1&&Error>-10&&Error<10&&DropRow<20)
  {
    //入口左右丢线 左全丢右全不丢
    for(uint8 hang=ring_start_R;hang>ring_start_R-5;hang--)
    {
      if(rightlineflag[hang]==1)
        R_find_cnt++;
      if(leftlineflag[hang]==0)
        L_lost_cnt++;
      for(uint8 lie=10;lie>0;lie--)
      {
        if(Pixle[hang][lie]==white)
          ring_white++;
      }
    }
    //最底行白点数较少 防止斜入十字误判
    for(uint8 hang=45;hang>40;hang--)
    {
      for(uint8 lie=10;lie>0;lie--)
      {
        if(Pixle[hang][lie]==white)
          ring_white_di++;
      }
    }
    //截止行向上找全线 防止小弯误判
    for(uint8 hang=ring_end_R;hang>DropRow;hang--)
    {
      if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
        ALL_find_cnt++;
    }
    //找跳变 白黑黑白跳变 圆环第一特征
    for(uint8 hang=ring_xie_R;hang>DropRow+2;hang--)
    {
      if(Pixle[hang][(Col+rightline[ring_xie_R])/2]==black&&Pixle[hang-1][(Col+rightline[ring_xie_R])/2]==white)
        ring_tiaobian++;
      if(Pixle[hang][(Col+rightline[ring_xie_R])/2]==white&&Pixle[hang-1][(Col+rightline[ring_xie_R])/2]==black)
        ring_tiaobian++;
    }
    //判断条件找小环标志位
    if(ring_tiaobian>=2&&R_find_cnt<=1&&L_lost_cnt<=2&&ring_white<15&&ring_white_di<40&&ring_start_R-ring_end_R>=2&&ALL_find_cnt>=5)
    {
      for(uint8 hang=ring_start_R;hang<ring_start_R+8;hang++)
      {
        if((rightline[hang+1]-rightline[hang])<0)
          if((rightline[hang+1]-rightline[hang])<=(rightline[hang+2]-rightline[hang+1]))
            ring_cnt++;
      }
      ring_B_L=regression(DropRow,Row-2,leftline);
      if(ring_cnt>4&&ring_B_L>-30)
      {
        ringflag=7;
        smallbigflag=1;
      }
    }
  }
  else if(ring_start_L_Flag==0&&ring_end_L_Flag==0&&ring_xie_L_Flag==0&&ring_start_R_Flag==1&&ring_end_R_Flag==0&&ring_xie_R_Flag==1&&Error>-10&&Error<10&&DropRow<20)
  {
    //入口左右丢线 左全丢右全不丢
    for(uint8 hang=ring_start_R;hang>ring_start_R-5;hang--)
    {
      if(rightlineflag[hang]==1)
        R_find_cnt++;
      if(leftlineflag[hang]==0)
        L_lost_cnt++;
      for(uint8 lie=10;lie>0;lie--)
      {
        if(Pixle[hang][lie]==white)
          ring_white++;
      }
    }
    //最底行白点数较少 防止斜入十字误判
    for(uint8 hang=45;hang>40;hang--)
    {
      for(uint8 lie=10;lie>0;lie--)
      {
        if(Pixle[hang][lie]==white)
          ring_white_di++;
      }
    }
    //截止行向上找全线 防止小弯误判
    for(uint8 hang=DropRow+10;hang>DropRow;hang--)
    {
      if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
        ALL_find_cnt++;
    }
    //找跳变 白黑黑白跳变 圆环第一特征
    for(uint8 hang=ring_xie_R;hang>DropRow+2;hang--)
    {
      if(Pixle[hang][(Col+rightline[ring_xie_R])/2]==black&&Pixle[hang-1][(Col+rightline[ring_xie_R])/2]==white)
        ring_tiaobian++;
      if(Pixle[hang][(Col+rightline[ring_xie_R])/2]==white&&Pixle[hang-1][(Col+rightline[ring_xie_R])/2]==black)
        ring_tiaobian++;
    }
    //判断条件找小环标志位
    if(ring_tiaobian>=2&&R_find_cnt<=1&&L_lost_cnt<=2&&ring_white<15&&ring_white_di<40&&ALL_find_cnt>=3)
    {
      for(uint8 hang=ring_start_R;hang<ring_start_R+8;hang++)
      {
        if((rightline[hang+1]-rightline[hang])<0)
          if((rightline[hang+1]-rightline[hang])<=(rightline[hang+2]-rightline[hang+1]))
            ring_cnt++;
      }
      ring_B_L=regression(DropRow,Row-2,leftline);
      if(ring_cnt>4&&ring_B_L>-30)
      {
        ringflag=7;
        smallbigflag=2;
      }
    }
  }
}
int ring_hang_R[Col],ring_hang_L[Col];
int ring_R=Col-1,ring_L=Col-1;
int xieflag=0;
//入圆环补线
void ring_ru(void)
{
  ring_R=Col/2,ring_L=Col/2;
  ring_end_L=Row-1,ring_end_L_Flag=0,ring_end_R=Row-1,ring_end_R_Flag=0;
  for(uint8 lie=0;lie<Col;lie++)
  {
    ring_hang_R[lie]=0;
    ring_hang_L[lie]=0;
  }
  
  //左圆环
  if(ringflag==1)
  {
    xieflag=0;
    //找终点
    for(uint8 hang=Row-2;hang>endline+3;hang--)
    {
      if(leftlineflag[hang]==0&&leftlineflag[hang-1]==0&&leftlineflag[hang-2]!=0)
      {
        ring_end_L=hang;
        ring_end_L_Flag=1;
        break;
      }
    }
    //截止行查找
    //找左边白点数
    int lost=0;
    for(uint8 hang=55;hang>45;hang--)
    {
      if(leftlineflag[hang]==0||leftline[hang]<5)
      {
        lost++;
      }
    }
    if(lost>=7)
    {
      //截止行标定 (可行度极高)
      for(uint8 hang=50;hang>endline;hang--)
      {
        if(Pixle[hang][20]==black&&Pixle[hang+1][20]==black)
        {
          endline=hang+2;
          break;
        }
      }
    }
    else
    {
      //截止行标定 (可行度极高)
      for(uint8 hang=50;hang>endline;hang--)
      {
        if(Pixle[hang][45]==black&&Pixle[hang+1][45]==black)
        {
          endline=hang+2;
          break;
        }
      }
    }
    //找拐点清标志位
    //原始图像列扫描 局部不正确有些点没找到 以后优化
    for(uint8 lie=119;lie>60;lie--)
    {
      for(uint8 hang=55;hang>endline;hang--)
      {
        if(Pixle[hang][lie]==black)
        {
          ring_hang_L[lie]=hang;
          break;
        }
      }
    }
    for(uint8 lie=119;lie>60;lie--)
    {
      if(ring_hang_L[119]==0&&ring_hang_L[118]==0)
        if(ring_hang_L[lie]!=0&&ring_hang_L[lie-1]!=0)
        {
          ring_L=lie;
          break;
        }
    }
    //连线
    rightline[endline]=(int)(80-endline*2);//90
    rightline[59]=119;
    connect(59,endline,rightline);
    for(uint8 hang=59;hang>endline;hang--)
    {
      rightlineflag[hang]=1;
      leftlineflag[hang]=0;
      if(leftlineflag[hang]==0)
      {
        leftlineflag[hang]=1;
        leftline[hang]=0;
      }
    }
    
    //清标志
    if(ring_L>90)
      ringflag=3;
  }
  //右圆环
  else if(ringflag==2)
  {    
    xieflag=0;
    for(uint8 hang=Row-2;hang>endline+3;hang--)
    {
      if(rightlineflag[hang]==0&&rightlineflag[hang-1]==0&&rightlineflag[hang-2]!=0)
      {
        ring_end_R=hang;
        ring_end_R_Flag=1;
        break;
      }
    }
    //截止行查找
    //找左边白点数
    int lost=0;
    for(uint8 hang=55;hang>45;hang--)
    {
      if(rightlineflag[hang]==0||rightline[hang]>Col-5)
      {
        lost++;
      }
    }
    if(lost>=7)
    {
      //截止行标定 (可行度极高)
      for(uint8 hang=50;hang>endline;hang--)
      {
        if(Pixle[hang][Col-20]==black&&Pixle[hang+1][Col-20]==black)
        {
          endline=hang+2;
          break;
        }
      }
    }
    else
    {
      //截止行标定 (可行度极高)
      for(uint8 hang=50;hang>endline;hang--)
      {
        if(Pixle[hang][Col-45]==black&&Pixle[hang+1][Col-45]==black)
        {
          endline=hang+2;
          break;
        }
      }
    }
    
    //找拐点清标志位
    //原始图像列扫描 局部不正确有些点没找到 以后优化
    for(uint8 lie=0;lie<60;lie++)
    {
      for(uint8 hang=55;hang>endline;hang--)
      {
        if(Pixle[hang][lie]==black)
        {
          ring_hang_R[lie]=hang;
          break;
        }
      }
    }
    for(uint8 lie=0;lie<60;lie++)
    {
      if(ring_hang_R[0]==0&&ring_hang_R[1]==0)
        if(ring_hang_R[lie]!=0&&ring_hang_R[lie+1]!=0)
        {
          ring_R=lie;
          break;
        }
    }
    //连线
    leftline[endline]=(int)(40+endline*2);//30
    leftline[59]=0;
    connect(59,endline,leftline);
    for(uint8 hang=59;hang>endline;hang--)
    {
      leftlineflag[hang]=1;
      rightlineflag[hang]=0;
      if(rightlineflag[hang]==0)
      {
        rightlineflag[hang]=1;
        rightline[hang]=Col-1;
      }
    }

    if(ring_R<30)
      ringflag=4;
  }
}
int ring_kong[Row];
int ring_chu_num=0;
void ring_chu(void)
{
  ring_xie_L=0,ring_xie_R=0;
  ring_chu_num=0;
  ring_end_L=Row-1,ring_end_R=Row-1;
  ring_end_L_Flag=0,ring_end_R_Flag=0;
  R_lost_cnt=0,L_lost_cnt=0,ALL_find_cnt=0,R_find_cnt=0,L_find_cnt=0;

  for(uint8 hang=0;hang<Row;hang++)
  {
    ring_kong[hang]=0;
  }
  //左圆环
  if(ringflag==3)
  { 
    //右线右拐
    for(uint8 hang=Row-2;hang>endline+1;hang--)
    {
        if(rightline[hang-1]-rightline[hang]>1)
          if(rightlineflag[hang-1]==1&&rightlineflag[hang]==1)
        {
          ring_xie_R=hang;
          xieflag=1;
          break;
        }
    }
    if((ring_xie_R==0||ring_xie_R>45)&&xieflag!=0)
    {
      for(uint8 hang=Row-5;hang>endline;hang--)
      {
        for(uint8 lie=0;lie<Col;lie++)
        {
          if(Pixle[hang][lie]==white&&(leftline[hang]<5||leftlineflag[hang]==0)&&(rightline[hang]>Col-5||rightlineflag[hang]==0))
            ring_kong[hang]++;
        }
        if(ring_kong[hang]>Col-5)
          ring_chu_num++;
      }
      if(ring_chu_num>5)
      {
        for(uint8 hang=Row-1;hang>endline;hang--)
        {
          leftline[hang]=0;
          rightline[hang]=110-endline;//110
          leftlineflag[hang]=1;
          rightlineflag[hang]=1;
        }
        xieflag=2;
      }
      else
      {
        if(DropRow<30)
        {
          int temp=0;
          for(uint8 hang=Row-2;hang>DropRow+2;hang--)
          {
            if(leftlineflag[hang]==0&&leftlineflag[hang-1]==0&&leftlineflag[hang-2]!=0)
            {
              ring_end_L=hang;
              ring_end_L_Flag=1;
              break;
            }
          }
          for(uint8 hang=ring_end_L-1;hang>ring_end_L-6;hang--)
          {
            if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
              ALL_find_cnt++;
          }
          temp=ring_end_L+6;
          if(temp>60) temp=60;
          for(uint8 hang=ring_end_L+1;hang<temp;hang++)
          {
            if(leftlineflag[hang]==1)
              L_find_cnt++;
            if(rightlineflag[hang]==0)
              R_lost_cnt++;
          }
          if(L_find_cnt<=2&&R_lost_cnt<=2&&ALL_find_cnt>=3)
          {
            if(smallbigflag==1&&xieflag==2)
            {
              ringflag=5;
              smallbigflag=0;
            }
            else if(smallbigflag==2&&xieflag==2)
            {
              ringflag=5;
              smallbigflag=0;
            }
          } 
          else 
          {
            ring_cnt=0;
            int num=0;
            for(uint8 hang=50;hang>endline;hang--)
            {
              if(rightlineflag[hang]==0)
                num++;
            }
            
            for(uint8 hang=40;hang<50;hang++)
            {
              if((leftline[hang]-leftline[hang+1])<0)
               if((leftline[hang]-leftline[hang+1])<=(leftline[hang+1]-leftline[hang+2]))
                 ring_cnt++;
            }
            if(ring_cnt>=4&&num<=3&&xieflag==2)
            {
              ringflag=5;
              smallbigflag=0;
            }
          }
        } 
      }
    }
    else
    {
      for(uint8 hang=ring_xie_R+5;hang>endline;hang--)
      {
        rightline[hang]=rightline[ring_xie_R+5]-1.2*(ring_xie_R+5-hang);
        rightlineflag[hang]=1;
        leftlineflag[hang]=1;
        leftline[hang]=0;
      }
    }
  }
  //右圆环
  else if(ringflag==4)
  {
    //左斜点
    for(uint8 hang=Row-2;hang>endline+1;hang--)
    {
      if(leftline[hang]-leftline[hang-1]>1)
        if(leftlineflag[hang]==1&&leftlineflag[hang-1]==1)
      {
        ring_xie_L=hang;
        xieflag=1;
        break;
      }
    }
    if((ring_xie_L==0||ring_xie_L>45)&&xieflag!=0)
    {
      for(uint8 hang=Row-5;hang>endline;hang--)
      {
        for(uint8 lie=0;lie<Col-1;lie++)
        {
          if(Pixle[hang][lie]==white&&(leftline[hang]<5||leftlineflag[hang]==0)&&(rightline[hang]>Col-5||rightlineflag[hang]==0))
            ring_kong[hang]++;
        }
        if(ring_kong[hang]>Col-5)
          ring_chu_num++;
      }
      if(ring_chu_num>5)
      {
        for(uint8 hang=Row-1;hang>endline;hang--)
        {
          leftline[hang]=10+endline;
          rightline[hang]=Col-1;
          leftlineflag[hang]=1;
          rightlineflag[hang]=1;
        }
        xieflag=2;
      }
      else
      {
        if(DropRow<30)//
        {
          int temp=0;
          for(uint8 hang=Row-2;hang>DropRow+2;hang--)
          {
            if(rightlineflag[hang]==0&&rightlineflag[hang-1]==0&&rightlineflag[hang-2]!=0)
            {
              ring_end_R=hang;
              ring_end_R_Flag=1;
              break;
            }
          }
          for(uint8 hang=ring_end_R-1;hang>ring_end_R-6;hang--)
          {
            if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
              ALL_find_cnt++;
          }
          temp=ring_end_R+6;
          if(temp>60) temp=60;
          for(uint8 hang=ring_end_R+1;hang<temp;hang++)
          {
            if(leftlineflag[hang]==0)
              L_lost_cnt++;
            if(rightlineflag[hang]==1)
              R_find_cnt++;
          }
          if(L_lost_cnt<=2&&R_find_cnt<=2&&ALL_find_cnt>=3)
          {
            if(smallbigflag==1&&xieflag==2)
            {
              ringflag=5;
              smallbigflag=0;
            }
            else if(smallbigflag==2&&xieflag==2)
            {
              ringflag=5;
              smallbigflag=0;
            }
          } 
          else 
          {
            ring_cnt=0;
            int num=0;
            for(uint8 hang=50;hang>endline;hang--)
            {
              if(leftlineflag[hang]==0)
                num++;
            }
            for(uint8 hang=40;hang<50;hang++)
            {
              if((rightline[hang+1]-rightline[hang])<0)
               if((rightline[hang+1]-rightline[hang])<=(rightline[hang+2]-rightline[hang+1]))
                 ring_cnt++;
            }
            if(ring_cnt>=4&&num<=3&&xieflag==2)
            {
              ringflag=5;
              smallbigflag=0;
            }
          }
        } 
      }   
    }
    else
    {
      for(uint8 hang=ring_xie_L+5;hang>endline;hang--)
      {
        leftline[hang]=leftline[ring_xie_L+5]+1.2*(ring_xie_L+5-hang);
        rightlineflag[hang]=1;
        leftlineflag[hang]=1;
        rightline[hang]=Col-1;
      }
    }
  }
  if(ringflag==5)
  {
    ring_time++;
    if(ring_time>25)
    {
      ringflag=0;
      ring_time=0;
    }
  }
}

