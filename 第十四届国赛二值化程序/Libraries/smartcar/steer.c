#include "headfile.h"

/**********************************************
舵机处理函数
偏差提取 加权平均
模糊pid控制
***********************************************/
//舵机限幅参数
#define MID     875    
#define RIGHT   730  //730   
#define LEFT    1020   
//赛道中值
#define basic 61  //61
//差速力度
float error_p=0.5; //*********************************
//正常加权
float Weight[60]={ 
                     0,0,0,0,0,0,0,0,0,0,           //0-9行，基本用不到
                    
                     0,0,0,0,0,0,0,0,0,0,            //0-19行，基本用不到
                    
                     5,5,8,7,8,9,8,8,9,9,        //20-29行
                    
                     6,7,7,6,5,4,4,5,5,4,            //30-39行
                  
                     2,2,2,2,2,1,1,1,1,1,           //40-49行
                     
                     0,0,0,0,0,0,0,0,0,0,           //最近十行   
                
                  };  
//十字加权
float CrossWeight[60]={ 
                   
                     0,0,0,0,0,0,0,0,0,0,           //0-9行，基本用不到
                    
                     0,0,0,0,0,0,0,0,0,0,            //0-19行，基本用不到
                    
                     0,0,0,0,0,0,0,0,0,0,        //20-29行
                    
                     4,3,3,2,2,3,2,2,3,2,            //30-39行
                  
                     4,3,3,2,2,3,2,2,3,2,           //40-49行
                     
                     1,1,1,1,1,1,1,1,1,1,           //最近十行   
                
                  };  
//坡道加权
float RampWeight[60]={ 
                   
                     0,0,0,0,0,0,0,0,0,0,           //0-9行，基本用不到
                    
                     0,0,0,0,0,0,0,0,0,0,            //0-19行，基本用不到
                    
                     0,0,0,0,0,0,0,0,0,0,           //40-49行
                    
                     0,0,0,0,0,0,0,0,0,0,            //30-39行
                  
                     1,1,1,1,1,1,1,1,1,1,           //40-49行
                     
                     1,1,1,1,1,1,1,1,1,1,           //最近十行   
                
                  };  
//断路加权
float CutWeight[60]={ 
                   
                     0,0,0,0,0,0,0,0,0,0,           //0-9行，基本用不到
                    
                     0,0,0,0,0,0,0,0,0,0,            //0-19行，基本用不到
                    
                     2,3,3,2,2,1,1,1,1,1,           //40-49行
                    
                     4,3,3,2,2,3,2,2,3,2,            //30-39行
                  
                     2,3,3,2,2,1,1,1,1,1,           //40-49行
                     
                     1,1,1,1,1,1,1,1,1,1,           //最近十行   
                
                  };  

