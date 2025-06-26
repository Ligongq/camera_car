/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.26
 * @Target core		NXP RT1021DAG5A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-02-18
 ********************************************************************************************************************/


//�����Ƽ�IO�鿴Projecct�ļ����µ�TXT�ı�



//���µĹ��̻��߹����ƶ���λ�����ִ�����²���
//��һ�� �ر��������д򿪵��ļ�
//�ڶ��� project  clean  �ȴ��·�����������




#include "headfile.h"

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "isr.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
volatile uint32_t g_tick;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
 
 #include "nncie.h"
 #define IMAGE_SIZE    (8 + 32*32*3)  // 3080

extern void* RunModel(const void *in_buf);
//q7_t *g_out;
int16 g_out[1];


typedef enum _enum_OutType
{
	outtype_cls = 0,
	outtype_lgsbin = 1,
	outtype_rgrcls = 2,
	outtype_angle = 3,
	outtype_recon = 4,
	outtype_raw = 100,
}outType_e;

typedef struct _ClsTest_t
{
	uint32_t clsCnt;
}ClsTest_t;

typedef struct _TestFileHdr_t
{
	char sMgc[4];	// NCTT
	uint32_t tvSize;
	uint32_t tvCnt;
	uint8_t outType;
	uint8_t isExtTV;	// is using extended test vector format
	uint16_t outDim;
	union {
		float f32Params[2];
		int32_t i32Params[2];
	};
	uint32_t rsvd[2];
}TestFileHdr_t;

typedef struct _TestVec_t
{
	char _0xA5;	// must be 0xA5, else check for TestVecExt_t
	uint8_t w, h, c;
	union {
		uint32_t label;
	};
}TestVec_t;

typedef struct _TestVecExt_t
{
	char magic[4];	//
	uint16_t w, h, c, z;
	union {
		uint32_t label;
	};
}TestVecExt_t;

typedef struct _TestCtx_t
{
	TestFileHdr_t hdr;
	const void* pvTVs;
	uint32_t tvNdx;
	uint8_t isXIP;
}TestCtx_t;

const TestVec_t* TestVecPopNext(TestCtx_t* pCtx) {
	if (pCtx->tvNdx >= pCtx->hdr.tvCnt)
		return 0;
	uint32_t adr = (uint32_t)pCtx->pvTVs + pCtx->tvNdx * pCtx->hdr.tvSize;
	pCtx->tvNdx++;
	return (const TestVec_t*)adr;
}

int TestVecCreateCtxXIP(const void *pvTestData, TestCtx_t* pCtx) {
	int ret = 0;
	memset(pCtx, 0, sizeof(*pCtx));
	memcpy(&pCtx->hdr, pvTestData, sizeof(pCtx->hdr));
	pCtx->pvTVs = (const void*)((uint32_t)pvTestData + sizeof(pCtx->hdr));
	pCtx->isXIP = 1;
	return ret;
}

#if 0
int TestVecCreateCtx(const char* pszTestFile, TestCtx_t* pCtx) {
	int ret = 0;
	FILE* fp = fopen(pszTestFile, "rb");
	uint32_t tvsSize;
	if (0 == fp) {
		ret = -1L;
		goto cleanup;
	}
	memset(pCtx, 0, sizeof(*pCtx));
	fread(&pCtx->hdr, sizeof(pCtx->hdr), 1, fp);
	tvsSize = pCtx->hdr.tvCnt * pCtx->hdr.tvSize;
	pCtx->pvTVs = malloc(tvsSize);
	fread((void*)pCtx->pvTVs, 1, tvsSize, fp);
	fclose(fp);
cleanup:	
	return ret;
}
#endif

int TestVecDestroyCtx(TestCtx_t* pCtx) {
	if (!pCtx->isXIP)
		free((void*)pCtx->pvTVs);
	return 0;
}

