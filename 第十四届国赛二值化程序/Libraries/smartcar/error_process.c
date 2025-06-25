#include "headfile.h"


#define MID     1353     
#define RIGHT   1527     //1537    
#define LEFT    1207     //1217


float Tp=0;
float Td=0;

float RAD[5]={0};

float Weight[60]={ 
                     0,0,0,0,0,0,0,0,0,0,           //0-9行，基本用不到
                    
                     4,3,3,2.2,3,2.2,3,2,           //0-19行，基本用不到
                    
                     5,6,4,5,4,6,5,5,4,6,           //20-29行
                    
                     4,3,4,3,3,3,4,4,3,3,           //30-39行
                  
                     4,3,3,2.2,3,2.2,3,2,           //40-49行
                     
                     1,1,1,2,2,1,1,1,2,1,           //最近十行   
                
                  };         

float  WeightSum=0;
float  CenterMeanValue=0;
float  CenterSum=0;

float temp=0;

int rad=0;

float  BasicP; //基本的P值
float  J;//调节p和偏差的关系，越大，作用越强 J=0.160(3.15)

void get_deviation(void)
{
  
  CenterSum=0;              
  CenterMeanValue=0;           
  WeightSum=0;  
  
  for(uint8 hang=57;hang>endline;hang--)  
  {        
    CenterSum+=midline[hang]*Weight[hang]; 
    WeightSum+=Weight[hang];       
  }
  
  if(WeightSum!=0) 
  {
    CenterMeanValue=(CenterSum/WeightSum);//算出加权平均后中线的值
  }
  
  temp=(CenterMeanValue-61);// 一场图像偏差值 
  
  if(temp>40)
    temp=40;
  else if(temp<-40)
    temp=-40;
  
  if(endline>50)
  {
    if(ringflag==0)
    {
      int time=0;                                          
      for(uint8 i=0;i<5;i++)
      {
        if(RAD[i]>=0)
          time++;
        else
          time--;
      }
      if(time>=0)                                          
        temp=40;
      else                                                 
        temp=-40;
    }
    else if(ringflag==1||ringflag==3)
    {
      temp=-40;
    }
    else if(ringflag==2||ringflag==4)
    {
      temp=40;
    }
  }
  
  //临时打角存储完毕存入RAD中便于pid控制读取
  for(uint8 i=4;i>0;i--)
    RAD[i]=RAD[i-1];
  RAD[0]=temp; 
  
  OLED_Print_Num1(90,0,(int)temp);
  
  BasicP=3.2;
  J=0.0038;//1600
  Tp=(temp*temp)*J+BasicP;
  Td=5;
  if(Tp>10)
    Tp=10;
}

void DM_Control(void)
{
  //需调
  float error_P=0,error_D=0;             

  error_P=RAD[0];
  error_D=RAD[0]-RAD[1];
  rad=(int)((Tp*error_P+Td*error_D));                    
  
  //舵机打角补偿
  //限制打角
  rad=rad+MID;
  
  if(rad<LEFT)
    rad=LEFT;
  if(rad>RIGHT)
    rad=RIGHT;
  
  ftm_pwm_duty(ftm3,ftm_ch3,rad);          //改变舵机打角
  
  if(cutrouteflag==1)
    ftm_pwm_duty(ftm3,ftm_ch3,MID);
  
}
