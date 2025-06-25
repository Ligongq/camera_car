#include "headfile.h"
//��· ��� Բ�� ʮ��
void get_route(void)
{
  //����Ѱ��
  get_route_one(Row-1,Col/2);
  get_route_all();
  //������Ϣ��ȡ
  GetBlackEndParam();
  //��ֹ���ж�
  get_route_endline();
    
//  if(ringflag==0&&cutrouteflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0)
//  {
    //�µ�  ʱ��΢��
    scratch_line();
//  }

  if(ringflag==0&&cutrouteflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0)
  {
    //�µ�  ʱ��΢��
    ramp_find();
  }
  if(ringflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0)
  {
    //���  ��ֵ΢��
    obstruct_find_plain();
    if(obstructflag==1)
      cutrouteflag=0;
  }
  if(ringflag==0&&cutrouteflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0)
  {
    //��·
    cutroute_find();
  }
  if(ringflag==0&&cutrouteflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0)  
  {
    //Բ��
    ring_find();
  }
  if(ringflag==0&&cutrouteflag==0&&obstructflag==0&&rampflag==0&&scratchflag==0&&scratchflag==0)  
  {
    //ʮ��
    cross_find();
  }
  
  //ȫԪ�ؿ���
  gpio_set(B5,1);
  if(obstructflag!=0||cutrouteflag!=0||ringflag!=0||scratchflag==1||xie_cross_flag==1||ru_cross_flag==1||chu_cross_flag==1||rampflag!=0)
    gpio_set(B5,0);
    
  //Ԫ�ش���
  ramp_deal();
  cross_deal();
  ring_start();
  ring_ru();
  ring_chu();
  obstruct_deal();
  cut_route_process();
  //���
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
    //����ж�
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