float GetAngle(signed char *pv1, signed char *pv2)
{
	int32_t iM1, iM2, iDot;
	float fM1, fM2, fDot;
	int i;
	for (i = 0, iM1 = 0, iM2 = 0, iDot = 0; i<128; i++) {
		iDot += (int32_t)pv1[i] * pv2[i];
		iM1 += (int32_t)pv1[i] * pv1[i];
		iM2 += (int32_t)pv2[i] * pv2[i];
	}
	fDot = (float)iDot;
	fM1 = sqrtf((float)iM1);
	fM2 = sqrtf((float)iM2);
	float cosVal = fDot / (fM1 * fM2);
	float angle = acosf(cosVal) * 180 / 3.141592654f;
	return angle;
}


#define ANGLE_BORDER	59.8f // 64.569822f
int CIETest(const void *pvModel, TestCtx_t *pCtx) {
	int i, j, okCnt = 0;
	const char* pImg;
	float avgAngs[2] = { 0.0f, 0.0f };
	int cnts[2] = { 0, 0 };
	int isSame = 0;
	int avg_mse = 0;
	ModelInfo_t inf;
	CI_GetModelInfoXIP(pvModel, &inf);
	if (inf.pszString)
		PRINTF("%s\r\n", inf.pszString); 
	#ifdef SRC_DEBUG
	uint8_t *pOut;
	int max, maxAt;
	#else
	
	 // CI_RunModelXIP(pvModel, pImg + 8, g_out);
	#endif 
	uint32_t outType = pCtx->hdr.outType;
	uint32_t t0, t1;
	t0 = g_tick;
	for (i = 0; i < 100000; i++) 
	{
		pImg = (const char*) TestVecPopNext(pCtx);
		// pImg = GetTestSample(i);
		if (0 == pImg)
			break;
		#ifdef SRC_DEBUG
		uint8_t *pOut;
		int max, maxAt;
		pOut = (uint8_t*)RunModel(pImg + 8);
		memcpy(g_out, pOut, 128);
		int16_t *pOut16 = (int16_t*)g_out;
		pOut = (int8_t *)g_out;
		for (j = 0, max = -128; j < pCtx->hdr.outDim; j++) 
		{
			if (pOut[j] > max) 
			{
				max = pOut[j];
				maxAt = j;
			}
		}
		g_out[0] = maxAt;
		outType = outtype_cls;
		#else
		const CI_OutMetricInfo* pOM = CI_GetOutMetricInfo(0);
		//CI_RunModelXIP_NoCopyOutput(pvModel, pImg + 8, (void**)&g_out);
		CI_RunModelXIP(pvModel, pImg + 8, g_out);
	  
		#endif
		int16_t g_servoValue;
		if(inf.quantBits>7)
		{
			int16_t *g_out16 = (int16_t *)g_out;
			g_servoValue = g_out16[0] >> (inf.quantBits - pOM->fracBits - 1);
			g_servoValue = g_out[0]>>3;
		}
		else
		{
			g_servoValue = g_out[0];
		}

		//PRINTF("%04d %d got %d -- %d\r\n", i + 1, label,(g_out16[0]), avg_mse);
		avg_mse += (pImg[4] - g_servoValue) * (pImg[4] - g_servoValue);
	}
	 
	 
	t1 = g_tick - t0;
	j = (10000 * okCnt) / i;
	int printOvhd = 0;
	avg_mse=sqrt(avg_mse/i);
	j = avg_mse;
	//t1 = t1 / i;
	PRINTF("%ddms, average %d.%02ddms/inference %d\r\n", t1, t1 / i - printOvhd, (t1 * 100 / i) % 100, i);
	
	PRINTF("%05d\r\n", j);
	return j;
}


#define SYSTICK_PRESCALE 10


int16 smotor_angle,smotor_angle1;			//������Ʊ���

uint8 send_buff[20];		//����ת���ڷ�������


