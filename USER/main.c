#include "Con_Phoenix.h"
/*
��ɫʶ������ɫ����	����
����ԭ������				sse��չ
*/
PID_Phoenix	PID;//��װPID�ṹ�����
u8 d_z,d_shi,d_ge;//�м������������ʾP��I��D������
#define  OV7725 1	//ʶ��ţ���������7725/7670
#define  OV7670 2	//ʶ��ţ�ͬ��
u8 sensor=0;			//7725/7670���������ֱ�־λ
int main(void)
{		
	delay_init();	    	 																		//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 																		//���ڳ�ʼ��Ϊ 115200
 	usmart_dev.init(72);																		//��ʼ��USMART		
 	LED_Init();		  																				//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();																							//��ʼ������
	LCD_Init();			   																			//��ʼ��LCD  
	TPAD_Init(6);																						//����������ʼ�� 
	Phoenix_TIM3_CH12_PA67_PWM1_UP_HIGH_Init(19999,71);			//��ʱ��3-ͨ��1	&	ͨ��2 ����ȽϹ��ܳ�ʼ������Ƶ����Ϊ50HZ��������20ms.
	Ov7725_7670_Choose_Init();															//OV7725��7670��ʼ��
	Constant_Init();																				//PID��Ӧ������ֵ��ʼ��
	
	TIM6_Int_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�		(1s�������printf��ӡ"֡��ͳ��")			
	EXTI8_Init();										//ʹ�ܶ�ʱ������-----(����֡�ж�)	
	
	LCD_Clear(BLACK);								//���ú���
	LCD_Fill(0,0,480,230,		WHITE);	//��ϣ�����ϣ��벿���ǰ�ɫ����Ѹ���ע�͵����ɣ����߻�����������ɫ
	LCD_Fill(0,570,480,800,	WHITE);	//��ϣ�����£��벿���ǰ�ɫ����Ѹ���ע�͵����ɣ����߻�����������ɫ
	LCD_ShowString(10,100,12,24,24,					"P:");
	LCD_ShowString(10,100+30,12,24,24,			"I:");
	LCD_ShowString(10,100+30*2,12,24,24,		"D:");
	LCD_ShowString(10,200,480,24,24,	"PID_OUT     LR:       UD:");
	PWM_TEST();//�Զ����Զ���Ƿ����������Դ���(-90��)��>(0��)��>(+90��)��������Ҫ��ע�͵����ɡ�
	
	PID.KI_magnify=0.01;PID.KD_magnify=0.01;//����ϵ����΢��ϵ���ı������ò������Զ�Ӧϵ������Ϊ���յĸ���ϵ����
 	while(1)
	{
    OV7725_7670_Camera_Refresh_Phoenix();	/*ͼ����ʾ����*/
		Key_Modify_Phoenix();									/*�����޸Ĳ���*/
		Feature_Recognize_Start_Phoenix();		/*����ƥ�䲿��*/
		PID_LR_figure_Phoenix();							/*PID���򻷿��Ʋ���*/
		PID_UD_figure_Phoenix();							/*PID���򻷿��Ʋ���*/
		Show_LCD_Phoenix();										/*��ʾ����*/
		Upper_Computer_Phoenix();							/*�������ݵ���λ��*/
		delay_ms(50);													//������ʾ��λ����Ҫ�ϸ��50ms�������� 
	}	   
}









