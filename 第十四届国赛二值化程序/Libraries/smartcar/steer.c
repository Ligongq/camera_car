#include "headfile.h"

/**********************************************
���������
ƫ����ȡ ��Ȩƽ��
ģ��pid����
***********************************************/
//����޷�����
#define MID     875    
#define RIGHT   730  //730   
#define LEFT    1020   
//������ֵ
#define basic 61  //61
//��������
float error_p=0.5; //*********************************
//������Ȩ
float Weight[60]={ 
                     0,0,0,0,0,0,0,0,0,0,           //0-9�У������ò���
                    
                     0,0,0,0,0,0,0,0,0,0,            //0-19�У������ò���
                    
                     5,5,8,7,8,9,8,8,9,9,        //20-29��
                    
                     6,7,7,6,5,4,4,5,5,4,            //30-39��
                  
                     2,2,2,2,2,1,1,1,1,1,           //40-49��
                     
                     0,0,0,0,0,0,0,0,0,0,           //���ʮ��   
                
                  };  
//ʮ�ּ�Ȩ
float CrossWeight[60]={ 
                   
                     0,0,0,0,0,0,0,0,0,0,           //0-9�У������ò���
                    
                     0,0,0,0,0,0,0,0,0,0,            //0-19�У������ò���
                    
                     0,0,0,0,0,0,0,0,0,0,        //20-29��
                    
                     4,3,3,2,2,3,2,2,3,2,            //30-39��
                  
                     4,3,3,2,2,3,2,2,3,2,           //40-49��
                     
                     1,1,1,1,1,1,1,1,1,1,           //���ʮ��   
                
                  };  
//�µ���Ȩ
float RampWeight[60]={ 
                   
                     0,0,0,0,0,0,0,0,0,0,           //0-9�У������ò���
                    
                     0,0,0,0,0,0,0,0,0,0,            //0-19�У������ò���
                    
                     0,0,0,0,0,0,0,0,0,0,           //40-49��
                    
                     0,0,0,0,0,0,0,0,0,0,            //30-39��
                  
                     1,1,1,1,1,1,1,1,1,1,           //40-49��
                     
                     1,1,1,1,1,1,1,1,1,1,           //���ʮ��   
                
                  };  
//��·��Ȩ
float CutWeight[60]={ 
                   
                     0,0,0,0,0,0,0,0,0,0,           //0-9�У������ò���
                    
                     0,0,0,0,0,0,0,0,0,0,            //0-19�У������ò���
                    
                     2,3,3,2,2,1,1,1,1,1,           //40-49��
                    
                     4,3,3,2,2,3,2,2,3,2,            //30-39��
                  
                     2,3,3,2,2,1,1,1,1,1,           //40-49��
                     
                     1,1,1,1,1,1,1,1,1,1,           //���ʮ��   
                
                  };  

//����
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
//ƫ���Ȩ
void get_deviation(void)
{
  
  CenterSum=0;              
  CenterMeanValue=0;           
  WeightSum=0;  
  
  //���ϰ�
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
  //���ϰ�
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
    //ʮ��ƫ���
    //��ʮ��
    if(ru_cross_flag==1)
    {
      //ȡ������
      if(ru_point<55)
      {
        for(uint8 hang=55;hang>ru_point;hang--)  
        {        
          CenterSum+=midline[hang]*CrossWeight[hang]; 
          WeightSum+=CrossWeight[hang];       
        }
        //�����Ȩƽ�������ߵ�ֵ
        if(WeightSum!=0) 
        {
          CenterMeanValue=(CenterSum/WeightSum);
        }
        // һ��ͼ��ƫ��ֵ 
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
    //б��ʮ��
    else if(xie_cross_flag==1)
    {
      //ȡ�յ�����
      if(SZ_xie_point<45)
      {
        for(uint8 hang=55;hang>SZ_xie_point;hang--)  
        {        
          CenterSum+=midline[hang]*CrossWeight[hang]; 
          WeightSum+=CrossWeight[hang];       
        }
        //�����Ȩƽ�������ߵ�ֵ
        if(WeightSum!=0) 
        {
          CenterMeanValue=(CenterSum/WeightSum);
        }
        // һ��ͼ��ƫ��ֵ 
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
      //ȡ�յ�����
      for(uint8 hang=55;hang>endline;hang--)  
      {        
        CenterSum+=midline[hang]*CutWeight[hang]; 
        WeightSum+=CutWeight[hang];       
      }
      //�����Ȩƽ�������ߵ�ֵ
      if(WeightSum!=0) 
      {
        CenterMeanValue=(CenterSum/WeightSum);
      }
      // һ��ͼ��ƫ��ֵ 
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
      //ȡ�յ�����
      for(uint8 hang=55;hang>endline;hang--)  
      {        
        CenterSum+=midline[hang]*CutWeight[hang]; 
        WeightSum+=CutWeight[hang];       
      }
      //�����Ȩƽ�������ߵ�ֵ
      if(WeightSum!=0) 
      {
        CenterMeanValue=(CenterSum/WeightSum);
      }
      // һ��ͼ��ƫ��ֵ 
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
        //�����Ȩƽ�������ߵ�ֵ
        if(WeightSum!=0) 
        {
          CenterMeanValue=(CenterSum/WeightSum);
        }
        // һ��ͼ��ƫ��ֵ 
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
        //�����Ȩƽ�������ߵ�ֵ
        if(WeightSum!=0) 
        {
          CenterMeanValue=(CenterSum/WeightSum);
        }
        // һ��ͼ��ƫ��ֵ 
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
  //�ϰ�����̬P �����Ż����
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
  //�ϰ�����̬P �����Ż����
  else if(obstructflag==1||obstructflag==2)
  {
    Tp=5;
    Td=0;
  }
  //��Բ��
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
//���pid����
void DM_Control(void)
{
  //���
  float error_P=0,error_D=0;             

  error_P=Error;
  error_D=Error-LastError;
  LastError=Error;
  rad=(int)((Tp*error_P+Td*error_D));  
  
  RAD=rad;
  if(RAD>150) RAD=150;
  if(RAD<-150) RAD=-150;
  
  //�����ǲ���
  //���ƴ��
  rad=-rad+MID;
  if(rad>LEFT)
    rad=LEFT;
  if(rad<RIGHT)
    rad=RIGHT;
  
  ftm_pwm_duty(ftm3,ftm_ch3,rad);          //�ı������
}
#define ANG_NUM    45
int ssssss=0;
int endtime=0;
int a;
//����
void differential_control(void)
{
  a=0;
  //�ҹ�
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
  //�������
  if(zuo<50&&you<50&&obstructflag==0)
  {
    ssssss=1;
  }
  if(ssssss==1)
  {
    set_speed_zuo=0;
    set_speed_you=0;
  }
  //ͣ��
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
//��Ų���
void differential_control_ele(void)
{
  a=0;
  //�ҹ�
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


