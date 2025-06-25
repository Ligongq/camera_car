#include "headfile.h"
/*********************************************************************
基础寻线
第一行特殊处理：分起跑线和正常赛道 （稳定）
跟踪寻线 进行了优化，上一行找到给范围找线，上一行丢全行找（稳定）
截止行寻线 圆环截掉与十字截掉较多 但比较好使
中线拟合 拟合赛道宽度，分直道弯道  (稳定)
赛道特征提取 用于判断直道补线（稳定）
**********************************************************************/
//存左右中边线数组 1-118
int leftline[Row];
int midline[Row];
int rightline[Row];
//存储标志 2可信 1不一定可信 0丢线
int leftlineflag[Row];
int rightlineflag[Row];
//截止行 55-0
int endline=0;
//计算真实赛道宽度
int route_width[Row];
//真实赛道宽度
int track_width[Row]={5,6,7,8,9,10,11,13,15,17,
                      19,21,23,25,26,27,29,31,33,34,
                      36,38,40,41,43,45,47,49,51,53,
                      55,56,58,60,63,64,66,67,68,69,
                      73,75,77,79,81,83,85,87,88,91,
                      93,94,95,97,99,101,102,104,106,106};
int width[Row];
//拟合的赛道宽度
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


//第一行图像寻线
//起跑线与其他图像寻线方式区别
void get_route_one(uint8 hang,uint8 start)
{
  //清零
  for(uint8 hang=0;hang<Row;hang++)
  {
    leftline[hang]=0;
    rightline[hang]=Col-1;
    leftlineflag[hang]=0;
    rightlineflag[hang]=0;
    midline[hang]=Col/2;
  }
  endline=0;
  //检测本行起跑线标志位
  int baihei=0;
  for(uint8 lie=0;lie<Col-1;lie++)
  {
    if(Pixle[hang][lie]==white&&Pixle[hang][lie+1]==black)
      baihei++;
  }
  //起跑线寻线
  if(baihei>3)
  {
    if(Pixle[hang][5]==white)
    {
      //向左找
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
      //向右找
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
    //如果没找到，直接给定值
    if(leftlineflag[hang]==0)
    {
      leftline[hang]=1;
      leftlineflag[hang]=1;
    }
    //右边线
    if(Pixle[hang][Col-5]==white)
    {
      //向右找
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
      //向左找
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
    //如果没找到，直接给定值
    if(rightlineflag[hang]==0)
    {
      rightline[hang]=Col-2;
      rightlineflag[hang]=2;
    }
  }
  else
  {
    //左边线
    if(Pixle[hang][start]==white)
    {
      //向左找
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
      //向右找
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
    //如果没找到，直接给定值
    if(leftlineflag[hang]==0)
    {
      leftline[hang]=1;
      leftlineflag[hang]=2;
    }
    //右边线
    if(Pixle[hang][start]==white)
    {
      //向右找
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
      //向左找
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
    //如果没找到，直接给定值
    if(rightlineflag[hang]==0)
    {
      rightline[hang]=Col-2;
      rightlineflag[hang]=2;
    }
    //如果左右边线都不绝对可信
    if(rightlineflag[hang]==1&&leftlineflag[hang]==1)
    {
      leftline[hang]=1;
      rightline[hang]=Col-2;
    }
    leftlineflag[hang]=1;
    rightlineflag[hang]=1;
  }
}
//跟踪寻线
void get_route_all(void)
{
  //初始化
  int Lstart=0,L_max=0,L_min=0;
  int Rstart=0,R_max=0,R_min=0;
  int range=10;
  for(uint8 hang=Row-2;hang>0;hang--)
  {
	//左边线
	//确定搜线范围
    //根据上一行确定本行寻线点
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
    //左边线
    if(Pixle[hang][Lstart]==black)
    {
      //向右找
      for(uint8 lie=Lstart;lie<L_max;lie++)
      {
        if(Pixle[hang][lie]==black&&Pixle[hang][lie+1]==white)
        {
			leftline[hang]=lie;
			leftlineflag[hang]=1;
			break;
        }
      }
      //如果还没找到说明丢线
      if(leftlineflag[hang]==0)
      {
        leftline[hang]=Col-2;
        leftlineflag[hang]=0;
      }
    }
    else
    {
      //向左找
      for(uint8 lie=Lstart;lie>L_min;lie--)
      {
        if(Pixle[hang][lie]==white&&Pixle[hang][lie-1]==black)
        {
            leftline[hang]=lie;
            leftlineflag[hang]=1;
			break;
        }
      }
      //如果还没找到说明丢线
      if(leftlineflag[hang]==0)
      {
        leftline[hang]=1;
        leftlineflag[hang]=0;
      }
    }
    //右边线
    //确定寻线范围
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
    //右边线
    if(Pixle[hang][Rstart]==black)
    {
      //向左找
      for(uint8 lie=Rstart;lie>R_min;lie--)
      {
        if(Pixle[hang][lie]==black&&Pixle[hang][lie-1]==white)
        {
          rightline[hang]=lie;
          rightlineflag[hang]=1;
          break;
        }
      }
      //如果还没找到
      if(rightlineflag[hang]==0)
      {
        rightline[hang]=1;
        rightlineflag[hang]=0;
      }
    }
    else
    {
      //向右找
      for(uint8 lie=Rstart;lie<R_max;lie++)
      {
        if(Pixle[hang][lie]==white&&Pixle[hang][lie+1]==black)
        {
          rightline[hang]=lie;
          rightlineflag[hang]=1;
          break;
        }
      }
      //如果还没找到
      if(rightlineflag[hang]==0)
      {
        rightline[hang]=Col-2;
        rightlineflag[hang]=0;
      }
    }
  }
  //车最远一行按上一行补
  leftline[0]=leftline[1];
  leftlineflag[0]=leftlineflag[1];
  rightline[0]=rightline[1];
  rightlineflag[0]=rightlineflag[1];
}
//寻截止行
void get_route_endline(void)
{  
  //截止行的判断需要消除折点，这里不破坏原来左右线数组
  int left_line[Row],right_line[Row];
  
  //复制
  for(uint8 hang=Row-1;hang>0;hang--)
  {
    left_line[hang]=leftline[hang];
    right_line[hang]=rightline[hang];
  }
  //消折点（突变点）
  for(uint8 hang=Row-3;hang>0;hang--)                    
  {    
    int temp=0;       
    //左	
    if((left_line[hang+1]-left_line[hang])>0)                  
    {                               
		temp=left_line[hang+1];                                  
		left_line[hang+1]=left_line[hang];
		left_line[hang]=temp;
    }
    //右
    if((right_line[hang]-right_line[hang+1])>0)                 
    {                           
		temp=right_line[hang+1];
		right_line[hang+1]=right_line[hang];
		right_line[hang]=temp;
    }
  }
  endline=DropRow;
  //截止行判断
  for(uint8 hang=Row-5;hang>endline;hang--)
  {
    //隔离赛道外的边线
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
//拟合中线
void get_route_midline(void)
{
  for(uint8 hang=55;hang>endline;hang--)
  {
    //直道补线
    if(BlackEndM>=10)
    {
      //左丢又不丢
      if((leftlineflag[hang]==0)&&(rightlineflag[hang]!=0))    
      {
        leftline[hang]=rightline[hang]-Width_P_Z*track_width[hang]-5; 
        midline[hang]=(leftline[hang]+rightline[hang])/2;
      }      
      //右丢左不丢
      if((leftlineflag[hang]!=0)&&(rightlineflag[hang]==0))      
      {
        rightline[hang]=leftline[hang]+Width_P_Z*track_width[hang]+5;
        midline[hang]=(leftline[hang]+rightline[hang])/2;
      }
      //全丢
      if((leftlineflag[hang]==0)&&(rightlineflag[hang]==0))
      {
        rightline[hang]=Col-2;
        leftline[hang]=1;
        midline[hang]=(leftline[hang]+rightline[hang])/2;
      }
      //全不丢
      if((leftlineflag[hang]!=0)&&(rightlineflag[hang]!=0))
      {
        midline[hang]=(leftline[hang]+rightline[hang])/2;
      }
    }
    else
    {
      if(ringflag==3) 
      {
        //左丢又不丢
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]!=0))    
        {
          leftline[hang]=rightline[hang]-Width_P_R_L*track_width[hang]; 
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }       
        //右丢左不丢
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]==0))      
        {
          rightline[hang]=leftline[hang]+Width_P_R_L*track_width[hang];
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }
        //全丢
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]==0))
        {
          rightline[hang]=Col-2;
          leftline[hang]=1;
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }
        //全不丢
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]!=0))
        {
          midline[hang]=(leftline[hang]+rightline[hang])/2;   
        } 
      }
      else if(ringflag==4) 
      {
        //左丢又不丢
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]!=0))    
        {
          leftline[hang]=rightline[hang]-Width_P_R_R*track_width[hang]; 
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }       
        //右丢左不丢
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]==0))      
        {
          rightline[hang]=leftline[hang]+Width_P_R_R*track_width[hang];
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }
        //全丢
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]==0))
        {
          rightline[hang]=Col-2;
          leftline[hang]=1;
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }
        //全不丢
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]!=0))
        {
          midline[hang]=(leftline[hang]+rightline[hang])/2;   
        } 
      }
      else
      {
        //左丢又不丢
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]!=0))    
        {
          leftline[hang]=rightline[hang]-Width_P_W_L*track_width[hang];
          midline[hang]=(leftline[hang]+rightline[hang])/2;
        }      
        //右丢左不丢
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]==0))              
        {       
          rightline[hang]=leftline[hang]+Width_P_W_R*track_width[hang];
          midline[hang]=(leftline[hang]+rightline[hang])/2;       
        }
         //全
        if((leftlineflag[hang]==0)&&(rightlineflag[hang]==0))
        {
          rightline[hang]=Col-2;
          leftline[hang]=1;
          midline[hang]=(leftline[hang]+rightline[hang])/2; 
        }
        //全不丢
        if((leftlineflag[hang]!=0)&&(rightlineflag[hang]!=0))
        {
          midline[hang]=(leftline[hang]+rightline[hang])/2; 
        } 
      } 
    }  
  }
}
//取最大值
int max(int a,int b)
{
  return a>b?a:b;
}
//提取图像的特征
//选取几列，从图像底部往上扫描
void GetBlackEndParam(void)//获取黑线截止行
{
  
  unsigned char LEndFlag  = 0;//标志位
  unsigned char MEndFlag  = 0;
  unsigned char REndFlag  = 0;	
  unsigned char MREndFlag = 0;
  unsigned char MLEndFlag = 0;
  unsigned char LLEndFlag = 0;
  unsigned char RREndFlag = 0;

  int i=0;

  BlackEndMR=0;//清零
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
		BlackEndM++;//中黑线截至行
        }
	else if(i > 1 && Pixle[i-1][Col/2] == black && Pixle[i-2][Col/2] == black)//连续两行是黑色        
        {
		MEndFlag = 1;
        }
	if(Pixle[i][Col/4] == white && !LEndFlag )
        {
		BlackEndL++;//左黑线截至行
        }
	else if(i > 1 && Pixle[i-1][Col/4] == black && Pixle[i-2][Col/4] == black)
        {
		LEndFlag = 1;
        }
	if(Pixle[i][Col*3/4] == white && !REndFlag )
        {
		BlackEndR++;//右黑线截至行
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
        BlackEndMax =max(BlackEndL,BlackEndM);//取大值
        BlackEndMax =max(BlackEndMax,BlackEndR);
        BlackEndMaxMax =max(BlackEndMax,BlackEndMR);
        BlackEndMaxMax =max(BlackEndMax,BlackEndML);
        BlackEndMaxMax =max(BlackEndMax,BlackEndLL);
        BlackEndMaxMax =max(BlackEndMax,BlackEndRR);
        if(BlackEndMaxMax>60)
        {
            BlackEndMaxMax=58;
        }
        DropRow=60-BlackEndMaxMax;//封顶的行数 

        if(DropRow>=55) DropRow=55;
        
        if(DropRow<=6) DropRow=6;
 }


void scratch_line(void)
{
  scratch_cnt=0;
  //起跑线跳变检测
  for(uint8 hang=50;hang<55;hang++)
      {
        for(uint8 lie=30;lie<Col-30;lie++)
        {
          if(image[hang][lie]-image[hang][lie-1]>=15)
            scratch_cnt++;
        }
      }
  //判断起跑线
  if(scratch_cnt>=50&&distance_scratch>400)
    scratchflag=1;
}
/**************************************
连线函数
start da
end xiao
**************************************/
void connect(int hang_start,int hang_end,int p[])
{
  float temp=0;
  if((hang_end>0)&&(hang_start>0)&&(hang_start<=Row-1)&&(hang_end<=Row-1))    //防止数组数量不足卡死
    if(hang_start>hang_end)
    {
      temp=(p[hang_end]-p[hang_start])*1.0/(hang_end-hang_start);
      for(uint8 hang=hang_start;hang>hang_end-1;hang--)
        p[hang]=(int)(p[hang_start]-temp*(hang_start-hang));
    }
} 
/**************************************
线性回归方程计算斜率   
startline小
endline大
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