uint8 collection_succes;	//1��ad���ݲɼ����   0��ad����δ�ɼ����
int8 ad1,ad2,ad3,ad4,ad5,ad6,ad7,ad8;//ad����
#define   NM1    5//�˲����
uint8 ajbz=0;
int16  AD_valu1[7],AD_V1[7][NM1];
int16  AD_sum1[7]; 
uint16  AD[7],AD1[7];
float min_v[7]={257,257,257,257,257,257,257};  
float max_v[7]={0,0,0,0,0,0,0};  
float max_v1[7]={0,0,0,0,0,0,0};  
float max_v2[7]={0,0,0,0,0,0,0};  
float max_v3[7]={0,0,0,0,0,0,0};  
float max_v4[7]={0,0,0,0,0,0,0};  
void ad_collection(void)
{
	ad1 = adc_mean_filter(ADC_1,ADC1_CH3_B14 ,10)-128;
    ad2 = adc_mean_filter(ADC_1,ADC1_CH4_B15 ,10)-128;
    ad3 = adc_mean_filter(ADC_1,ADC1_CH5_B16 ,10)-128;
    ad4 = adc_mean_filter(ADC_1,ADC1_CH6_B17 ,10)-128;
    ad5 = adc_mean_filter(ADC_1,ADC1_CH7_B18,10)-128;
    ad6 = adc_mean_filter(ADC_1,ADC1_CH8_B19,10)-128;
    ad7 = adc_mean_filter(ADC_1,ADC1_CH9_B20,10)-128;
	
	collection_succes = 1;
}

/************************************************/
//ţ�ٵ����� ��������
float newtonSqrt(float a)
{  
	int i;  
	float x;  
	x=a;  
	for(i=1;i<=10;i++)  
		x=(x+a/x)/2;  
	return x;  
}
float dongtaiwucha;
float cishu=1;


/*************************�������*******************************/
//==========��¼ת��============
int16 angle_save[20]={0};
void angle_record(int16 j)
{
	static int i=0;
	for(i=0;i<19;i++)
		angle_save[i]=angle_save[i+1];
	angle_save[19]=j;
}
int lostline_flag=0;
int lostline_dir=0;
int16 duojijiaodu=0;
void lost_line_judge()
{
   static int i=0,zuo=0,you=0;// ���Ҽ���  
   if(lostline_flag==0)//���߱�־Ϊ0ʱ����
   {
      if(AD[2]<150) //���붪������
	  {
		  my_delay(1000);
		  if(AD[2]<150) //���붪������
		  {
			lostline_flag=1;//���߱�־��
		  }
	  }
      for(i=10;i<19;i++)//����
	  {
		 if(angle_save[i]<0)you++;
		 if(angle_save[i]>=0)zuo++;
	  }
	  if((zuo>you)&&(AD[0]>AD[4]))
	  {
		 lostline_dir=1;//���߱�־
		 you=0;
		 zuo=0;
	  }
	  else if(AD[0]<AD[4])
	  {
		 lostline_dir=2;//�Ҷ��߱�־
	  }
	  if((zuo<you)&&(AD[0]<AD[4]))
	  {
		 lostline_dir=2;//�Ҷ��߱�־
		 you=0;
		 zuo=0;
	  }
	  else if(AD[0]>AD[4])
	  {
		 lostline_dir=1;//���߱�־		
	  }
   }
}

void lostline_deal()
{
    if(lostline_flag==1)
    {
        if(lostline_dir==1)//����
		  duojijiaodu=SMOTOR_CENTER+SMOTOR_RANGE;
        if(lostline_dir==2)//�Ҷ���
          duojijiaodu=SMOTOR_CENTER-SMOTOR_RANGE;;
        if(AD[2]>=150)//����
          lostline_flag=0;
    }
}
/*************************************************************/

void smotor_control(float i)
{
	smotor_angle = i;	
	smotor_angle = (int16)limit(smotor_angle,SMOTOR_RANGE);//�޷��������������ֿ��� ���¶���ջ�
	smotor_angle1 = smotor_angle;
	duojijiaodu=SMOTOR_CENTER+smotor_angle1;
	angle_record(smotor_angle);
	lost_line_judge();
	lostline_deal();
    pwm_duty(PWM4_MODULE3_CHA_C31,duojijiaodu);
}

int8 cie_data[7];
void cie_data_get(void)
{
	cie_data[0] = ad1;
	cie_data[1] = ad2;
	cie_data[2] = ad3;
	cie_data[3] = ad4;
	cie_data[4] = ad5;
	cie_data[5] = ad6;
	cie_data[6] = ad7;
}

