#include "headfile.h"
//断路 横断 圆环 十字
void get_route(void)
{
  //基础寻线
  get_route_one(Row-1,Col/2);
  get_route_all();
  //赛道信息提取
  GetBlackEndParam();
  //截止行判断
  get_route_endline();
    
//  if(ringflag==0&&cutrouteflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0)
//  {
    //坡道  时间微调
    scratch_line();
//  }

  if(ringflag==0&&cutrouteflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0)
  {
    //坡道  时间微调
    ramp_find();
  }
  if(ringflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0)
  {
    //横断  阈值微调
    obstruct_find_plain();
    if(obstructflag==1)
      cutrouteflag=0;
  }
  if(ringflag==0&&cutrouteflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0)
  {
    //断路
    cutroute_find();
  }
  if(ringflag==0&&cutrouteflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0)  
  {
    //圆环
    ring_find();
  }
  if(ringflag==0&&cutrouteflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0&&scratchflag==0)  
  {
    //十字
    cross_find();
  }
  
  //全元素开响
  gpio_set(B5,1);
  if(obstructflag!=0||cutrouteflag!=0||ringflag!=0||scratchflag==1||xie_cross_flag==1||ru_cross_flag==1||chu_cross_flag==1||rampflag!=0)
    gpio_set(B5,0);
    
  //元素处理
  ramp_deal();
  cross_deal();
  ring_start();
  ring_ru();
  ring_chu();
  obstruct_deal();
  cut_route_process();
  //拟合
  get_route_midline();
}

int main(void)
{
  int i=0;
    get_clk();
    all_gpio_init();
    OLED_Init();
    FTM_Init();
    camera_init();
    adc_port_init();
    adc_init(ADC0_SE15);
    Dial_Switch();
    //电机中断
    pit_init_ms(pit0,1);
    set_irq_priority(PIT0_IRQn,1);
    enable_irq(PIT0_IRQn);
    while(1)
    {
      if(mt9v032_finish_flag)
      {
        mt9v032_finish_flag=0;
        get_adc_error();
        Get_Pixle();
        get_route();
        get_deviation();
        Track_Line();
        Control_Speed();
        differential_control();
        user_gpio();
        user_oled();
        if(startflag==1)
        {
          i++;
          if(i>=100)
            startflag=2;
        }
      }                         
    }
}


