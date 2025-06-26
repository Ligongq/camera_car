#include "headfile.h"
#include "isr.h"
void ad_collection1();
void fenqvtiaosu();	
void smotor_control(float i);
void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();     //调用SDK自带的中断函数 这个函数最后会调用我们设置的回调函数
    __DSB();                    //数据同步隔离
}
extern uint8 ajbz;
extern float dgwc;
extern uint16 seep_lift;
extern uint16 seep_right;
int ajbz2=0;
int sdlift,sdright;
float jiaodu=0;
extern int qidongtiaosu;
extern int16 smotor_angle;
extern char ai;
char waibubz=0;
char stop=0;
int baohutime=0;
char bz1=0,bz2=0,bz3=0,bz4=0,bz5=0;
int time4ms=0,time4ms1=0;
unsigned long maichong=0,shi=0;
int time1s=0;
extern int16 duojijiaodu;
extern float dongtaiwucha;
void PIT_IRQHandler(void)
{
    if(PIT_FLAG_GET(PIT_CH0))
    {
        PIT_FLAG_CLEAR(PIT_CH0);
        // if((((gpio_get(C5)==0)||(ajbz==1))&&(ai==0))&&(qidongtiaosu==0))
		// {
		// 	smotor_control(smotor_angle+pidinit());
		// }
		//else if(((gpio_get(C5)==0)||(ajbz==1))&&(ai==1))
		//{
			smotor_control(pidinit());
		//}        
    }
    
    if(PIT_FLAG_GET(PIT_CH1))
    {
        PIT_FLAG_CLEAR(PIT_CH1);
		ad_collection1();	
    }
    
    if(PIT_FLAG_GET(PIT_CH2))
    {
        PIT_FLAG_CLEAR(PIT_CH2);
    }
    
    if(PIT_FLAG_GET(PIT_CH3))
    {
        PIT_FLAG_CLEAR(PIT_CH3);
		time1s++;
		if(time1s>=5)
		{
			time1s=0;
			//PRINTF("%d,%d\r\n",duojijiaodu,(int)dongtaiwucha);
		}
		if(bz4==1)
		{
			time4ms1++;
			bz5=time4ms1/200;
			if(bz5>=3)
			{
				bz5=0;
				time4ms1=0;
			}
		}
		if(bz1==1)
		{
			time4ms++;
			if(time4ms>=1000)
			{
				time4ms=0;
				bz2=0;
				bz1=0;
			}
		}
		if(((gpio_get(C6)==0)||(ajbz2==1))&&(qidongtiaosu==0))
		{
			ajbz2=1;

			sdlift=fabs(qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0 ));
			sdright=fabs(qtimer_quad_get(QTIMER_1,QTIMER1_TIMER2_C2));
			if(waibubz==1)
			{
				maichong+=sdlift;
				if(maichong>=10000)
				{
					maichong=0;
					shi++;
				}
			}
			qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0 );
			qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2 );	
			chashu();
			fenqvtiaosu();	
			if((sdlift==0||sdright==0))
			{
				baohutime++;
				if(baohutime>=50)
				{
					maichong=0;
					shi=0;
					waibubz=0;
					ajbz2=0;
					baohutime=0;
					seep_lift=0;
					seep_right=0;
					pwm_duty(PWM1_MODULE3_CHB_D1,0);
					pwm_duty(PWM1_MODULE3_CHA_D0,0);
				}
				else
				{
					pwm_duty(PWM1_MODULE3_CHB_D1,pid_sudu_lift(seep_lift,sdlift,0));
					pwm_duty(PWM1_MODULE3_CHA_D0,pid_sudu2_right(seep_right,sdright,0));
				}
			}	
			else
			{
					pwm_duty(PWM1_MODULE3_CHB_D1,pid_sudu_lift(seep_lift,sdlift,0));
					pwm_duty(PWM1_MODULE3_CHA_D0,pid_sudu2_right(seep_right,sdright,0));
			}

		}
    }
    __DSB();
}


// void GPIO3_Combined_16_31_IRQHandler(void)
// {
//     if(GET_GPIO_FLAG(D27))
//     {
//         CLEAR_GPIO_FLAG(D27);//清除中断标志位
// 		my_delay(100);
// 		if(gpio_get(D27)==0)
// 		{	
// 			bz1=1;
// 			if(bz2==0)
// 			{
// 				bz2=1;
// 				waibubz++;
// 			}
// 		}
//     }  
// }

