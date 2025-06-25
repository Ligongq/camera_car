#include "headfile.h"


#define MID     1353     
#define RIGHT   1527     //1537    
#define LEFT    1207     //1217


float Tp=0;
float Td=0;

float RAD[5]={0};

float Weight[60]={ 
                     0,0,0,0,0,0,0,0,0,0,           //0-9�У������ò���
                    
                     4,3,3,2.2,3,2.2,3,2,           //0-19�У������ò���
                    
                     5,6,4,5,4,6,5,5,4,6,           //20-29��
                    
                     4,3,4,3,3,3,4,4,3,3,           //30-39��
                  
                     4,3,3,2.2,3,2.2,3,2,           //40-49��
                     
                     1,1,1,2,2,1,1,1,2,1,           //���ʮ��   
                
                  };         

float  WeightSum=0;
float  CenterMeanValue=0;
float  CenterSum=0;

float temp=0;

int rad=0;

float  BasicP; //������Pֵ
float  J;//����p��ƫ��Ĺ�ϵ��Խ������Խǿ J=0.160(3.15)

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
    CenterMeanValue=(CenterSum/WeightSum);//�����Ȩƽ�������ߵ�ֵ
  }
  
  temp=(CenterMeanValue-61);// һ��ͼ��ƫ��ֵ 
  
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
  
  //��ʱ��Ǵ洢��ϴ���RAD�б���pid���ƶ�ȡ
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
  //���
  float error_P=0,error_D=0;             

  error_P=RAD[0];
  error_D=RAD[0]-RAD[1];
  rad=(int)((Tp*error_P+Td*error_D));                    
  
  //�����ǲ���
  //���ƴ��
  rad=rad+MID;
  
  if(rad<LEFT)
    rad=LEFT;
  if(rad>RIGHT)
    rad=RIGHT;
  
  ftm_pwm_duty(ftm3,ftm_ch3,rad);          //�ı������
  
  if(cutrouteflag==1)
    ftm_pwm_duty(ftm3,ftm_ch3,MID);
  
}
