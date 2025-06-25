#include "headfile.h"
/*********************************************************************

**********************************************************************/

#define step 5


void get_gray(void)
{
  Get_Use_Image();
  get_gray_one(59);
  get_gray_all();
  get_gray_endline();
}

//��һ��ͼ��Ѱ��
//������������ͼ��Ѱ�߷�ʽ����
void get_gray_one(uint8 hang)
{
  //����
  for(uint8 hang=0;hang<Row;hang++)
  {
    leftline[hang]=0;
    rightline[hang]=Col-1;
    leftlineflag[hang]=0;
    rightlineflag[hang]=0;
    midline[hang]=Col/2;
  }
  endline=0;
  //��Ȿ�������߱�־λ
  int baihei=0;
  for(uint8 lie=1;lie<Col-2;lie++)
  {
    //�׺�����
    if(Image_Use[hang][lie]-Image_Use[hang][lie+1]>step)
      baihei++;
  }
  //������Ѱ��
  if(baihei>3)
  {
    //�����
    for(uint8 lie=0;lie<Col*3/4;lie++)
    {
      //�ڰ�
      if((Image_Use[hang][lie+1]-Image_Use[hang][lie])*100/(Image_Use[hang][lie+1]+Image_Use[hang][lie])>step)
      {
        leftline[hang]=lie+1;
        leftlineflag[hang]=1;
        break;
      }
    }
    if(leftlineflag==0)
    {
      leftline[hang]=1;
      leftlineflag[hang]=1;
    }
    //�ұ���
    for(uint8 lie=Col-1;lie>Col*1/4;lie--)
    {
      if((Image_Use[hang][lie-1]-Image_Use[hang][lie])*100/(Image_Use[hang][lie-1]+Image_Use[hang][lie])>step)
      {
        rightline[hang]=lie-1;
        rightlineflag[hang]=1;
        break;
      }
    }
    if(rightlineflag[hang]==0)
    {
      rightline[hang]=Col-2;
      rightlineflag[hang]=1;
    }
  }
  else
  {
    //�����
    for(uint8 lie=Col/2;lie>0;lie--)
    {
      //�׺�
      if((Image_Use[hang][lie+1]-Image_Use[hang][lie])*100/(Image_Use[hang][lie+1]+Image_Use[hang][lie])>step)
      {
        leftline[hang]=lie+1;
        leftlineflag[hang]=1;
        break;
      }
    }
    if(leftlineflag[hang]==0)
    {
      for(uint8 lie=Col/2;lie<Col*3/4;lie++)
      {
        if((Image_Use[hang][lie]-Image_Use[hang][lie-1])*100/(Image_Use[hang][lie]+Image_Use[hang][lie-1])>step)
        {
          leftline[hang]=lie;
          leftlineflag[hang]=1;
          break;
        }
      }
    }
    if(leftlineflag[hang]==0)
    {
      leftline[hang]=1;
      leftlineflag[hang]=1;
    }
    //�ұ���
    for(uint8 lie=Col/2;lie<Col;lie++)
    {
      //�׺�
      if((Image_Use[hang][lie-1]-Image_Use[hang][lie])*100/(Image_Use[hang][lie-1]+Image_Use[hang][lie])>step)
      {
        rightline[hang]=lie-1;
        rightlineflag[hang]=1;
        break;
      }
    }
    if(rightlineflag[hang]==0)
    {
      for(uint8 lie=Col/2;lie>Col*1/4;lie--)
      {
        if((Image_Use[hang][lie]-Image_Use[hang][lie+1])*100/(Image_Use[hang][lie]+Image_Use[hang][lie+1])>step)
        {
          rightline[hang]=lie;
          rightlineflag[hang]=1;
          break;
        }
      }
    }
    if(rightlineflag[hang]==0)
    {
      rightline[hang]=Col-2;
      rightlineflag[hang]=1;
    }
  }
}

void get_gray_all(void)
{
  int range=5;
  int L_max=0,L_min=0;
  int R_max=0,R_min=0;
  
  for(uint8 hang=Row-2;hang>0;hang--)
  {
    //ȷ����Χ
    if(leftlineflag[hang+1]==0)
    {
      L_max=Col/2;
      L_min=0;
    }
    else
    {
      L_max=leftline[hang+1]+range;
      L_min=leftline[hang+1]-range;
      if(L_max>Col-1)
        L_max=Col-1;
      if(L_min<0)
        L_min=0;
    }
  
    if(rightlineflag[hang+1]==0)
    {
      R_max=Col-1;
      R_min=Col/2;
    }
    else
    {
      R_max=rightline[hang+1]+range;
      R_min=rightline[hang+1]-range;
      if(R_max>Col-1)
        R_max=Col-1;
      if(R_min<0)
        R_min=0;
    } 
    //�����
    for(uint8 lie=L_max;lie>L_min;lie--)
    {
      //�׺�
      if((Image_Use[hang][lie+1]-Image_Use[hang][lie])*100/(Image_Use[hang][lie+1]+Image_Use[hang][lie])>step)
      {
        leftline[hang]=lie+1;
        leftlineflag[hang]=1;
        break;
      }  
    }
    if(leftlineflag[hang]==0)
    {
      leftline[hang]=1;  
    }
    //�ұ���
    for(uint8 lie=R_min;lie<R_max;lie++)
    {
      //�׺�
      if((Image_Use[hang][lie-1]-Image_Use[hang][lie])*100/(Image_Use[hang][lie-1]+Image_Use[hang][lie])>step)
      {
        rightline[hang]=lie-1;
        rightlineflag[hang]=1;
        break;
      }
    }
    if(rightlineflag[hang]==0)
    {
      rightline[hang]=Col-2; 
    }
  }
  
  //����Զһ�а���һ�в�
  leftline[0]=leftline[1];
  leftlineflag[0]=leftlineflag[1];
  rightline[0]=rightline[1];
  rightlineflag[0]=rightlineflag[1];
    
}

void get_gray_endline(void)
{
  endline=0;
  //��ֹ���ж�
  for(uint8 hang=Row-5;hang>5;hang--)
  {
    int mid=0; 
    //���ֲ���
    if((leftlineflag[hang]==0)&&(rightlineflag[hang]!=0))    
    { 
      mid=rightline[hang]-60; 
    }      
    //�Ҷ��󲻶�
    if((leftlineflag[hang]!=0)&&(rightlineflag[hang]==0))        
    {  
      mid=leftline[hang]+60; 
    }
    //ȫ��
    if((leftlineflag[hang]==0)&&(rightlineflag[hang]==0)) 
    {
      mid=Col/2;
    }
    //ȫ����
    if((leftlineflag[hang]!=0)&&(rightlineflag[hang]!=0))
    {
      mid=(leftline[hang]+rightline[hang])/2;
    }
    if(mid<10)
      mid=10;
    else if(mid>110)
      mid=110;
    if((Image_Use[hang][mid]-Image_Use[hang-1][mid])*100/(Image_Use[hang][mid]+Image_Use[hang-1][mid])>step-2)
    {
      endline=hang;
      break;
    }
  }
}