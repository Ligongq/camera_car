#include "fuzzy.h"

//p�����
unsigned int rule_p[7][7]={ 
//���仯�� -3,-2,-1, 0, 1, 2, 3      // ���     
             {6, 5, 4, 3, 2, 0, 0,},   //   -3   
			 {5, 4, 3, 2, 1, 0, 1,},   //   -2 
			 {4, 3, 2, 1, 0, 1, 2,},   //   -1 
			 {3, 2, 1, 0, 1, 2, 3,},   //    0 
			 {2, 1, 0, 1, 2, 3, 4,},   //    1 
			 {1, 0, 1, 2, 3, 4, 5,},   //    2 
			 {0, 0, 2, 3, 4, 5, 6}};   //    3 
////d�����
//unsigned int rule_d[7][7]={ 
//���仯�� -3,-2,-1, 0, 1, 2, 3      // ���     
//			 {2, 2, 6, 5, 6, 4, 2,},   //   -3   
//			 {1, 2, 5, 4, 3, 1, 0,},   //   -2 
//			 {0, 1, 3, 3, 1, 1, 0,},   //   -1 
//			 {0, 1 , 1, 1, 1, 1, 0,},  //    0 
//			 {0, 0, 0, 0, 0, 0, 0,},   //    1 
//			 {5, 1, 1, 1, 1, 1, 1,},   //    2 
//			 {6, 4, 4, 3, 3, 1, 1}};   //    3 

unsigned int rule_d[7][7]={ 
//���仯�� -3,-2,-1, 0, 1, 2, 3      // ���     
             {6, 5, 4, 3, 2, 0, 0,},   //   -3   
			 {5, 4, 3, 2, 1, 0, 1,},   //   -2 
			 {4, 3, 2, 1, 0, 1, 2,},   //   -1 
			 {3, 2, 1, 0, 1, 2, 3,},   //    0 
			 {2, 1, 0, 1, 2, 3, 4,},   //    1 
			 {1, 0, 1, 2, 3, 4, 5,},   //    2 
			 {0, 0, 2, 3, 4, 5, 6}};   //    3 

              //0  1   2   3   4   5   6 
int16 PFF[4] = {0,125,250,375}; 
int16 DFF[4] = {0,25,50,75};  //0 24 72 96                      //1�����仯�����ò���
/*�����U����ֵ������ 0     1   2     3   4    5    6  */       //2���ʵ����ڹ����
//float UFF_P[7] = {0.05,0.08,0.11,0.14,0.16,0.18,0.21};
//float UFF_D[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};

//float UFF_P[7] = {0.1,0.13,0.16,0.19,0.22,0.25,0.28};
//float UFF_D[7] = {0.05,0.08,0.11,0.14,0.17,0.2,0.23};

float UFF_P[7] = {0.1,0.13,0.16,0.19,0.22,0.25,0.28};
float UFF_D[7] = {0.5,1,1.5,2,2.5,3,4};


int16 PMAX = 2000;
int16 PMIN = -2000; 
int16 DMAX = 120;    //500
int16 DMIN = -120;
int16 P_bestMAX = 500;//��0~500��ֵ(��һ��)
int16 P_bestMIN = -500;
int16 D_bestMAX = 100;//��0~100��ֵ(��һ��)  
int16 D_bestMIN = -100;
int16 FMAX = 100; //����ֵ������ֵ

