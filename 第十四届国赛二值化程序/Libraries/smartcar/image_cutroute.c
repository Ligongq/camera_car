#include "headfile.h"
/*********************************************************************
断路处理
E24 E25
电磁左右最大2000
**********************************************************************/
int cutrouteflag=0;
int cutrouteline[Col];
int cutroute_far_hang=60,cutroute_far_lie=0;
int cut_allfind=0;
int cut_lost_L=0,cut_lost_R=0;
int cut_start=0;
int cut_B_L=0,cut_B_R=0;
int cut_xie_L=0,cut_xie_R=0;
int cut_one=0,cut_two=0;
int cut_cnt=0;
void cutroute_find(void)
{
  //找顶点
  //竖向扫描
  //存列找行
  cutroute_far_hang=60,cutroute_far_lie=0;
  for(uint8 lie=0;lie<Col;lie++)
    cutrouteline[lie]=0;

  for(uint8 lie=0;lie<Col;lie++)
  {
    if(Pixle[59][lie]==white)
    {
      for(uint8 hang=59;hang>0;hang--)
      {
        if(Pixle[hang][lie]==black)
        {
          cutrouteline[lie]=hang;
          break;
        }
      }
    }
  }
  //存最远行和列
  for(uint8 lie=0;lie<Col;lie++)
  {
    if(cutroute_far_hang>cutrouteline[lie]&&cutrouteline[lie]!=0)
    {
      cutroute_far_hang=cutrouteline[lie];
      cutroute_far_lie=lie;
    }
  }
  
  //弯道断路
  cut_one=0;
  cut_two=0;
  cut_start=0;
  cut_B_L=0;
  cut_B_R=0;
  cut_xie_L=0;
  cut_xie_R=0;
  cut_allfind=0;
  //左斜点
  for(uint8 hang=Row-2;hang>cutroute_far_hang;hang--)
  {
    if(leftline[hang+1]-leftline[hang]>0&&rightline[hang+1]-rightline[hang]>0)
    {
      cut_xie_L=hang;
      break;
    }
  }
  //右斜点
  for(uint8 hang=Row-2;hang>cutroute_far_hang;hang--)
  {
    if(rightline[hang+1]-rightline[hang]<0&&leftline[hang+1]-leftline[hang]<0)
    {
      cut_xie_R=hang;
      break;
    }
  }
  //左弯
  if(cut_xie_L!=0&&cut_xie_R==0&&cutroute_far_hang>=15&&cutroute_far_lie>=20)
  {
    for(uint8 hang=cut_xie_L;hang>cutroute_far_hang;hang--)
    {
      if(leftline[hang]-leftline[hang+1]>0&&leftline[hang+1]-leftline[hang+2]>0)
      {
        cut_start=hang;
        break;
      }
    }
    //找到起始点
    if(cut_start!=0&&cut_start-cutroute_far_hang>=5)
    {
      cut_B_L=regression(cutroute_far_hang,cut_start,leftline);
      cut_B_R=regression(cutroute_far_hang,Row-1,rightline);
      for(uint8 hang=cut_start;hang>cut_start-10;hang--)
      {
        if(leftline[hang-1]>leftline[hang])
          cut_cnt++;
      }  
    }
    //找顶点向左白点数
    for(uint8 lie=0;lie<10;lie++)
    {
      if(Pixle[cutroute_far_hang][lie]==1)
        cut_one++;
      if(Pixle[cutroute_far_hang+1][lie]==1)
        cut_two++;
    }
    for(uint8 hang=cutroute_far_hang+10;hang>cutroute_far_hang;hang--)
    {
      if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
        cut_allfind++;
    } 
    //右边斜率正大 左边斜率负大
    if(cut_B_L<-100&&cut_B_R>40&&cut_one<=2&&cut_two<=2&&cut_cnt>=8&&cut_allfind>=8)
    {
      cutrouteflag=2;
    }
  }
  //右大弯
  //左弯
  else if(cut_xie_L==0&&cut_xie_R!=0&&cutroute_far_hang>=15&&cutroute_far_lie<=100)
  {
    for(uint8 hang=cut_xie_R;hang>cutroute_far_hang;hang--)
    {
      if(rightline[hang]-rightline[hang+1]<0&&rightline[hang+1]-rightline[hang+2]<0)
      {
        cut_start=hang;
        break;
      }
    }
    
    //找到起始点
    if(cut_start!=0&&cut_start-cutroute_far_hang>=5)
    {
      cut_B_R=regression(cutroute_far_hang,cut_start,rightline);
      cut_B_L=regression(cutroute_far_hang,Row-1,leftline);
      for(uint8 hang=cut_start;hang>cut_start-10;hang--)
      {
        if(rightline[hang]>rightline[hang-1])
          cut_cnt++;
      }
    }
    //找顶点向左白点数
    for(uint8 lie=110;lie<Col;lie++)
    {
      if(Pixle[cutroute_far_hang][lie]==1)
        cut_one++;
      if(Pixle[cutroute_far_hang+1][lie]==1)
        cut_two++;
    }
    for(uint8 hang=cutroute_far_hang+10;hang>cutroute_far_hang;hang--)
    {
      if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
        cut_allfind++;
    } 
    //右边斜率正大 左边斜率负大
    if(cut_B_R>100&&cut_B_L<-40&&cut_one<=2&&cut_two<=2&&cut_cnt>=8&&cut_allfind>=8)
    {
      cutrouteflag=3;
    }
  }
  
  cut_one=0;
  cut_two=0;
  //正常弯断路
  if(cutrouteflag!=2&&cutrouteflag!=3)
  {
    //左右都不丢
    for(uint8 hang=Row-1;hang>cutroute_far_hang;hang--)
    {
      if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
      {
        cut_allfind++;
      }
    }
    //防止误判 找截止行上的黑点
    for(uint8 lie=cutroute_far_lie-10;lie<cutroute_far_lie+10;lie++)
    {
      if(Pixle[cutroute_far_hang-2][lie]==black)
        cut_one++;
      if(Pixle[cutroute_far_hang-3][lie]==black)
        cut_two++;
    }
    //判断
    if(cutroute_far_hang>=25)
    {
      //判分弯道直道
      if(cut_allfind>=55-cutroute_far_hang&&cut_one>15&&cut_two>15)
      {
        cutrouteflag=1;
      }
    }
  }
}
int cut_time=0;
void cut_route_process(void)
{
  cutroute_far_hang=60,cutroute_far_lie=0;
  cut_one=0,cut_two=0;
  for(uint8 lie=0;lie<Col;lie++)
    cutrouteline[lie]=0;
  
  if(cutrouteflag==1)
  {
    if(cut_time<=100)
      cut_time++;
    for(uint8 lie=0;lie<Col;lie++)
    {
      if(Pixle[59][lie]==white)
        cut_one++;
      if(Pixle[57][lie]==white)
        cut_two++;
    }
    if(DropRow<40&&cut_one>90&&cut_two>80&&cut_two<cut_one&&cut_time>=20)
    {
      cutrouteflag=0;
      cut_time=0;
    }
  }
  //左大弯
  else if(cutrouteflag==2)
  {
    //竖向扫描
    //存列找行
    for(uint8 lie=0;lie<Col;lie++)
    {
      if(Pixle[59][lie]==white)
      {
        for(uint8 hang=59;hang>0;hang--)
        {
          if(Pixle[hang][lie]==black)
          {
            cutrouteline[lie]=hang;
            break;
          }
        }
      }
    }
    //存最远行和列
    for(uint8 lie=0;lie<Col;lie++)
    {
      if(cutroute_far_hang>cutrouteline[lie]&&cutrouteline[lie]!=0)
      {
        cutroute_far_hang=cutrouteline[lie];
        cutroute_far_lie=lie;
      }
    }
    //根据endline打死
    for(uint8 hang=Row-1;hang>endline;hang--)
    {
      leftlineflag[hang]=1;
      rightlineflag[hang]=1;
      leftline[hang]=rightline[hang]-track_width[hang];
    }
    if(cutroute_far_lie>60)
      cutrouteflag=1;
    else 
    {
      if(DropRow>50)
        cutrouteflag=1;
    }
  }
  else if(cutrouteflag==3)
  {
    //竖向扫描
    //存列找行
    for(uint8 lie=0;lie<Col;lie++)
    {
      if(Pixle[59][lie]==white)
      {
        for(uint8 hang=59;hang>0;hang--)
        {
          if(Pixle[hang][lie]==black)
          {
            cutrouteline[lie]=hang;
            break;
          }
        }
      }
    }
    //存最远行和列
    for(uint8 lie=0;lie<Col;lie++)
    {
      if(cutroute_far_hang>cutrouteline[lie]&&cutrouteline[lie]!=0)
      {
        cutroute_far_hang=cutrouteline[lie];
        cutroute_far_lie=lie;
      }
    }
    //根据endline打死
    for(uint8 hang=Row-1;hang>endline;hang--)
    {
      leftlineflag[hang]=1;
      rightlineflag[hang]=1;
      rightline[hang]=leftline[hang]+track_width[hang];
    }
    if(cutroute_far_lie<60)
      cutrouteflag=1;
    else 
    {
      if(DropRow>50)
        cutrouteflag=1;
    }
  }
}

void adc_port_init(void)
{
  //左
  adc_init(ADC0_SE18);
  //左中
  adc_init(ADC0_SE17);
  //右中
  adc_init(ADC0_SE12);
  //右
  adc_init(ADC0_SE13);
}
int zuo=0;
int you=0;
int outflag=0;
int midzuo=0;
int midyou=0;
void get_adc_error(void)
{
  zuo=0;
  you=0;
  midyou=0;
  midzuo=0;
  for(uint8 i=0;i<20;i++)
  {
    zuo+=adc_once(ADC0_SE18,ADC_12bit);
    midzuo+=adc_once(ADC0_SE17,ADC_12bit);
    midyou+=adc_once(ADC0_SE12,ADC_12bit);
    you+=adc_once(ADC0_SE13,ADC_12bit);
  }
  zuo=zuo/20;
  midzuo=midzuo/20;
  midyou=midyou/20;
  you=you/20;
 
}

