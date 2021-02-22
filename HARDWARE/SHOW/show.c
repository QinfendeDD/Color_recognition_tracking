#include "Con_Phoenix.h"

extern u8 d_z,d_shi,d_ge;//�м������������ʾP��I��D������





/*
	��LCD��ӡ����		
*/
void Show_LCD_Phoenix(void)
{
	/*����ֵ �� ����ֵ��ʾ*/
	LCD_ShowxNum(00,650,PID.User_X,3,24,0x00);			//�û���������	User_X	: ͼ�����ĵ�	(x)
	LCD_ShowxNum(40,650,PID.User_Y,3,24,0x00);			//�û���������	User_Y	: ͼ�����ĵ�	(y)
	LCD_ShowxNum(00,680,PID.Feedback_X,3,24,0x00);	//��������	Feedback_X	: Ŀ�����ĵ�	(x)
	LCD_ShowxNum(40,680,PID.Feedback_Y,3,24,0x00);	//��������	Feedback_Y	: Ŀ�����ĵ�	(y)		

	/*PIDϵ����ʾ*/
	POINT_COLOR=MAGENTA;
	d_z=PID.Position_KP*10;		d_shi=d_z/10;		d_ge=d_z%10;//��ǰP����	
	LCD_ShowxNum(50,100,d_shi,3,24,0x00);	
	LCD_ShowString(50+15*3,100,12,24,24,".");
	LCD_ShowxNum(50+15*4,100,d_ge,1,24,0x00);	

	d_z=PID.Position_KI*10;		d_shi=d_z/10;		d_ge=d_z%10;//��ǰI����		
	LCD_ShowxNum(50,100+30,d_shi,3,24,0x00);	
	LCD_ShowString(50+15*3,100+30,12,24,24,".");
	LCD_ShowxNum(50+15*4,100+30,d_ge,1,24,0x00);	

	d_z=PID.Position_KD*10;		d_shi=d_z/10;		d_ge=d_z%10;//��ǰD����		
	LCD_ShowxNum(50,100+30*2,d_shi,3,24,0x00);	
	LCD_ShowString(50+15*3,100+30*2,12,24,24,".");
	LCD_ShowxNum(50+15*4,100+30*2,d_ge,1,24,0x00);	

	LCD_ShowxNum(152+12*3,200,PID.PWM_value_LR,4,24,0x80);//���κ���PID���	
	LCD_ShowxNum(272+12*3,200,PID.PWM_value_UD,4,24,0x80);//��������PID���	
	POINT_COLOR=RED;
}



unsigned char i_uc;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
/*
	����ʾ����		
	����λ����������
*/
void Upper_Computer_Phoenix(void)
{   
	DataScope_Get_Channel_Data( PID.Feedback_X, 1 );       
	DataScope_Get_Channel_Data( PID.User_X, 2 );
	DataScope_Get_Channel_Data( 0, 3 );              
//	DataScope_Get_Channel_Data( 0 , 4 );	//����Ҫ��ʾ�������滻0������
//	DataScope_Get_Channel_Data(	0	, 5 );	//����Ҫ��ʾ�������滻0������
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