/*****************
P �������ܳ�Ѱ�������ֵ
D ���ֵ�ı仯��
ģ�������㷨ͨ���������������Լ�����������ǰӦ�õ�����Kp,Ki,Kdֵ
*******************/
float Fuzzy_Kp(int16 P, int16 D)   //ģ����������,����Kpֵ
{
  uint16 PF[2];         //ƫ��������        PF[0]�����Ȳ���
  uint16 DF[2];         //ƫ��΢��������    DF[0]�����Ȳ���
  uint16 UF[4];         //���ֵ��������    ƫ��,ƫ��΢���Լ����ֵ��������PF[1]��P�������ȣ�PF[0]�������ȵĲ���
  int16 Pn = 0, Dn = 0; //�Ǳ�
  int16 Un[4];          //��Ӧ����KP��ֵ
  int32 temp1, temp2;
  float out=0 ;         //���ֵ�ľ�ȷ�� 
  float Un_out[4]; 
  
  //����޷�
  if (P < PMIN)
    P = PMIN;
  if (P > PMAX)
    P = PMAX;
  if (D < DMIN)
    D = DMIN;
  if (D > DMAX)
    D = DMAX;
  
  P = (int16)(((double)(P - PMIN)) / (PMAX - PMIN) * (P_bestMAX - P_bestMIN) + P_bestMIN); //��һ����0~500
  D = (int16)(((double)(D - DMIN)) / (DMAX - DMIN) * (D_bestMAX - D_bestMIN) + D_bestMIN); //��һ����0~300
  
  /*����E Ec��ָ������ֵ�����Ч������*/ 
  //Ѱ��e�������� 
 //  P=fabs(P);
 //  D=fabs(D);
 //-480      480   P��һ����
   if(P>-PFF[3] && P<PFF[3])//E�ı仯�ڷ�ֵ�� 
   { 
                /*-360*/
		 if(P<=-PFF[2])     
         {
            Pn=-2;
            PF[0]=(int)(FMAX*((float)(-PFF[2]-P)/(PFF[3]-PFF[2])));
      	 } 
    	 else if(P<=-PFF[1])   
     	 {
            Pn=-1;
            PF[0]=(int)(FMAX*((float)(-PFF[1]-P)/(PFF[2]-PFF[1])));
		 } 
		 else if(P<=PFF[0])   
		 {
			 Pn=0;
			 PF[0]=(int)(FMAX*((float)(-PFF[0]-P)/(PFF[1]-PFF[0])));
		 } 
		 else if(P<=PFF[1])   
    	 {
            Pn=1; 
            PF[0]=(int)(FMAX*((float)(PFF[1]-P)/(PFF[1]-PFF[0])));
         } 
         else if(P<=PFF[2]) 
    	 {
             Pn=2; 
			PF[0]=(int)(FMAX*((float)(PFF[2]-P)/(PFF[2]-PFF[1])));
    	 } 
    	 else if(P<=PFF[3])   
     	 {
         	Pn=3; 
	    	PF[0]=(int)(FMAX*((float)(PFF[3]-P)/(PFF[3]-PFF[2])));
     	 }   
   }
   else if(P<=-PFF[3])  //�޷� 
   {
		Pn=-2;   
		PF[0]=FMAX;
   } 
   else if(P>=PFF[3])   
   {
		 Pn=3;   
		 PF[0]=0;
   } 
   PF[1]=(uint16)(FMAX-PF[0]);  //���ԭ������
   
   if(D>-DFF[3] && D<DFF[3])
   {		
        if(D<=-DFF[2])     
		{
			Dn=-2;
			DF[0]=(int)(FMAX*((float)(-DFF[2]-D)/(DFF[3]-DFF[2])));
		} 
		else if(D<=-DFF[1])   
		{
			Dn=-1;
			DF[0]=(int)(FMAX*((float)(-DFF[1]-D)/(DFF[2]-DFF[1])));
		} 
		else if(D<=DFF[0])   
		{
			Dn=0; 
			DF[0]=(int)(FMAX*((float)(-DFF[0]-D)/(DFF[1]-DFF[0])));
		} 
		else if(D<=DFF[1])   
		{
			Dn=1;
			DF[0]=(int)(FMAX*((float)(DFF[1]-D)/(DFF[1]-DFF[0])));
		} 
		else if(D<=DFF[2])   
		{
			Dn=2; 
			DF[0]=(int)(FMAX*((float)(DFF[2]-D)/(DFF[2]-DFF[1])));
		} 
		else if(D<=DFF[3])   
		{
			Dn=3; 
			DF[0]=(int)(FMAX*((float)(DFF[3]-D)/(DFF[3]-DFF[2])));
		}   
   }
   else if(D<=-DFF[3])   
   {
	Dn=-2; 
	DF[0]=FMAX;
   } 
   else if(D>=DFF[3])   
   {
	 Dn=3;
	 DF[0]=0;
   } 
   DF[1]=(uint16)(FMAX-DF[0]);    

  //ʹ����Χ�Ż���Ĺ����rule[7][7]
  //���ֵʹ��13����������,����ֵ��UFF[7]ָ��
  //һ�㶼���ĸ�������Ч
  
  Un[0]=rule_p[Pn+2][Dn+2]; 
  Un[1]=rule_p[Pn+3][Dn+2]; 
  Un[2]=rule_p[Pn+2][Dn+3];   
  Un[3]=rule_p[Pn+3][Dn+3]; 
  
  if(PF[0]<=DF[0])      // P D ������
	UF[0]=PF[0];    //UF���ֵ������
  else 
	UF[0]=DF[0]; 
	  
  if(PF[1]<=DF[0])
	  UF[1]=PF[1];
  else 
	  UF[1]=DF[0];
	   
  if(PF[0]<=DF[1])
	  UF[2]=PF[0]; 
  else 
	  UF[2]=DF[1]; 
	  
  if(PF[1]<=DF[1])
	  UF[3]=PF[1]; 
  else 
	  UF[3]=DF[1]; 
	  	  
  /*ͬ���������������ֵ���*/ 
  if(Un[0]==Un[1])
  {
      if(UF[0]>UF[1])
	 UF[1]=0;
      else 
         UF[0]=0;
  } 
  
  if(Un[0]==Un[2])
  {
      if(UF[0]>UF[2])
         UF[2]=0;
      else 
         UF[0]=0;
  } 
  
  if(Un[0]==Un[3])
  {
      if(UF[0]>UF[3])
	 UF[3]=0;
      else
	 UF[0]=0;
  } 
  
  if(Un[1]==Un[2])
  {
      if(UF[1]>UF[2])
	 UF[2]=0;
      else
	 UF[1]=0;
  } 
  
  if(Un[1]==Un[3])
  {
      if(UF[1]>UF[3])
	 UF[3]=0;
      else 
         UF[1]=0;
  } 
  
  if(Un[2]==Un[3])
  {
      if(UF[2]>UF[3])
	 UF[3]=0;
      else 
         UF[2]=0;
  } 
  /*���ķ���ģ��*/ 
  /*Un[]ԭֵΪ�������������ţ�ת��Ϊ��������ֵ*/ 
  if(Un[0]>=0)
      Un_out[0]=UFF_P[Un[0]];
  else 
      Un_out[0]=-UFF_P[-Un[0]]; 
	  
  if(Un[1]>=0)
      Un_out[1]=UFF_P[Un[1]];
  else 
      Un_out[1]=-UFF_P[-Un[1]]; 
	  
  if(Un[2]>=0)
      Un_out[2]=UFF_P[Un[2]];
  else
      Un_out[2]=-UFF_P[-Un[2]]; 
	  
  if(Un[3]>=0)
      Un_out[3]=UFF_P[Un[3]];
  else  
      Un_out[3]=-UFF_P[-Un[3]];  
  
  temp1=(int)(UF[0]*Un_out[0]+UF[1]*Un_out[1]+UF[2]*Un_out[2]+UF[3]*Un_out[3]); 
  temp2=UF[0]+UF[1]+UF[2]+UF[3]; 
  out=(float)temp1/temp2; 
  
  return out;
}