// int shachebz=0;
// void stop1()
// {
// 	if((waibubz==(1+qvanshu))) //ajbz2==1是否有bug
// 	{
// 		maichong=0;
// 		shi=0;
// 		ajbz2=0;
// 		bz4=1;
// 		shachebz=shache;
// 		switch (bz5)
// 		{
// 			case 0: pwm_duty(PWM1_MODULE3_CHB_D1,0);
// 					pwm_duty(PWM1_MODULE3_CHA_D0,0); 
// 					break;
// 			case 1:	if(shachebz==1)
// 					{
// 						gpio_set(D2, 0);						 //左
// 						gpio_set(D3, 0);						//右
// 						pwm_duty(PWM1_MODULE3_CHB_D1,11000);
// 						pwm_duty(PWM1_MODULE3_CHA_D0,11000);
// 						break;
// 					}
// 					else 
// 						break;
// 			case 2:	pwm_duty(PWM1_MODULE3_CHB_D1,0);
// 					pwm_duty(PWM1_MODULE3_CHA_D0,0);
// 					gpio_set(D2, 1);						 //左
// 					gpio_set(D3, 1);
// 			        waibubz=0;
// 					bz4=0;
// 					bz5=0;
// 					time4ms1=0;
// 					break;
// 		}
// 	}
// }



/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了周期定时器中断
void PIT_IRQHandler(void)
{
    //务必清除标志位
    __DSB();
}
记得进入中断后清除标志位
CTI0_ERROR_IRQHandler
CTI1_ERROR_IRQHandler
CORE_IRQHandler
FLEXRAM_IRQHandler
KPP_IRQHandler
TSC_DIG_IRQHandler
GPR_IRQ_IRQHandler
LCDIF_IRQHandler
CSI_IRQHandler
PXP_IRQHandler
WDOG2_IRQHandler
SNVS_HP_WRAPPER_IRQHandler
SNVS_HP_WRAPPER_TZ_IRQHandler
SNVS_LP_WRAPPER_IRQHandler
CSU_IRQHandler
DCP_IRQHandler
DCP_VMI_IRQHandler
Reserved68_IRQHandler
TRNG_IRQHandler
SJC_IRQHandler
BEE_IRQHandler
PMU_EVENT_IRQHandler
Reserved78_IRQHandler
TEMP_LOW_HIGH_IRQHandler
TEMP_PANIC_IRQHandler
USB_PHY1_IRQHandler
USB_PHY2_IRQHandler
ADC1_IRQHandler
ADC2_IRQHandler
DCDC_IRQHandler
Reserved86_IRQHandler
Reserved87_IRQHandler
GPIO1_INT0_IRQHandler
GPIO1_INT1_IRQHandler
GPIO1_INT2_IRQHandler
GPIO1_INT3_IRQHandler
GPIO1_INT4_IRQHandler
GPIO1_INT5_IRQHandler
GPIO1_INT6_IRQHandler
GPIO1_INT7_IRQHandler
GPIO1_Combined_0_15_IRQHandler
GPIO1_Combined_16_31_IRQHandler
GPIO2_Combined_0_15_IRQHandler
GPIO2_Combined_16_31_IRQHandler
GPIO3_Combined_0_15_IRQHandler
GPIO3_Combined_16_31_IRQHandler
GPIO4_Combined_0_15_IRQHandler
GPIO4_Combined_16_31_IRQHandler
GPIO5_Combined_0_15_IRQHandler
GPIO5_Combined_16_31_IRQHandler
WDOG1_IRQHandler
RTWDOG_IRQHandler
EWM_IRQHandler
CCM_1_IRQHandler
CCM_2_IRQHandler
GPC_IRQHandler
SRC_IRQHandler
Reserved115_IRQHandler
GPT1_IRQHandler
GPT2_IRQHandler
PWM1_0_IRQHandler
PWM1_1_IRQHandler
PWM1_2_IRQHandler
PWM1_3_IRQHandler
PWM1_FAULT_IRQHandler
SEMC_IRQHandler
USB_OTG2_IRQHandler
USB_OTG1_IRQHandler
XBAR1_IRQ_0_1_IRQHandler
XBAR1_IRQ_2_3_IRQHandler
ADC_ETC_IRQ0_IRQHandler
ADC_ETC_IRQ1_IRQHandler
ADC_ETC_IRQ2_IRQHandler
ADC_ETC_ERROR_IRQ_IRQHandler
PIT_IRQHandler
ACMP1_IRQHandler
ACMP2_IRQHandler
ACMP3_IRQHandler
ACMP4_IRQHandler
Reserved143_IRQHandler
Reserved144_IRQHandler
ENC1_IRQHandler
ENC2_IRQHandler
ENC3_IRQHandler
ENC4_IRQHandler
TMR1_IRQHandler
TMR2_IRQHandler
TMR3_IRQHandler
TMR4_IRQHandler
PWM2_0_IRQHandler
PWM2_1_IRQHandler
PWM2_2_IRQHandler
PWM2_3_IRQHandler
PWM2_FAULT_IRQHandler
PWM3_0_IRQHandler
PWM3_1_IRQHandler
PWM3_2_IRQHandler
PWM3_3_IRQHandler
PWM3_FAULT_IRQHandler
PWM4_0_IRQHandler
PWM4_1_IRQHandler
PWM4_2_IRQHandler
PWM4_3_IRQHandler
PWM4_FAULT_IRQHandler
Reserved171_IRQHandler
GPIO6_7_8_9_IRQHandler*/