void Gpio_init()
{
    pwm_init(PWM2_MODULE3_CHA_D2,13000,0); 
    pwm_init(PWM2_MODULE3_CHB_D3,13000,0);
    pwm_init(PWM1_MODULE3_CHA_D0,13000,0);
    pwm_init(PWM1_MODULE3_CHB_D1,13000,0);	
//    gpio_init(B11, GPO, 0, GPIO_PIN_CONFIG); //������
//    gpio_init(C25,GPI,1,FAST_GPIO_PIN_CONFIG);//����
//    gpio_init(C26,GPI,1,FAST_GPIO_PIN_CONFIG);
//    gpio_init(C3,GPI,1,FAST_GPIO_PIN_CONFIG);
//    gpio_init(C4,GPI,1,FAST_GPIO_PIN_CONFIG);
//    gpio_init(D27,GPI,1,FAST_GPIO_PIN_CONFIG);
//    gpio_init(C27,GPI,1,FAST_GPIO_PIN_CONFIG); 
//    gpio_init(B13,GPI,1,FAST_GPIO_PIN_CONFIG);//�ɻɹ�
//    gpio_init(B12,GPI,1,FAST_GPIO_PIN_CONFIG);  
//    gpio_init(B9,GPO,0,FAST_GPIO_PIN_CONFIG);
}


uint16 seep_lift,seep_right;
void dj_seep(float a)  //�������
{
	if(smotor_angle>chasuxianfu)
	{
		seep_lift=sudu-0.3*(sudu+smotor_angle1)*a/chasu;
		seep_right=sudu+(sudu+smotor_angle1)*a/chasu;
	}
	else if(smotor_angle<-chasuxianfu)
	{
		seep_lift=sudu+(sudu+fabs(smotor_angle1))*a/chasu;
		seep_right=sudu-0.3*(sudu+fabs(smotor_angle1))*a/chasu;
	}
	else
	{
		seep_lift=sudu+(sudu+fabs(smotor_angle1))*a/chasu;
		seep_right=sudu+(sudu+fabs(smotor_angle1))*a/chasu;
	}
	if(seep_lift<=0)
	{
		seep_lift=0;
	}
	if(seep_right<=0)
	{
		seep_right=0;
	}
}

int sdtj=0;
extern int ajbz2,sdlift,sdright;
uint8 xs1=0,xs2=0,xs3=0,xs4=0,xs5=0;
char ai=0;
extern int16 zdsd,wdsd;
int qidongtiaosu=0;
extern char waibubz;
extern unsigned long maichong,shi;
void smotor_init()
{
	pwm_init(PWM4_MODULE3_CHA_C31, 50, SMOTOR_CENTER);
	pwm_init(PWM1_MODULE3_CHA_D0, 13000, 0); //��
	pwm_init(PWM1_MODULE3_CHB_D1, 13000, 0); //��
	gpio_set(D2, 1);						 //��
	gpio_set(D3, 1);						 //��
}
void Opa_Init()
{
	adc_init(ADC_1,ADC1_CH3_B14 ,ADC_12BIT);
    adc_init(ADC_1,ADC1_CH4_B15 ,ADC_12BIT);
    adc_init(ADC_1,ADC1_CH5_B16 ,ADC_12BIT);
    adc_init(ADC_1,ADC1_CH6_B17 ,ADC_12BIT);
	adc_init(ADC_1,ADC1_CH7_B18 ,ADC_12BIT);
    adc_init(ADC_1,ADC1_CH8_B19 ,ADC_12BIT);
    adc_init(ADC_1,ADC1_CH9_B20 ,ADC_12BIT);

}
void bian_ma_qi()
{
	qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);  
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
}
int16 zhidaosudu=0,zdsd=0,wdsd=0;

int jlsudu=0;
int y=0;

