#include "headfile.h"
/******************************************************************************
��ȡͼ��
����㷨����·�޷�
******************************************************************************/
//��ֵ��ͼ��
uint8 Pixle[Row][Col]; 
//�Ҷ�ͼ��
extern uint8 image[ROW][COL];   
//��ֵ����ֵ
uint8 Gate=0;
//��·��ֵ   �ص�
#define cut_lost 80

//�õ���ֵ��ͼ��
void Get_Pixle(void)
{ 

  Gate=my_adapt_threshold(*image,Col,Row)+5;   //*******************************
  
  if(cutrouteflag==1)
  {
    if(Gate<cut_lost)  
      Gate=cut_lost;
  }
  //��ֵ��
  for(uint8 hang=0;hang<Row;hang++)
    for(uint8 lie=0;lie<Col;lie++)
    {
      if(image[hang][lie]>=Gate)
        Pixle[hang][lie]=white;
      else
        Pixle[hang][lie]=black;
      
    }
}

uint8 my_adapt_threshold(uint8 *image, uint16 col, uint16 row)   //ע�������ֵ��һ��Ҫ��ԭͼ��
{
   #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold = 0;
    uint8* data = image;  //ָ���������ݵ�ָ��
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }
    
    uint32 gray_sum=0;
    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
    for (i = 0; i < height; i+=2)
    {
        for (j = 0; j < width; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
            gray_sum+=(int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
        }
    }
                      
    //����ÿ������ֵ�ĵ�������ͼ���еı���  
  
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        
    }

    //�����Ҷȼ�[0,255]  
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    
     
        
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)         
        {
            
                w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
                u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ 
           
               w1=1-w0;
               u1tmp=gray_sum/pixelSum-u0tmp;
        
                u0 = u0tmp / w0;              //����ƽ���Ҷ�
                u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
                u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
                deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
                if (deltaTmp > deltaMax)
                {
                    deltaMax = deltaTmp;
                    threshold = j;
                }
                if (deltaTmp < deltaMax)
                {
                break;
                }
          
         }

    return threshold;

}
