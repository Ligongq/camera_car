#include "headfile.h"
/*********************************************************************
����Ѱ��
��һ�����⴦���������ߺ��������� ���ȶ���
����Ѱ�� �������Ż�����һ���ҵ�����Χ���ߣ���һ�ж�ȫ���ң��ȶ���
��ֹ��Ѱ�� Բ���ص���ʮ�ֽص��϶� ���ȽϺ�ʹ
������� ���������ȣ���ֱ�����  (�ȶ�)
����������ȡ �����ж�ֱ�����ߣ��ȶ���
**********************************************************************/
//�������б������� 1-118
int leftline[Row];
int midline[Row];
int rightline[Row];
//�洢��־ 2���� 1��һ������ 0����
int leftlineflag[Row];
int rightlineflag[Row];
//��ֹ�� 55-0
int endline=0;
//������ʵ�������
int route_width[Row];
//��ʵ�������
int track_width[Row]={5,6,7,8,9,10,11,13,15,17,
                      19,21,23,25,26,27,29,31,33,34,
                      36,38,40,41,43,45,47,49,51,53,
                      55,56,58,60,63,64,66,67,68,69,
                      73,75,77,79,81,83,85,87,88,91,
                      93,94,95,97,99,101,102,104,106,106};
int width[Row];
//��ϵ��������
#define Width_P_W_L  1.1  //1.2
#define Width_P_W_R  1.5  //1.5
#define Width_P_Z  1
#define Width_P_R_L  1  //1.25
#define Width_P_R_R  1.3

int scratch_cnt=0;
int scratch_allfind=0;
int scratchflag=0;

int BlackEndMR      = 0;
int BlackEndML      = 0;
int BlackEndLL      = 0;
int BlackEndRR      = 0;
int BlackEndL       = 0;
int BlackEndM       = 0;
int BlackEndR       = 0;
int BlackEndMaxMax  = 0;
int BlackEndMax     = 0;
int DropRow         = 0;