void fenqvtiaosu()
{
	if((duojijiaodu>(SMOTOR_CENTER+80))||(duojijiaodu<(SMOTOR_CENTER-80)))  //С��
	{
//		if(shi>=17&&shi<=20)
//		{	
//			y=0.5*wdsd-25;
//			jlsudu=-y;
//			if(y<=0)
//				y=0;
//		}
//		else
//		{
//			jlsudu=0;
//		}	
		zhidaosudu=-(sdlift+sdright)/(jiansuxishu)+wdsd;		
	}
	else
	{
//		if(shi>=17&&shi<=20)
//		{
//			y=0.5*zdsd-25;
//			if(y<=0)
//				y=0;
//			jlsudu=-y;
//		}
//		else
//		{
//			jlsudu=0;
//		}
		zhidaosudu = zdsd;	
	}
}

float pid_out=0;
int main(void)
{
	int16 temp;	
	DisableGlobalIRQ();
    board_init();//��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
    EnableGlobalIRQ(0);
	//oled_init();
	Gpio_init();
	pit_init(); 
	pit_interrupt_ms(PIT_CH0,4);
	pit_interrupt_ms(PIT_CH1,2);
	pit_interrupt_ms(PIT_CH3,3);  	
	NVIC_SetPriority(PIT_IRQn,15);
	Opa_Init();	
	smotor_init();
	bian_ma_qi();	
	//gpio_interrupt_init(D27,FALLING,GPIO_INT_CONFIG);	//�ⲿ�жϸɻɹ�
	//NVIC_SetPriority(GPIO3_Combined_16_31_IRQn,0);	    //�����ⲿ�ж����ȼ�
	pwm_init(PWM4_MODULE3_CHA_C31, 50, SMOTOR_CENTER);
	SysTick_Config(CLOCK_GetFreq(kCLOCK_CoreSysClk) / (SYSTICK_PRESCALE * 10000U));	
	const CI_OutMetricInfo* pOM = CI_GetOutMetricInfo(0);
	ModelInfo_t inf;
	CI_GetModelInfoXIP(model1, &inf);
	EnableGlobalIRQ(0);	
    while(1)
    {
		if(collection_succes)
		{
			collection_succes = 0;
			cie_data_get();//��ȡRunModel��������Ĳ���
				CI_RunModelXIP(model1, cie_data, &temp);//����ģ�ͼ���ת��				
				//���ݻ�ȡ����ģ�Ͳ����Լ�������������λ
				int16_t g_servoValue;
				if(inf.quantBits > 7)
				{
					int16_t *g_out16 = (int16_t *)g_out;
					g_servoValue = temp >> (inf.quantBits - pOM->fracBits - 1);
					g_servoValue = temp >> 3;
				}
				else
				{
					g_servoValue = temp;
				}			
				smotor_angle = g_servoValue * (int32)420 / 128;//�Ŵ�����
			}
    }
}


#define PROFILING	1
#ifdef 	PROFILING

#define PROF_CNT		20
#define PROF_ERR		16
#define PROF_MASK 		(~(PROF_ERR - 1))
#define PROF_HITCNT_INC	100
// whether hitCnt should decay, faster decay makes most time consuming functions
// seems to have even more hit count
#define PROF_DECAY	
uint32_t s_ignrList[] = {
	0,
	// (uint32_t)sensor_snapshot + 0x40 ,
};

typedef struct {
	uint32_t baseAddr;	// (aligned) base address range of PC sample
	uint32_t hitCnt;    // hit count (a decay mecahnism automatically drops it)
	uint32_t hitRatio;	// 10-bit resolution hit ratio, 
	uint32_t rsvd;		// reserved for better view in memory window
} ProfUnit_t;

typedef struct {
	uint8_t decayNdx;  // which item to decay its hitCnt 
	uint32_t profCnt;  // totoal hit count of profiling
	ProfUnit_t items[PROF_CNT];
}Prof_t;
Prof_t s_prof;