/*****************
P �������ܳ�Ѱ�������ֵ
D ���ֵ�ı仯��
ģ�������㷨ͨ���������������Լ�����������ǰӦ�õ�����Kp,Ki,Kdֵ
*******************/
float Fuzzy_Kd(int16 P, int16 D)   //ģ����������,����Kdֵ
{
  uint16 PF[2];
  uint16 DF[2];
  uint16 UF[4];  //ƫ��,ƫ��΢���Լ����ֵ��������PF[1]��P�������ȣ�PF[0]�������ȵĲ���
  int16 Pn = 0, Dn = 0;
  int16 Un[4];
  int32 temp1, temp2;
  float out=0 ;  //���ֵ�ľ�ȷ�� 
  float Un_out[4]; 
  
  //����޷�
  if (P < PMIN)
    P = PMIN;
  if (P > PMAX)
    P = PMAX;
  if (D < DMIN)
    D = DMIN;
  if (D > DMAX)
    D = DMAX;
  
  P = (int16)(((double)(P - PMIN)) / (PMAX - PMIN) * (P_bestMAX - P_bestMIN) + P_bestMIN); //��һ����0~500
  D = (int16)(((double)(D - DMIN)) / (DMAX - DMIN) * (D_bestMAX - D_bestMIN) + D_bestMIN); //��һ����0~300
  
  /*����E Ec��ָ������ֵ�����Ч������*/ 
  //Ѱ��e�������� 
   if(P>-PFF[3] && P<PFF[3])//E�ı仯�ڷ�ֵ�� 
   { 
		 if(P<=-PFF[2])     
		 {
			Pn=-2;
			PF[0]=(int)(FMAX*((float)(-PFF[2]-P)/(PFF[3]-PFF[2])));
		 } 
		 else if(P<=-PFF[1])   
		 {
			Pn=-1;
			PF[0]=(int)(FMAX*((float)(-PFF[1]-P)/(PFF[2]-PFF[1])));
		 } 
		 else if(P<=PFF[0])   
		 {
			 Pn=0;
			 PF[0]=(int)(FMAX*((float)(-PFF[0]-P)/(PFF[1]-PFF[0])));
		 } 
		 else if(P<=PFF[1])   
		 {
			 Pn=1; 
			 PF[0]=(int)(FMAX*((float)(PFF[1]-P)/(PFF[1]-PFF[0])));
		 } 
		 else if(P<=PFF[2]) 
		 {
			 Pn=2; 
			PF[0]=(int)(FMAX*((float)(PFF[2]-P)/(PFF[2]-PFF[1])));
		 } 
		 else if(P<=PFF[3])   
		 { 
			 Pn=3; 
			PF[0]=(int)(FMAX*((float)(PFF[3]-P)/(PFF[3]-PFF[2])));
		 }   
   }
   else if(P<=-PFF[3])  //�޷� 
   {
		Pn=-2;   
		PF[0]=FMAX;
   } 
   else if(P>=PFF[3])   
   {
		Pn=3;   
		PF[0]=0;
   } 
   PF[1]=(uint16)(FMAX-PF[0]);  
   
   if(D>-DFF[3] && D<DFF[3])
   {		
        if(D<=-DFF[2])     
		{
				Dn=-2;
				DF[0]=(int)(FMAX*((float)(-DFF[2]-D)/(DFF[3]-DFF[2])));
		} 
		else if(D<=-DFF[1])   
		{
				Dn=-1;
				DF[0]=(int)(FMAX*((float)(-DFF[1]-D)/(DFF[2]-DFF[1])));
		} 
			else if(D<=DFF[0])   
		{
				Dn=0; 
				DF[0]=(int)(FMAX*((float)(-DFF[0]-D)/(DFF[1]-DFF[0])));
		} 
		else if(D<=DFF[1])   
		{
				Dn=1;
				DF[0]=(int)(FMAX*((float)(DFF[1]-D)/(DFF[1]-DFF[0])));
		 } 
		else if(D<=DFF[2])   
		{
				Dn=2; 
				DF[0]=(int)(FMAX*((float)(DFF[2]-D)/(DFF[2]-DFF[1])));
		} 
		else if(D<=DFF[3])   
		{
				Dn=3; 
				DF[0]=(int)(FMAX*((float)(DFF[3]-D)/(DFF[3]-DFF[2])));
		}   
   }
   else if(D<=-DFF[3])   
   {
	Dn=-2; 
	DF[0]=FMAX;
   } 
   else if(D>=DFF[3])   
   {
	 Dn=3;
	 DF[0]=0;
   } 
   DF[1]=(uint16)(FMAX-DF[0]);    

  //ʹ����Χ�Ż���Ĺ����rule[7][7]
  //���ֵʹ��13����������,����ֵ��UFF[7]ָ��
  //һ�㶼���ĸ�������Ч
  
  Un[0]=rule_d[Pn+2][Dn+2]; 
  Un[1]=rule_d[Pn+3][Dn+2]; 
  Un[2]=rule_d[Pn+2][Dn+3];   
  Un[3]=rule_d[Pn+3][Dn+3]; 
  
  if(PF[0]<=DF[0])
	UF[0]=PF[0];  
  else 
	UF[0]=DF[0]; 
	  
  if(PF[1]<=DF[0])
	  UF[1]=PF[1];
  else 
	  UF[1]=DF[0];
	   
  if(PF[0]<=DF[1])
	  UF[2]=PF[0]; 
  else 
	  UF[2]=DF[1]; 
	  
  if(PF[1]<=DF[1])
	  UF[3]=PF[1]; 
  else 
	  UF[3]=DF[1]; 
	  	  
  /*ͬ���������������ֵ���*/ 
  if(Un[0]==Un[1])
  {
      if(UF[0]>UF[1])
	 UF[1]=0;
      else 
         UF[0]=0;
  } 
  
  if(Un[0]==Un[2])
  {
      if(UF[0]>UF[2])
         UF[2]=0;
      else 
         UF[0]=0;
  } 
  
  if(Un[0]==Un[3])
  {
      if(UF[0]>UF[3])
		UF[3]=0;
      else
		UF[0]=0;
  } 
  
  if(Un[1]==Un[2])
  {
      if(UF[1]>UF[2])
		UF[2]=0;
      else
		UF[1]=0;
  } 
  
  if(Un[1]==Un[3])
  {
      if(UF[1]>UF[3])
		UF[3]=0;
      else 
        UF[1]=0;
  } 
  
  if(Un[2]==Un[3])
  {
      if(UF[2]>UF[3])
		UF[3]=0;
      else 
        UF[2]=0;
  } 
  
  
  /*���ķ���ģ��*/ 
  /*Un[]ԭֵΪ�������������ţ�ת��Ϊ��������ֵ*/ 
  if(Un[0]>=0)
      Un_out[0]=UFF_D[Un[0]];
  else 
      Un_out[0]=-UFF_D[-Un[0]]; 
	  
  if(Un[1]>=0)
      Un_out[1]=UFF_D[Un[1]];
  else 
      Un_out[1]=-UFF_D[-Un[1]]; 
	  
  if(Un[2]>=0)
      Un_out[2]=UFF_D[Un[2]];
  else
      Un_out[2]=-UFF_D[-Un[2]]; 
	  
  if(Un[3]>=0)
      Un_out[3]=UFF_D[Un[3]];
  else  
      Un_out[3]=-UFF_D[-Un[3]];  
  
  temp1=(int)(UF[0]*Un_out[0]+UF[1]*Un_out[1]+UF[2]*Un_out[2]+UF[3]*Un_out[3]); 
  temp2=UF[0]+UF[1]+UF[2]+UF[3]; 
  out=(float)temp1/temp2; 
  
  return out;
}

