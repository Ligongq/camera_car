#ifndef _GET_IMAGE_H_
#define _GET_IMAGE_H_

#define Row    60               //ͼ������
#define Col    120               //ͼ������

#define white 1
#define black 0

void Get_Use_Image(void);
void Get_Pixle(void);

uint8 my_adapt_threshold(uint8 *image, uint16 col, uint16 row);   //ע�������ֵ��һ��Ҫ��ԭͼ��

extern uint8 Pixle[Row][Col]; 
extern uint8 Gate;

#endif 