void _ProfOnHit(ProfUnit_t *pItem, uint32_t pc) 
{
	pItem->baseAddr = pc & PROF_MASK;
	s_prof.profCnt+= PROF_HITCNT_INC;
	pItem->hitCnt += PROF_HITCNT_INC;
	pItem->hitRatio = 
		(uint32_t)(((uint64_t)(pItem->hitCnt) << 10) / s_prof.profCnt);
	// sort items descending
	ProfUnit_t tmpItem;	
	for (;pItem != s_prof.items && pItem[0].hitCnt > pItem[-1].hitCnt; pItem--) 
	{
		tmpItem = pItem[0]; pItem[0] = pItem[-1] ; pItem[-1] = tmpItem;
	}
}
void ProfUpdateRate(float res)
{
  uint32_t i; 
  float profCnt = s_prof.profCnt;
  ProfUnit_t *pItem = s_prof.items;
  for (i=0; i<PROF_CNT; i++, pItem++) {
	pItem->hitRatio = 
	  (uint32_t)( (float)(pItem->hitCnt) * res / profCnt);
  }
}
void ProfReset(void)
{
  memset(&s_prof, 0, sizeof(s_prof));
}
void Profiling(uint32_t pc) 
{
	uint32_t i;
	ProfUnit_t *pItem = &s_prof.items[0];
	// filter ignore list
	for (i=0; i<ARRAY_SIZE(s_ignrList); i++) {
		if (pc - s_ignrList[i] < PROF_ERR)
			return;
	}
	#ifdef PROF_DECAY
		// apply decaying, we do not decay to zero, this means PC samples do not
		// get removed automatically, only can be pushed by later new more frequent
		// hit PC samples 
		if (s_prof.items[s_prof.decayNdx].hitCnt > 1) {
			s_prof.items[s_prof.decayNdx].hitCnt--;
			s_prof.profCnt--;
		}
		if (++s_prof.decayNdx == PROF_CNT)
			s_prof.decayNdx = 0;
	#endif	
	uint32_t freeNdx = PROF_CNT;
	// >>> traverse to search existing same PC sample
	for (i=0, pItem = s_prof.items; i<PROF_CNT; i++, pItem++) {
		if (pItem->baseAddr == (pc & PROF_MASK)) {
			_ProfOnHit(pItem, pc);
			break;
		} else if (freeNdx == PROF_CNT && pItem->hitCnt == 0){
			// record empty item, in case need to use for new PC sample
			freeNdx = i;
		}
	}
	if (i == PROF_CNT) {
		if (freeNdx < PROF_CNT) {
			// does not find, allocate for new
			_ProfOnHit(s_prof.items + freeNdx, pc);
		} else {
		  // replace the last one. We must give new samples chance to compete to
		  // get into the list.
		  freeNdx = PROF_CNT - 1;
		  s_prof.profCnt -= s_prof.items[freeNdx].hitCnt;
		  s_prof.items[freeNdx].hitCnt = 0;
		  _ProfOnHit(s_prof.items + freeNdx, pc);
		}
	}
}

#endif
typedef struct {
    uint32_t    r0, r1, r2, r3, r12, lr, pc, xpsr;
} ExceptionRegisters_t;


volatile uint32_t s_prescale;
void SysTick_C_Handler(ExceptionRegisters_t *regs) {
    extern uint32_t uwTick;
	
	#if SYSTICK_PRESCALE > 1
	if (++s_prescale < SYSTICK_PRESCALE)
		return;
	s_prescale = 0;
	#endif
	
	#ifdef PROFILING
	Profiling(regs->pc);
	#endif	
	g_tick++;
}



#if (__CC_ARM)
__asm void SysTick_Handler(void) 
{
	IMPORT	SysTick_C_Handler

	PRESERVE8
	tst lr, #4 
    ite eq 
	mrseq r0, msp
	mrsne r0, psp	
	push   {lr}
	bl SysTick_C_Handler  // Off to C land
	pop    {lr}
	bx	   lr
}

#else

void SysTick_Handler(void)
{
    __asm (
		".globl SysTick_C_Handler\n"
		".align 3				\n"
        "tst lr, #4             \n"
        "ite eq              	\n"
        "mrseq r0, msp       	\n"
		"mrsne r0, psp	        \n"
		"push   {lr}	        \n"
		"bl SysTick_C_Handler	\n"
		"pop    {lr}	       	\n"
		"bx	   lr				\n"
    );
}

#endif
