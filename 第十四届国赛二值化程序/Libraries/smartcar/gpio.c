#include "headfile.h"

uint8 mode=0;
int speed_flag=0;

//引脚初始化
void all_gpio_init()
{
  //蜂鸣器
  gpio_init(B5,GPO,1);
  //灯
  gpio_init(C0,GPO,1);
  gpio_init(D15,GPO,0);
  gpio_init(A17,GPO,0);
  gpio_init(E26,GPO,0);
  //按键
  gpio_init(A29,GPI,1);  //1
  gpio_init(A25,GPI,1);
  gpio_init(A27,GPI,1);
  port_init(A29,ALT1|PULLUP);
  port_init(A25,ALT1|PULLUP);
  port_init(A27,ALT1|PULLUP);
  //拨码
  gpio_init(A10,GPI,1);
  gpio_init(A11,GPI,1);
  gpio_init(A12,GPI,1);  
  gpio_init(A13,GPI,1); //1
  port_init(A10,ALT1|PULLUP);
  port_init(A11,ALT1|PULLUP);
  port_init(A12,ALT1|PULLUP);
  port_init(A13,ALT1|PULLUP);
  
  gpio_set(C0,0);
  gpio_set(D15,0);
  gpio_set(A17,0);
  gpio_set(E26,0);
  gpio_set(B5,1);
}
//人机交互函数
void gpio_display()
{
  gpio_set(B5,1);
  gpio_set(D8,1);
  gpio_set(D9,1);
  systick_delay_ms(500);
  gpio_set(B5,0);
  gpio_set(D8,0);
  gpio_set(D9,0);
}

void user_gpio()
{
  
  if(gpio_get(A29)==0)
  {
    systick_delay_ms(100);
    if(gpio_get(A29)==0)
    {
      mode++;
    }
  }
  if(mode>8)
    mode=0;
  
}

void Dial_Switch(void)
{
  
  if(gpio_get(A10)==0)
  {
    Min_Speed=480;
    speed_flag=1;
  }
  else if(gpio_get(A11)==0)
  {
    Min_Speed=490;
    speed_flag=2;
  }
  else if(gpio_get(A12)==0)
  {
    Min_Speed=500;
    speed_flag=3;
  }
  else if(gpio_get(A13)==0)
  {
    Min_Speed=510;
    speed_flag=4;
  }
}





