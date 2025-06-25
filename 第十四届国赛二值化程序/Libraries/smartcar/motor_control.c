#include "headfile.h"
/*********************************************************************
�ٶȿ���
���������ж� 1�����ֱ�� 2ֱ�� 3ֱ������� 4��� ���ȶ���

**********************************************************************/
float Last_Error[5];

int Track_Line_Time=0;
int Track_Line_Num=0;
float Foresight=0;
int Foresight_hang=0;
float Error_abs=0;

int Set_Speed=0;
//50
int Max_Speed;      
int Min_Speed=460;   
int High_Speed=0;    //�����ٶȵĲ���

int cutroute_time=0;

int obstruct_speed=250;//300
int cutroute_speed=400;//250  350
int ramp_speed=450;  //450
int ringstart_speed=350;
int ring_speed=480;  //480

//���������ж�
void Track_Line(void)
{
  //ƫ�ֵ
  Track_Line_Time++;
  if(Error<0)
    Error_abs=-Error;
  else 
    Error_abs=Error;
  if(Track_Line_Time==5)
  {
     Track_Line_Time=0;
     Last_Error[0] = Last_Error[1];
     Last_Error[1] = Last_Error[2];
     Last_Error[2] = Last_Error[3];
     Last_Error[3] = Last_Error[4];
     Last_Error[4] = Error_abs;
  }
    
  //ǰհ 
  if(endline<20)
    Foresight_hang=20;
  else
    Foresight_hang=endline+5;
  if(Foresight_hang>58)
    Foresight_hang=58;
  Foresight=(0.2*Error+0.8*(midline[Foresight_hang]-60));
  if(Foresight<0)
    Foresight=-Foresight;
  if(Foresight>40)
    Foresight=40;
   
  if(Last_Error[0]>15 && Last_Error[1]>15 && Last_Error[2]>15  && endline<25) //�����ֱ��
  {
    Track_Line_Num = 1;
  }
  else if(Last_Error[0]<15 && Last_Error[1]<15 && Last_Error[2]<15 && Foresight<=15) //ֱ��
  {
    Track_Line_Num = 2;
  }
  else if(Last_Error[0]<15 && Last_Error[1]<15 && Last_Error[2]<15 && Foresight>=15) //ֱ�������
  {
    Track_Line_Num = 3;
  }
  else                                                                          //�����
  {
    Track_Line_Num=4;
  }
}

//���ٿ���
void Control_Speed(void)
{
  Max_Speed=Min_Speed+30;  //30

  if(Track_Line_Num==1)    
  {     
    Set_Speed +=5;//5
  }     
  else if (Foresight >= 30)        //34
  {       
    Set_Speed = Min_Speed;
  }     
  else if (Foresight >= 27)    //31
  {    
    Set_Speed = Min_Speed+1;
  }       
  else if (Foresight >= 24)       //28
  {   
    Set_Speed = Min_Speed+2; 
  }
  else if (Foresight >= 21)     //25
  {       
    Set_Speed = Min_Speed+3;      
  }       
  else if (Foresight >= 18)       //22
  {         
    Set_Speed = Min_Speed+4;        
  }       
  else if (Foresight >= 15)       //19
  {       
    Set_Speed = Min_Speed+5;       
  }       
  else if (Foresight >= 12)        //16
  {   
    Set_Speed = Min_Speed+6;     
  }    
  else     
  {  
    Set_Speed +=3;  //2   
  }
  
  if(Set_Speed>Max_Speed)Set_Speed=Max_Speed;    
  if(Set_Speed<Min_Speed)Set_Speed=Min_Speed;
  
  int cuttime;
  
  if(cutrouteflag==0)
    cuttime=0;
  
  if(obstructflag!=0)
  {
      Set_Speed=obstruct_speed;
  }
  else if(cutrouteflag!=0)  
  {
    cuttime++;
    if(cuttime<30)
      Set_Speed=0;
    else
      Set_Speed=cutroute_speed;
  }
  else if(ringflag==1||ringflag==2)
  {
    Set_Speed=ringstart_speed;
  }
  else if(ringflag==3||ringflag==4)
  {
    Set_Speed=ring_speed;
  }
  else if(rampflag==1)
    Set_Speed=ramp_speed;
  
  High_Speed = Set_Speed;
}