//��һ��ͼ��Ѱ��
//������������ͼ��Ѱ�߷�ʽ����
void get_route_one(uint8 hang,uint8 start)
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
  for(uint8 lie=0;lie<Col-1;lie++)
  {
    if(Pixle[hang][lie]==white&&Pixle[hang][lie+1]==black)
      baihei++;
  }
  //������Ѱ��
  if(baihei>3)
  {
    if(Pixle[hang][5]==white)
    {
      //������
      for(uint8 lie=5;lie>=1;lie--)
      {
		if(Pixle[hang][lie]==white&&Pixle[hang][lie-1]==black)
		{
			leftline[hang]=lie;
			leftlineflag[hang]=1;
			break;
		}
      }
    }
    else
    {
      //������
      for(uint8 lie=5;lie<=Col-2;lie++)
      {
		if(Pixle[hang][lie]==black&&Pixle[hang][lie+1]==white)
		{
			leftline[hang]=lie;
			leftlineflag[hang]=1;
			break;
		}
      }
    }
    //���û�ҵ���ֱ�Ӹ���ֵ
    if(leftlineflag[hang]==0)
    {
      leftline[hang]=1;
      leftlineflag[hang]=1;
    }
    //�ұ���
    if(Pixle[hang][Col-5]==white)
    {
      //������
      for(uint8 lie=Col-5;lie<=Col-2;lie++)
      {
		if(Pixle[hang][lie]==white&&Pixle[hang][lie+1]==black)
		{
			rightline[hang]=lie;
			rightlineflag[hang]=2;
			break;
		}
      }
    }
    else
    {
      //������
      for(uint8 lie=Col-5;lie>=1;lie--)
      {
		if(Pixle[hang][lie]==black&&Pixle[hang][lie-1]==white)
		{
			rightline[hang]=lie;
			rightlineflag[hang]=1;
			break;
		}
      }
    }
    //���û�ҵ���ֱ�Ӹ���ֵ
    if(rightlineflag[hang]==0)
    {
      rightline[hang]=Col-2;
      rightlineflag[hang]=2;
    }
  }
  else
  {
    //�����
    if(Pixle[hang][start]==white)
    {
      //������
      for(uint8 lie=start;lie>=1;lie--)
      {
		if(Pixle[hang][lie]==white&&Pixle[hang][lie-1]==black)
		{
			leftline[hang]=lie;
			leftlineflag[hang]=2;
			break;
		}
      }
    }
    else
    {
      //������
      for(uint8 lie=start;lie<=Col-2;lie++)
      {
		if(Pixle[hang][lie]==black&&Pixle[hang][lie+1]==white)
		{
			leftline[hang]=lie;
			leftlineflag[hang]=1;
			break;
		}
      }
    }
    //���û�ҵ���ֱ�Ӹ���ֵ
    if(leftlineflag[hang]==0)
    {
      leftline[hang]=1;
      leftlineflag[hang]=2;
    }
    //�ұ���
    if(Pixle[hang][start]==white)
    {
      //������
      for(uint8 lie=start;lie<=Col-2;lie++)
      {
		if(Pixle[hang][lie]==white&&Pixle[hang][lie+1]==black)
		{
			rightline[hang]=lie;
			rightlineflag[hang]=2;
			break;
		}
      }
    }
    else
    {
      //������
      for(uint8 lie=start;lie>=1;lie--)
      {
		if(Pixle[hang][lie]==black&&Pixle[hang][lie-1]==white)
		{
			rightline[hang]=lie;
			rightlineflag[hang]=1;
			break;
		}
      }
    }
    //���û�ҵ���ֱ�Ӹ���ֵ
    if(rightlineflag[hang]==0)
    {
      rightline[hang]=Col-2;
      rightlineflag[hang]=2;
    }
    //������ұ��߶������Կ���
    if(rightlineflag[hang]==1&&leftlineflag[hang]==1)
    {
      leftline[hang]=1;
      rightline[hang]=Col-2;
    }
    leftlineflag[hang]=1;
    rightlineflag[hang]=1;
  }
}
//����Ѱ��
void get_route_all(void)
{
  //��ʼ��
  int Lstart=0,L_max=0,L_min=0;
  int Rstart=0,R_max=0,R_min=0;
  int range=10;
  for(uint8 hang=Row-2;hang>0;hang--)
  {
	//�����
	//ȷ�����߷�Χ
    //������һ��ȷ������Ѱ�ߵ�
    if(leftlineflag[hang+1]==0)
    {
      L_max=Col-1;
      L_min=0;
      Lstart=Col*1/2-10;
    }
    else
    {
      L_max=leftline[hang+1]+range;
      L_min=leftline[hang+1]-range;
      Lstart=leftline[hang+1];
      if(L_max>Col-1)
        L_max=Col-1;
      if(L_min<0)
        L_min=0;
    }
    //�����
    if(Pixle[hang][Lstart]==black)
    {
      //������
      for(uint8 lie=Lstart;lie<L_max;lie++)
      {
        if(Pixle[hang][lie]==black&&Pixle[hang][lie+1]==white)
        {
			leftline[hang]=lie;
			leftlineflag[hang]=1;
			break;
        }
      }
      //�����û�ҵ�˵������
      if(leftlineflag[hang]==0)
      {
        leftline[hang]=Col-2;
        leftlineflag[hang]=0;
      }
    }
    else
    {
      //������
      for(uint8 lie=Lstart;lie>L_min;lie--)
      {
        if(Pixle[hang][lie]==white&&Pixle[hang][lie-1]==black)
        {
            leftline[hang]=lie;
            leftlineflag[hang]=1;
			break;
        }
      }
      //�����û�ҵ�˵������
      if(leftlineflag[hang]==0)
      {
        leftline[hang]=1;
        leftlineflag[hang]=0;
      }
    }
    //�ұ���
    //ȷ��Ѱ�߷�Χ
    if(rightlineflag[hang+1]==0)
    {
      Rstart=Col*1/2+10;
      R_max=Col-1;
      R_min=0;
    }
    else
    {
      Rstart=rightline[hang+1];
      R_max=rightline[hang+1]+range;
      R_min=rightline[hang+1]-range;
      if(R_max>Col-1)
        R_max=Col-1;
      if(R_min<0)
        R_min=0;
    }
    //�ұ���
    if(Pixle[hang][Rstart]==black)
    {
      //������
      for(uint8 lie=Rstart;lie>R_min;lie--)
      {
        if(Pixle[hang][lie]==black&&Pixle[hang][lie-1]==white)
        {
          rightline[hang]=lie;
          rightlineflag[hang]=1;
          break;
        }
      }
      //�����û�ҵ�
      if(rightlineflag[hang]==0)
      {
        rightline[hang]=1;
        rightlineflag[hang]=0;
      }
    }
    else
    {
      //������
      for(uint8 lie=Rstart;lie<R_max;lie++)
      {
        if(Pixle[hang][lie]==white&&Pixle[hang][lie+1]==black)
        {
          rightline[hang]=lie;
          rightlineflag[hang]=1;
          break;
        }
      }
      //�����û�ҵ�
      if(rightlineflag[hang]==0)
      {
        rightline[hang]=Col-2;
        rightlineflag[hang]=0;
      }
    }
  }
  //����Զһ�а���һ�в�
  leftline[0]=leftline[1];
  leftlineflag[0]=leftlineflag[1];
  rightline[0]=rightline[1];
  rightlineflag[0]=rightlineflag[1];
}
//Ѱ��ֹ��
void get_route_endline(void)
{  
  //��ֹ�е��ж���Ҫ�����۵㣬���ﲻ�ƻ�ԭ������������
  int left_line[Row],right_line[Row];
  
  //����
  for(uint8 hang=Row-1;hang>0;hang--)
  {
    left_line[hang]=leftline[hang];
    right_line[hang]=rightline[hang];
  }
  //���۵㣨ͻ��㣩
  for(uint8 hang=Row-3;hang>0;hang--)                    
  {    
    int temp=0;       
    //��	
    if((left_line[hang+1]-left_line[hang])>0)                  
    {                               
		temp=left_line[hang+1];                                  
		left_line[hang+1]=left_line[hang];
		left_line[hang]=temp;
    }
    //��
    if((right_line[hang]-right_line[hang+1])>0)                 
    {                           
		temp=right_line[hang+1];
		right_line[hang+1]=right_line[hang];
		right_line[hang]=temp;
    }
  }
  endline=DropRow;
  //��ֹ���ж�
  for(uint8 hang=Row-5;hang>endline;hang--)
  {
    //����������ı���
    if((right_line[hang-1]<left_line[hang])
       ||(left_line[hang-1]>right_line[hang]))
    {
		endline=hang;
		break;
    }
  }
 
  if(DropRow<10&&BlackEndM>50)
    endline=DropRow;

}
//�������
void get_route_midline(void)
{
  for(uint8 hang=55;hang>endline;hang--)
  {
    //ֱ������
    if(BlackEndM>=10)
    {
      //���ֲ���
      if((leftlineflag[hang]==0)&&(rightlineflag[hang]!=0))    
      {
        leftline[hang]=rightline[hang]-Width_P_Z*track_width[hang]-5; 
        midline[hang]=(leftline[hang]+rightline[hang])/2;
      }      
      //�Ҷ��󲻶�
      if((leftlineflag[hang]!=0)&&(rightlineflag[hang]==0))      
      {
        rightline[hang]=leftline[hang]+Width_P_Z*track_width[hang]+5;
        midline[hang]=(leftline[hang]+rightline[hang])/2;
      }
      //ȫ��
      if((leftlineflag[hang]==0)&&(rightlineflag[hang]==0))
      {
        rightline[hang]=Col-2;
        leftline[hang]=1;
        midline[hang]=(leftline[hang]+rightline[hang])/2;
      }
      //ȫ����
      if((leftlineflag[hang]!=0)&&(rightlineflag[hang]!=0))
      {
        midline[hang]=(leftline[hang]+rightline[hang])/2;
      }
    }
    else
    {
      if(ringflag==3) 
      {
        //���ֲ���
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]!=0))    
        {
          leftline[hang]=rightline[hang]-Width_P_R_L*track_width[hang]; 
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }       
        //�Ҷ��󲻶�
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]==0))      
        {
          rightline[hang]=leftline[hang]+Width_P_R_L*track_width[hang];
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }
        //ȫ��
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]==0))
        {
          rightline[hang]=Col-2;
          leftline[hang]=1;
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }
        //ȫ����
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]!=0))
        {
          midline[hang]=(leftline[hang]+rightline[hang])/2;   
        } 
      }
      else if(ringflag==4) 
      {
        //���ֲ���
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]!=0))    
        {
          leftline[hang]=rightline[hang]-Width_P_R_R*track_width[hang]; 
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }       
        //�Ҷ��󲻶�
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]==0))      
        {
          rightline[hang]=leftline[hang]+Width_P_R_R*track_width[hang];
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }
        //ȫ��
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]==0))
        {
          rightline[hang]=Col-2;
          leftline[hang]=1;
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }
        //ȫ����
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]!=0))
        {
          midline[hang]=(leftline[hang]+rightline[hang])/2;   
        } 
      }
      else
      {
        //���ֲ���
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]!=0))    
        {
          leftline[hang]=rightline[hang]-Width_P_W_L*track_width[hang];
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }      
        //�Ҷ��󲻶�
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]==0))              
        {       
          rightline[hang]=leftline[hang]+Width_P_W_R*track_width[hang];
          midline[hang]=(leftline[hang]+rightline[hang])/2;       
        }
         //ȫ
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]==0))
        {
          rightline[hang]=Col-2;
          leftline[hang]=1;
          midline[hang]=(leftline[hang]+rightline[hang])/2; 
        }
        //ȫ����
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]!=0))
        {
          midline[hang]=(leftline[hang]+rightline[hang])/2; 
        } 
      } 
    }  
  }
}
//ȡ���ֵ
int max(int a,int b)
{
  return a>b?a:b;
}
//��ȡͼ�������
//ѡȡ���У���ͼ��ײ�����ɨ��
void GetBlackEndParam(void)//��ȡ���߽�ֹ��
{
  
  unsigned char LEndFlag  = 0;//��־λ
  unsigned char MEndFlag  = 0;
  unsigned char REndFlag  = 0;	
  unsigned char MREndFlag = 0;
  unsigned char MLEndFlag = 0;
  unsigned char LLEndFlag = 0;
  unsigned char RREndFlag = 0;

  int i=0;

  BlackEndMR=0;//����
  BlackEndML=0;
  BlackEndLL=0;
  BlackEndRR=0;
  BlackEndL=0;
  BlackEndM=0;
  BlackEndR=0;
  
  for (i=Row-1;i>0;i--)
  {
	if(Pixle[i][Col/2] == white && !MEndFlag )//
        {
		BlackEndM++;//�к��߽�����
        }
	else if(i > 1 && Pixle[i-1][Col/2] == black && Pixle[i-2][Col/2] == black)//���������Ǻ�ɫ        
        {
		MEndFlag = 1;
        }
	if(Pixle[i][Col/4] == white && !LEndFlag )
        {
		BlackEndL++;//����߽�����
        }
	else if(i > 1 && Pixle[i-1][Col/4] == black && Pixle[i-2][Col/4] == black)
        {
		LEndFlag = 1;
        }
	if(Pixle[i][Col*3/4] == white && !REndFlag )
        {
		BlackEndR++;//�Һ��߽�����
	}
	else if(i > 1 && Pixle[i-1][Col*3/4] == black && Pixle[i-2][Col*3/4] == black)
        {
		REndFlag = 1;
        }
        if(Pixle[i][45] == white && !MLEndFlag )
        {
		BlackEndML++;
        }
	else if(i > 1 && Pixle[i-1][45] == black && Pixle[i-2][45] == black)
        {
		MLEndFlag = 1;
        }
	if(Pixle[i][75] == white && !MREndFlag )
        {
		BlackEndMR++;
        }
	else if(i > 1 && Pixle[i-1][75] == black && Pixle[i-2][75] == black)
        {
		MREndFlag = 1;
	}
        if(Pixle[i][5] == white && !LLEndFlag )
        {
		BlackEndLL++;
	}
	else if(i > 1 && Pixle[i-1][5] == black && Pixle[i-2][5] == black)
        {
		LLEndFlag = 1;
	}
        if(Pixle[i][115] == white && !RREndFlag )
        {
		BlackEndRR++;
	}
	else if(i > 1 && Pixle[i-1][115] == black && Pixle[i-2][115] == black)
        {
		RREndFlag = 1;
	}
   }
        BlackEndMax =max(BlackEndL,BlackEndM);//ȡ��ֵ
        BlackEndMax =max(BlackEndMax,BlackEndR);
        BlackEndMaxMax =max(BlackEndMax,BlackEndMR);
        BlackEndMaxMax =max(BlackEndMax,BlackEndML);
        BlackEndMaxMax =max(BlackEndMax,BlackEndLL);
        BlackEndMaxMax =max(BlackEndMax,BlackEndRR);
        if(BlackEndMaxMax>60)
        {
            BlackEndMaxMax=58;
        }
        DropRow=60-BlackEndMaxMax;//�ⶥ������ 

        if(DropRow>=55) DropRow=55;
        
        if(DropRow<=6) DropRow=6;
 }


