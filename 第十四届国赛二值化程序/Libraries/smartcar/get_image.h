#ifndef _GET_IMAGE_H_
#define _GET_IMAGE_H_

#define Row    60               //图像行数
#define Col    120               //图像列数

#define white 1
#define black 0

void Get_Use_Image(void);
void Get_Pixle(void);

uint8 my_adapt_threshold(uint8 *image, uint16 col, uint16 row);   //注意计算阈值的一定要是原图像

extern uint8 Pixle[Row][Col]; 
extern uint8 Gate;

#endif 