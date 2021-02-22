#include "Con_Phoenix.h"

extern u8 d_z,d_shi,d_ge;//中间变量，用于显示P、I、D参数。





/*
	向LCD打印数据		
*/
void Show_LCD_Phoenix(void)
{
	/*期望值 与 反馈值显示*/
	LCD_ShowxNum(00,650,PID.User_X,3,24,0x00);			//用户期望坐标	User_X	: 图像中心点	(x)
	LCD_ShowxNum(40,650,PID.User_Y,3,24,0x00);			//用户期望坐标	User_Y	: 图像中心点	(y)
	LCD_ShowxNum(00,680,PID.Feedback_X,3,24,0x00);	//反馈坐标	Feedback_X	: 目标中心点	(x)
	LCD_ShowxNum(40,680,PID.Feedback_Y,3,24,0x00);	//反馈坐标	Feedback_Y	: 目标中心点	(y)		

	/*PID系数显示*/
	POINT_COLOR=MAGENTA;
	d_z=PID.Position_KP*10;		d_shi=d_z/10;		d_ge=d_z%10;//当前P参数	
	LCD_ShowxNum(50,100,d_shi,3,24,0x00);	
	LCD_ShowString(50+15*3,100,12,24,24,".");
	LCD_ShowxNum(50+15*4,100,d_ge,1,24,0x00);	

	d_z=PID.Position_KI*10;		d_shi=d_z/10;		d_ge=d_z%10;//当前I参数		
	LCD_ShowxNum(50,100+30,d_shi,3,24,0x00);	
	LCD_ShowString(50+15*3,100+30,12,24,24,".");
	LCD_ShowxNum(50+15*4,100+30,d_ge,1,24,0x00);	

	d_z=PID.Position_KD*10;		d_shi=d_z/10;		d_ge=d_z%10;//当前D参数		
	LCD_ShowxNum(50,100+30*2,d_shi,3,24,0x00);	
	LCD_ShowString(50+15*3,100+30*2,12,24,24,".");
	LCD_ShowxNum(50+15*4,100+30*2,d_ge,1,24,0x00);	

	LCD_ShowxNum(152+12*3,200,PID.PWM_value_LR,4,24,0x80);//本次横向环PID输出	
	LCD_ShowxNum(272+12*3,200,PID.PWM_value_UD,4,24,0x80);//本次纵向环PID输出	
	POINT_COLOR=RED;
}



unsigned char i_uc;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
/*
	虚拟示波器		
	往上位机发送数据
*/
void Upper_Computer_Phoenix(void)
{   
	DataScope_Get_Channel_Data( PID.Feedback_X, 1 );       
	DataScope_Get_Channel_Data( PID.User_X, 2 );
	DataScope_Get_Channel_Data( 0, 3 );              
//	DataScope_Get_Channel_Data( 0 , 4 );	//用您要显示的数据替换0就行了
//	DataScope_Get_Channel_Data(	0	, 5 );	//用您要显示的数据替换0就行了
//	DataScope_Get_Channel_Data(	0 , 6 );	
//	DataScope_Get_Channel_Data(	0	, 7 );
//	DataScope_Get_Channel_Data( 0	, 8 ); 
//	DataScope_Get_Channel_Data(	0	, 9 );  
//	DataScope_Get_Channel_Data( 0 ,	10);
	
	Send_Count = DataScope_Data_Generate(3);
	
	for(i_uc=0;i_uc<Send_Count;i_uc++) 
	{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i_uc]; 
	}
}