void scratch_line(void)
{
  scratch_cnt=0;
  //������������
  for(uint8 hang=50;hang<55;hang++)
      {
        for(uint8 lie=30;lie<Col-30;lie++)
        {
          if(image[hang][lie]-image[hang][lie-1]>=15)
            scratch_cnt++;
        }
      }
  //�ж�������
  if(scratch_cnt>=50&&distance_scratch>400)
    scratchflag=1;
}
/**************************************
���ߺ���
start da
end xiao
**************************************/
void connect(int hang_start,int hang_end,int p[])
{
  float temp=0;
  if((hang_end>0)&&(hang_start>0)&&(hang_start<=Row-1)&&(hang_end<=Row-1))    //��ֹ�����������㿨��
    if(hang_start>hang_end)
    {
      temp=(p[hang_end]-p[hang_start])*1.0/(hang_end-hang_start);
      for(uint8 hang=hang_start;hang>hang_end-1;hang--)
        p[hang]=(int)(p[hang_start]-temp*(hang_start-hang));
    }
} 
/**************************************
���Իع鷽�̼���б��   
startlineС
endline��
**************************************/
int regression(int startline,int endline,int p[])    
{
  int i,B;
  int sumX=0,sumY=0,avrX=0,avrY=0 ;
  int num=0,B_up1=0,B_up2=0,B_up,B_down;
   for(i=startline;i<=endline;i++)
   {
            num++;
            sumX+=i;
            sumY+=p[i];
   }
   avrX=sumX/num;
   avrY=sumY/num;
   B_up=0;
   B_down=0;
   for(i=startline;i<=endline;i++)
   {      
     B_up1=(p[i]-avrY);
     B_up2=i-avrX;
     B_up+=10*(B_up1*B_up2);
     B_up=B_up/100*100;
     B_down+=10*(i-avrX)*(i-avrX);
   }
   if(B_down==0) 
    B=0;
   else 
    B=B_up*25/B_down;
   return B;
}