//参数
float Tp=0;
float Td=0;
float Error;
float LastError=0;
float  WeightSum=0;
float  CenterMeanValue=0;
float  CenterSum=0;
int rad=0;
int RAD=0;
int time=0;
//偏差加权
void get_deviation(void)
{
  
  CenterSum=0;              
  CenterMeanValue=0;           
  WeightSum=0;  
  
  //入障碍
  if(obstructflag==1)
  {
    time=0;
    if(obstruct_direction==0)
    {
      Error=-25;
    }    
    else if(obstruct_direction==1)
    {
      Error=25;
    }
  }
  //出障碍
  else if(obstructflag==2)
  {
    if(obstruct_direction==0)
    {
      time++;
      if(time>10)
        Error=20;
      else
        Error=20;  
    }    
    else if(obstruct_direction==1)
    {
      time++;
      if(time>10)
        Error=-20;
      else
        Error=-20;
    }
  }
  else
  {
    //十字偏差处理
    //入十字
    if(ru_cross_flag==1)
    {
      //取点以下
      if(ru_point<55)
      {
        for(uint8 hang=55;hang>ru_point;hang--)  
        {        
          CenterSum+=midline[hang]*CrossWeight[hang]; 
          WeightSum+=CrossWeight[hang];       
        }
        //算出加权平均后中线的值
        if(WeightSum!=0) 
        {
          CenterMeanValue=(CenterSum/WeightSum);
        }
        // 一场图像偏差值 
        Error=(CenterMeanValue-basic);
        if(Error<-40)
          Error=-40;
        else if(Error>40)
          Error=40;
      }
      else
      {
        Error=LastError;
      }
    }
    //斜入十字
    else if(xie_cross_flag==1)
    {
      //取拐点以下
      if(SZ_xie_point<45)
      {
        for(uint8 hang=55;hang>SZ_xie_point;hang--)  
        {        
          CenterSum+=midline[hang]*CrossWeight[hang]; 
          WeightSum+=CrossWeight[hang];       
        }
        //算出加权平均后中线的值
        if(WeightSum!=0) 
        {
          CenterMeanValue=(CenterSum/WeightSum);
        }
        // 一场图像偏差值 
        Error=(CenterMeanValue-basic);
        if(Error<-40)
          Error=-40;
        else if(Error>40)
          Error=40;
      }
      else
      {
        Error=LastError;
      }
    }
    else if(cutrouteflag==2)
    {
      //取拐点以下
      for(uint8 hang=55;hang>endline;hang--)  
      {        
        CenterSum+=midline[hang]*CutWeight[hang]; 
        WeightSum+=CutWeight[hang];       
      }
      //算出加权平均后中线的值
      if(WeightSum!=0) 
      {
        CenterMeanValue=(CenterSum/WeightSum);
      }
      // 一场图像偏差值 
      Error=(CenterMeanValue-basic);
      if(endline>45)
        Error=-40;
      if(Error<-40)
        Error=-40;
      else if(Error>40)
        Error=40;
    }
    else if(cutrouteflag==3)
    {
      //取拐点以下
      for(uint8 hang=55;hang>endline;hang--)  
      {        
        CenterSum+=midline[hang]*CutWeight[hang]; 
        WeightSum+=CutWeight[hang];       
      }
      //算出加权平均后中线的值
      if(WeightSum!=0) 
      {
        CenterMeanValue=(CenterSum/WeightSum);
      }
      // 一场图像偏差值 
      Error=(CenterMeanValue-basic);
      if(endline>45)
        Error=40;
      if(Error<-40)
        Error=-40;
      else if(Error>40)
        Error=40;
    }
    else if(rampflag==1)
    {
      if(endline<45)
      {
        for(uint8 hang=55;hang>endline;hang--)  
        {        
          CenterSum+=midline[hang]*RampWeight[hang]; 
          WeightSum+=RampWeight[hang];       
        }
        //算出加权平均后中线的值
        if(WeightSum!=0) 
        {
          CenterMeanValue=(CenterSum/WeightSum);
        }
        // 一场图像偏差值 
        Error=(CenterMeanValue-basic);
      }
      else
      {
        Error=LastError;
      }
    }
    else
    {
      if(endline<45)
      {
        for(uint8 hang=50;hang>endline;hang--)  
        {        
          CenterSum+=midline[hang]*Weight[hang]; 
          WeightSum+=Weight[hang];       
        }
        //算出加权平均后中线的值
        if(WeightSum!=0) 
        {
          CenterMeanValue=(CenterSum/WeightSum);
        }
        // 一场图像偏差值 
        Error=(CenterMeanValue-basic);
        if(Error<-40)
          Error=-40;
        else if(Error>40)
          Error=40;
      }
      else
      {
        Error=(midline[hstart]-60);
     
        if(Error>=0)
          Error=40;
        if(Error<0)
          Error=-40;
      }
    }
    if(ringflag==1||ringflag==3)
    {
      if(endline>=45)
        Error=-40;
    }
    else if(ringflag==2||ringflag==4)
    {
      if(endline>=45)
        Error=40;
    }
  }
  //障碍给静态P 主动优化打角
  if(cutrouteflag==1)
  {
    Tp=1.8;
    Td=3;
  }
  else if(cutrouteflag==2||cutrouteflag==3)
  {
    Tp=Fuzzy_P(Error,Error-LastError)*2;//2
    Td=Fuzzy_D(Error,Error-LastError)*1.2;//1.5
  }
  //障碍给静态P 主动优化打角
  else if(obstructflag==1||obstructflag==2)
  {
    Tp=5;
    Td=0;
  }
  //入圆环
  else if(ringflag==1||ringflag==2)
  {
    Tp=Fuzzy_P(Error,Error-LastError)*3;
    Td=Fuzzy_D(Error,Error-LastError)*0;
  }
  else if(ringflag==3||ringflag==4)
  {
    Tp=Fuzzy_P(Error,Error-LastError)*2;
    Td=Fuzzy_D(Error,Error-LastError)*1.5;
  }
  else
  {
    Tp=Fuzzy_P(Error,Error-LastError)*2.7;//2.7
    Td=Fuzzy_D(Error,Error-LastError)*1.5;//1.4
  }
}
//舵机pid控制
void DM_Control(void)
{
  //需调
  float error_P=0,error_D=0;             

  error_P=Error;
  error_D=Error-LastError;
  LastError=Error;
  rad=(int)((Tp*error_P+Td*error_D));  
  
  RAD=rad;
  if(RAD>150) RAD=150;
  if(RAD<-150) RAD=-150;
  
  //舵机打角补偿
  //限制打角
  rad=-rad+MID;
  if(rad>LEFT)
    rad=LEFT;
  if(rad<RIGHT)
    rad=RIGHT;
  
  ftm_pwm_duty(ftm3,ftm_ch3,rad);          //改变舵机打角
}
#define ANG_NUM    45
int ssssss=0;
int endtime=0;
int a;
//差速
void differential_control(void)
{
  a=0;
  //右拐
  if(RAD>0)
  {
    a=ANG_NUM*RAD*1.0/(MID-RIGHT);
    if(a<0)
      a=0;
    else if(a>ANG_NUM)
      a=ANG_NUM;
    set_speed_you=High_Speed;
    set_speed_you=High_Speed-High_Speed*a*error_p/ANG_NUM;
    set_speed_zuo=High_Speed;
  }
  else
  {
    a=ANG_NUM*(-RAD)*1.0/(LEFT-MID);
    if(a<0)
      a=0;
    else if(a>ANG_NUM)
      a=ANG_NUM;
    set_speed_zuo=High_Speed;
    set_speed_zuo=High_Speed-High_Speed*a*error_p/ANG_NUM;
    set_speed_you=High_Speed;
  }
  //冲出赛道
  if(zuo<50&&you<50&&obstructflag==0)
  {
    ssssss=1;
  }
  if(ssssss==1)
  {
    set_speed_zuo=0;
    set_speed_you=0;
  }
  //停车
  if(scratchflag==1)
  {
    if(endtime<50)
      endtime++;
    if(endtime>20)
    {
      set_speed_zuo=0;
      set_speed_you=0;
    }
  }
}
//电磁差速
void differential_control_ele(void)
{
  a=0;
  //右拐
  if(RAD>0)
  {
    a=ANG_NUM*RAD*1.0/(MID-RIGHT);
    if(a<0)
      a=0;
    else if(a>ANG_NUM)
      a=ANG_NUM;
    set_speed_you=High_Speed-High_Speed*a*0.3/ANG_NUM;
    set_speed_zuo=High_Speed+High_Speed*a*0.3/ANG_NUM;
  }
  else
  {
    a=ANG_NUM*(-RAD)*1.0/(LEFT-MID);
    if(a<0)
      a=0;
    else if(a>ANG_NUM)
      a=ANG_NUM;
    set_speed_zuo=High_Speed-High_Speed*a*0.3/ANG_NUM;
    set_speed_you=High_Speed+High_Speed*a*0.3/ANG_NUM;
  }
}


