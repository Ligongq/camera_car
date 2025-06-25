#include "headfile.h"

int startflag=0;

//显示边线
void display_line(void)
{
  uint8 line[Row][Col];
  for(uint8 hang=0;hang<Row;hang++)
    for(uint8 lie=0;lie<Col;lie++)
      line[hang][lie]=0;
  for(uint8 hang=0;hang<Row;hang++)
  {
    if(hang>endline)
    {
      line[hang][midline[hang]]=1;
      line[hang][leftline[hang]]=1;
      line[hang][rightline[hang]]=1;
    } 
  }     
  dis_bmp(Row,Col,line[0],0);                  
}

void user_oled()
{
  switch(mode)
  {
    case 0:
    {
      if(gpio_get(A27)==0)
      {
        systick_delay_ms(100);
        if(gpio_get(A27)==0)
        {
          startflag=1;
        }
      }
      break;
    }
    case 1:
    {
      OLED_Fill(0x00);
      //显示图像和截止行
      for(uint8 lie=0;lie<Col;lie++)
      {
        Pixle[endline][lie]=white;
      }
      dis_bmp(Row,Col,Pixle[0],0); 
      break;
    }
    case 2:
    {
      //显示边线
      OLED_Fill(0x00);
      display_line();
      OLED_Print_Num1(90,0,(int16)Gate);
      break;
    }
    case 3:
    {
      OLED_Fill(0x00);
      //显示电感值
      OLED_P6x8Str(0,0,"zuo");
      OLED_Print_Num1(80,0,(int16)zuo);
      OLED_P6x8Str(0,1,"you");
      OLED_Print_Num1(80,1,(int16)you);
      OLED_P6x8Str(0,2,"midzuo");
      OLED_Print_Num1(80,2,(int16)midzuo);
      OLED_P6x8Str(0,3,"midyou");
      OLED_Print_Num1(80,3,(int16)midyou);
      OLED_P6x8Str(0,4,"Error");
      OLED_Print_Num1(80,4,(int16)Error);
      OLED_P6x8Str(0,5,"distance");
      OLED_Print_Num1(80,5,(int16)distance);
      OLED_P6x8Str(0,6,"scratchcnt");
      OLED_Print_Num1(80,6,(int16)scratch_cnt);
      break;
    }
    case 4:
    {
      OLED_Fill(0x00);
      //各种标志位
      OLED_P6x8Str(0,0,"ring");
      OLED_Print_Num1(80,0,(int16)ringflag);
      OLED_P6x8Str(0,1,"cut");
      OLED_Print_Num1(80,1,(int16)cutrouteflag);
      OLED_P6x8Str(0,2,"ob");
      OLED_Print_Num1(80,2,(int16)obstructflag);
      OLED_P6x8Str(0,3,"ramp");
      OLED_Print_Num1(80,3,(int16)rampflag);
      OLED_P6x8Str(0,4,"scratch");
      OLED_Print_Num1(80,4,(int16)scratchflag);
//      OLED_P6x8Str(0,5,"astern");
//      OLED_Print_Num1(80,5,(int16)asternflag);
      OLED_P6x8Str(0,6,"scratchcnt");
      OLED_Print_Num1(80,6,(int16)scratch_cnt);
      break;
    }
    case 5:
    {
      //过障碍左右
      OLED_Fill(0x00);
      if(gpio_get(A25)==0)
      {
        systick_delay_ms(100);
        if(gpio_get(A25)==0)
        {
          obstruct_direction=1;
        }
      }
      if(gpio_get(A27)==0)
      {
        systick_delay_ms(100);
        if(gpio_get(A27)==0)
        {
          obstruct_direction=0;
        }
      }
      OLED_P8x16Str(0,0,"ob direction");
      if(obstruct_direction==0)
        OLED_P8x16Str(0,2,"zuo");
      else
      {
        OLED_P8x16Str(0,2,"you");
      }
      break;
    }
    case 6:
    {
      OLED_Fill(0x00);
      OLED_P8x16Str(0,0,"obstruct_speed");
      OLED_Print_Num1(0,3,(int16)obstruct_speed);
      if(gpio_get(A25)==0)
      {
        systick_delay_ms(100);
        if(gpio_get(A25)==0)
        {
          obstruct_speed+=5;
        }
      }
      if(gpio_get(A27)==0)
      {
        systick_delay_ms(100);
        if(gpio_get(A27)==0)
        {
          obstruct_speed-=5;
        }
      }
      break;
      break;
    }
    case 7:
    {
      OLED_Fill(0x00);
      OLED_P8x16Str(0,0,"cutroute_speed");
      OLED_Print_Num1(0,3,(int16)cutroute_speed);
      if(gpio_get(A25)==0)
      {
        systick_delay_ms(100);
        if(gpio_get(A25)==0)
        {
          cutroute_speed+=5;
        }
      }
      if(gpio_get(A27)==0)
      {
        systick_delay_ms(100);
        if(gpio_get(A27)==0)
        {
          cutroute_speed-=5;
        }
      }
      break;
    }
    case 8:
    {
      OLED_Fill(0x00);
      OLED_P8x16Str(0,0,"Min_Speed");
      OLED_Print_Num1(0,3,(int16)Min_Speed);
      if(gpio_get(A25)==0)
      {
        systick_delay_ms(100);
        if(gpio_get(A25)==0)
        {
          Min_Speed+=5;
        }
      }
      if(gpio_get(A27)==0)
      {
        systick_delay_ms(100);
        if(gpio_get(A27)==0)
        {
          Min_Speed-=5;
        }
      }
      break;
    }
  }
}




