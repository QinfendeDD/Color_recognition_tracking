#include "Con_Phoenix.h"
/*
颜色识别与颜色跟踪	技术
正点原子蓝本				sse拓展
*/
PID_Phoenix	PID;//封装PID结构体变量
u8 d_z,d_shi,d_ge;//中间变量，用于显示P、I、D参数。
#define  OV7725 1	//识别号，用于区别7725/7670
#define  OV7670 2	//识别号，同上
u8 sensor=0;			//7725/7670传感器区分标志位
int main(void)
{		
	delay_init();	    	 																		//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 																		//串口初始化为 115200
 	usmart_dev.init(72);																		//初始化USMART		
 	LED_Init();		  																				//初始化与LED连接的硬件接口
	KEY_Init();																							//初始化按键
	LCD_Init();			   																			//初始化LCD  
	TPAD_Init(6);																						//触摸按键初始化 
	Phoenix_TIM3_CH12_PA67_PWM1_UP_HIGH_Init(19999,71);			//定时器3-通道1	&	通道2 输出比较功能初始化，且频率设为50HZ，即周期20ms.
	Ov7725_7670_Choose_Init();															//OV7725或7670初始化
	Constant_Init();																				//PID相应常数赋值初始化
	
	TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断		(1s间隔利用printf打印"帧率统计")			
	EXTI8_Init();										//使能定时器捕获-----(捕获帧中断)	
	
	LCD_Clear(BLACK);								//设置黑屏
	LCD_Fill(0,0,480,230,		WHITE);	//不希望（上）半部分是白色，则把该行注释掉即可，或者换成其他的颜色
	LCD_Fill(0,570,480,800,	WHITE);	//不希望（下）半部分是白色，则把该行注释掉即可，或者换成其他的颜色
	LCD_ShowString(10,100,12,24,24,					"P:");
	LCD_ShowString(10,100+30,12,24,24,			"I:");
	LCD_ShowString(10,100+30*2,12,24,24,		"D:");
	LCD_ShowString(10,200,480,24,24,	"PID_OUT     LR:       UD:");
	PWM_TEST();//自动测试舵机是否正常，测试次序：(-90度)—>(0度)—>(+90度)，若不需要则注释掉即可。
	
	PID.KI_magnify=0.01;PID.KD_magnify=0.01;//比例系数及微分系数的倍数，该参数乘以对应系数，则为最终的该项系数。
 	while(1)
	{
    OV7725_7670_Camera_Refresh_Phoenix();	/*图像显示更新*/
		Key_Modify_Phoenix();									/*按键修改参数*/
		Feature_Recognize_Start_Phoenix();		/*特征匹配部分*/
		PID_LR_figure_Phoenix();							/*PID横向环控制部分*/
		PID_UD_figure_Phoenix();							/*PID纵向环控制部分*/
		Show_LCD_Phoenix();										/*显示部分*/
		Upper_Computer_Phoenix();							/*发送数据到上位机*/
		delay_ms(50);													//波形显示上位机需要严格的50ms传输周期 
	}	   
}









