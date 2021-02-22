#include "Con_Phoenix.h"


//����OV7725��������װ��ʽԭ��,OV7725_WINDOW_WIDTH�൱��LCD�ĸ߶ȣ�OV7725_WINDOW_HEIGHT�൱��LCD�Ŀ��
//ע�⣺�˺궨��ֻ��OV7725��Ч
#define  OV7725_WINDOW_WIDTH		320 // <=320		LCD��ʾ��ͼ��ߴ硪���߶�
#define  OV7725_WINDOW_HEIGHT		240 // <=240		LCD��ʾ��ͼ��ߴ硪�����
#define  OV7725 1			
#define  OV7670 2			
extern u8 ov_sta;		//��exit.c�� �涨�塪��֡�жϱ�ǣ��������ͼ�����ݴ�����ٶȡ�
extern u8 ov_frame;	//��timer.c���涨�塪������ͳ��֡�����Դ�ӡ֡��
extern u8 sensor;








///1.///////////////////////////////////////////////////////////////////////////
/*
	����LCD��ʾ(OV7725)
	��֡�жϿ����£���FIFO��������ݴ��ݵ�MCU���沢��ʾ��LCD����ȥ
*/
void OV7725_camera_refresh(void)
{
	u32 i,j;
 	u16 color;	 
	if(ov_sta)//��֡�жϸ��£�//ֻҪ����EXTI�ж�,�ñ������Լӷ��㣬��ô�������ͻ��жϵ���������,�Ӷ�ִ�б�����(ͼ�����ݶ�ȡ����)
	{//��"(ͼ��ɼ�����)"��"(ͼ�����ݶ�ȡ����)"ͬʱ����,����������ͼ��������ȫ����FIFO֮����ȥ��,�Ӷ������ͼ�����ݴ�����ٶȡ�
		LCD_Scan_Dir(U2D_L2R);//���ϵ���,������
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//����ʾ�������õ���Ļ����
		if(lcddev.id==0X1963)
        LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//����ʾ�������õ���Ļ����
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7725_RRST=0;				//��ʼ��λ��ָ�� 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//��λ��ָ����� 
		OV7725_RCK_H; 
		for(i=0;i<OV7725_WINDOW_HEIGHT;i++)
		{
			for(j=0;j<OV7725_WINDOW_WIDTH;j++)
			{
				OV7725_RCK_L;
				color=GPIOC->IDR&0XFF;	//������
				OV7725_RCK_H; 
				color<<=8;  
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//������
				OV7725_RCK_H; 
				LCD->LCD_RAM=color;  
			}
		}
 		ov_sta=0;	//����֡�жϱ��,  ʹ֮������ѭ��ִ��"(ͼ��ɼ�����)����"��"(ͼ�����ݶ�ȡ����)����"
		ov_frame++; //֡�ж�, ֡������, ����printf����֡������ӡ��ȥ��
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽��Ĭ����"������,���ϵ���"ɨ�跽ʽ,���ȵ�һ���ٵڶ��е�����...��  
	} 
}

/*
	����LCD��ʾ(OV7670)
	��֡�жϿ����£���FIFO��������ݴ��ݵ�MCU���沢��ʾ��LCD����ȥ
*/
void OV7670_camera_refresh(void)
{
	u32 j;
 	u16 color;	 
	if(ov_sta)//��֡�жϸ���
	{
		LCD_Scan_Dir(U2D_L2R);//���ϵ���,������  
		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//����ʾ�������õ���Ļ����
		else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//����ʾ�������õ���Ļ����
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK_H;
		for(j=0;j<76800;j++)
		{
			OV7670_RCK_L;
			color=GPIOC->IDR&0XFF;	//������
			OV7670_RCK_H; 
			color<<=8;  
			OV7670_RCK_L;
			color|=GPIOC->IDR&0XFF;	//������
			OV7670_RCK_H; 
			LCD->LCD_RAM=color;    
		}   							  
 		ov_sta=0;					//����֡�жϱ��
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
	} 
}

/*
	OV7725��ʼ��||7670��ʼ��
*/
void Ov7725_7670_Choose_Init(void)
{
	u8 lightmode=0,effect=0;
	s8 saturation=0,brightness=0,contrast=0;
	
	POINT_COLOR=RED;		 
  LCD_ShowString(30,10,200,16,16,"OV7725_OV7670 Init...");	
	while(1)//��ʼ��OV7725_OV7670
	{
		if(OV7725_Init()==0)//7725��ʼ��
		{
			sensor=OV7725;
			LCD_ShowString(30,10,200,16,16,"OV7725 Init OK       ");
			
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGAģʽ���
			
			OV7725_Light_Mode(lightmode);
			OV7725_Color_Saturation(saturation);
			OV7725_Brightness(brightness);
			OV7725_Contrast(contrast);
			OV7725_Special_Effects(effect);
			OV7725_CS=0;//һֱʹ��
			break;
		}
		else if(OV7670_Init()==0)//7670��ʼ��
		{
			sensor=OV7670;
			LCD_ShowString(30,230,200,16,16,"OV7670 Init OK       ");
			delay_ms(1500);	 	   
			OV7670_Light_Mode(lightmode);
			OV7670_Color_Saturation(saturation);
			OV7670_Brightness(brightness);
			OV7670_Contrast(contrast);
			OV7670_Special_Effects(effect);
			OV7670_Window_Set(12,176,240,320);//���ô���	"240,320"���û�����(��������Ҫ���õķֱ���)  
			//"12,176"�ǹٷ�Դ������,�����޸�;���޸ĵ���ֵƫ�����ֵ�Ƚ�Զ�Ļ�,��ᷢ��ͼ��ᱻ�ָ��Ӱ�졣
			OV7670_CS=0;//һֱʹ��
			break;
		}
		else
		{
			LCD_ShowString(30,230,200,16,16,"OV7725_OV7670 Error!!");
			delay_ms(200);
			LCD_Fill(30,230,239,246,WHITE);
			delay_ms(200);
		}
	}
}

/*
	OV7725������ʾ||7670������ʾ
*/
void OV7725_7670_Camera_Refresh_Phoenix(void)
{
	if(sensor==OV7725)					OV7725_camera_refresh();//7725������ʾ
	else if(sensor==OV7670)		OV7670_camera_refresh();//7670������ʾ	
}






///2.///////////////////////////////////////////////////////////////////////////
/*
	�����޸ĺ���������������ָ����
*/
void Key_Modify_Phoenix(void)
{
	u8 key; 
	key=KEY_Scan(1);
	if(key==WKUP_PRES)	Color_Capture_Reconfigure();//��ͼ��ģ�5��5������ɫ����Ϊ�µ�ʶ����ɫ��ʹ�ø÷��������֡�ʣ�
	if(key==KEY2_PRES)	PID.Position_KP+=0.1; 	//���ñ���ϵ��			Kp	
	if(key==KEY1_PRES)	PID.Position_KI+=0.1;		//���û���ʱ�䳣��	Ki
	if(key==KEY0_PRES)	PID.Position_KD+=0.1;		//����΢��ʱ�䳣��	Kd
}





///3.///////////////////////////////////////////////////////////////////////////
/*
	��ͼ��ģ�5��5������ɫ����Ϊ�µ�ʶ����ɫ
*/
void Color_Capture_Reconfigure(void)
{  
	COLOR_RGB_t rgb_tmp;
	COLOR_HLS_t hls_tmp;

	ReadColor(IMG_X+5,IMG_Y+5, &rgb_tmp);	//RGB��ȡ

	RGB2HSL( &rgb_tmp, &hls_tmp );				//HSLת����������ȫ�������ϴ�ţ�ģ�

	condition[global_page].H_MIN=hls_tmp.Hue-COLOR_RANG;
	condition[global_page].H_MAX=hls_tmp.Hue+COLOR_RANG;

	condition[global_page].L_MIN=hls_tmp.Lightness-COLOR_RANG;
	condition[global_page].L_MAX=hls_tmp.Lightness+COLOR_RANG;

	condition[global_page].S_MIN=hls_tmp.Saturation-COLOR_RANG;
	condition[global_page].S_MAX=hls_tmp.Saturation+COLOR_RANG;

	condition[global_page].HEIGHT_MIN=40;
	condition[global_page].HEIGHT_MAX=400;

	condition[global_page].WIDTH_MAX=400;
	condition[global_page].WIDTH_MIN=40;

	printf("NEW R=%d G=%d B=%d\r\n",rgb_tmp.Red,rgb_tmp.Green,rgb_tmp.Blue);
	printf("NEW H=%d S=%d L=%d\r\n",hls_tmp.Hue,hls_tmp.Saturation,hls_tmp.Lightness);

	printf("H_MIN��%d,H_MAM��%d",condition[global_page].H_MIN,condition[global_page].H_MAX);

	/*RGB������ʾ*/
	LCD_ShowxNum(100,600,rgb_tmp.Red,3,24,0x80);							//RGB--Red		����
	LCD_ShowxNum(100+12+12*3,600,rgb_tmp.Green,3,24,0x80);		//RGB--Green	����
	LCD_ShowxNum(100+12*2+12*6,600,rgb_tmp.Blue,3,24,0x80);		//RGB--Blue		����

	/*HSL������ʾ*/
	LCD_ShowxNum(100,630,hls_tmp.Hue,3,24,0x80);									//HSL--	Hue					data
	LCD_ShowxNum(100+12+12*3,630,hls_tmp.Saturation,3,24,0x80);		//HSL--	Saturation	data
	LCD_ShowxNum(100+12*2+12*6,630,hls_tmp.Lightness,3,24,0x80);	//HSL--	Lightness		data

	/*H_data_MAX&MIN��ʾ*/
	LCD_ShowxNum(100,660,condition[global_page].H_MIN,3,24,0x80);					//H_MIN
	LCD_ShowxNum(100+12+12*3,660,condition[global_page].H_MAX,3,24,0x80);	//H_MAX


	LCD_ShowString((IMG_X+IMG_W)/2,(IMG_Y+IMG_H)/2,200,16,16,(u8 *)"set complete!!");
	if(++global_page>=TRACE_NUM)
			global_page=0;
}

/*
	��һ��ʮ��׼��
*/
void Draw_Sign_Cross(u16 x,u16 y)
{
	LCD_DrawLine(x-5,y,x+5,y);
	LCD_DrawLine(x,y-5,x,y+5);
}

/*
	��ӡ	��Ŀ��ɫ�����ĵ㣨ʮ�����ĵ㣩�����в���	��LCD
*/
void color_Value(u16 x,u16 y)
{
    COLOR_RGB_t rgb_tmp;
    COLOR_HLS_t hls_tmp;
    
    ReadColor(x+1,y+1,&rgb_tmp);//��ͼ���rgb

    RGB2HSL(&rgb_tmp,&hls_tmp);//ת����hsl
		
		LCD_ShowxNum(210+100,600,rgb_tmp.Red,3,24,0x80);
		LCD_ShowxNum(210+100+12+12*3,600,rgb_tmp.Green,3,24,0x80);
		LCD_ShowxNum(210+100+12*2+12*6,600,rgb_tmp.Blue,3,24,0x80);
		
		LCD_ShowxNum(210+100,630,hls_tmp.Hue,3,24,0x80);
		LCD_ShowxNum(210+100+12+12*3,630,hls_tmp.Saturation,3,24,0x80);
		LCD_ShowxNum(210+100+12*2+12*6,630,hls_tmp.Lightness,3,24,0x80);

		LCD_ShowxNum(210+100,660,condition[global_page].H_MIN,3,24,0x80);
		LCD_ShowxNum(210+100+12+12*3,660,condition[global_page].H_MAX,3,24,0x80);
}

/*
	����ʶ��
*/
void Feature_Recognize_Start_Phoenix(void)
{
	u8 i_2=0;	 
	for(i_2=0;i_2<TRACE_NUM;i_2++)
	{
		if(Trace(&condition[i_2], &result[i_2]))//ִ����ɫʶ��  �������Ρ�����ʮ��׼�ǡ�
		{            
			LCD_DrawRectangle( result[i_2].x-result[i_2].w/2, result[i_2].y-result[i_2].h/2, result[i_2].x-result[i_2].w/2+result[i_2].w,  result[i_2].y-result[i_2].h/2+result[i_2].h);//������                     
			Draw_Sign_Cross(result[i_2].x, result[i_2].y);//��ʮ��׼��
			
			printf("Capture_HSL= %d,%d\r\n",result[i_2].x,result[i_2].y);//��Ŀ�����ĵ������ӡ�����ڡ�
			LCD_ShowxNum(0,600,result[i_2].x,3,24,0x00);	//��Ŀ�����ĵ�(x)�����ӡ��LCD��
			LCD_ShowxNum(40,600,result[i_2].y,3,24,0x00);	//��Ŀ�����ĵ�(y)�����ӡ��LCD��
			PID.Feedback_X=result[i_2].x;		PID.Feedback_Y=result[i_2].y;
			
			color_Value(PID.Feedback_X,PID.Feedback_Y);//��Ŀ������ĵ�(�ӽ����ĵ�)����ɫ�Ĳ�����ӡ��LCD��
			LED0=1;//ʶ�𵽣�LED0����   
		}
		else
		{
			LED0=0;//δʶ�� LED0��			
		} 
	}
}








///4.///////////////////////////////////////////////////////////////////////////
/*
	PID����
*/
void PID_LR_figure_Phoenix(void)
{
	/*1.��ȡƫ��*/
	PID.Bias_LR=PID.Feedback_X-PID.User_X;

	/*2.����PID*/
	PID.PID_LR_current_out=Position_PID(PID.Feedback_X,PID.User_X);//User_X����������    Feedback_X������ǰ����

	/*3.�������ȡ����ֵ*/
	PWM_abs(&PID.PID_LR_current_out);//ȡ����ֵ

	/*4.����*/
	if(PID.Bias_LR>0){PID.PWM_value_LR+=PID.PID_LR_current_out;}//���ң������ң�������7725���ų��·��ã����еġ�+=����Ϊ��-=����
	if(PID.Bias_LR<0){PID.PWM_value_LR-=PID.PID_LR_current_out;}//���󣬵����󣬼�С����7725���ų��·��ã����еġ�-=����Ϊ��+=����

	/*5.PWM�޷�*/
	Xianfu_Pwm(&PID.PWM_value_LR);//�޷�PWM

	/*6.����PWM*/
	TIM_SetCompare1(TIM3,PID.PWM_value_LR);
}

/*
	PID����
*/
void PID_UD_figure_Phoenix(void)
{
	/*1.��ȡƫ��*/
	PID.Bias_UD=PID.Feedback_Y-PID.User_Y;

	/*2.����PID*/
	PID.PID_UD_current_out=Position_PID(PID.Feedback_Y,PID.User_Y);

	/*3.�������ȡ����ֵ*/
	PWM_abs(&PID.PID_UD_current_out);//ȡ����ֵ

	/*4.����*/
	if(PID.Bias_UD>0){PID.PWM_value_UD-=PID.PID_UD_current_out;}//���£������£���С����7725���ų��·��ã����еġ�-=����Ϊ��+=����
	if(PID.Bias_UD<0){PID.PWM_value_UD+=PID.PID_UD_current_out;}//���ϣ������ϣ�������7725���ų��·��ã����еġ�+=����Ϊ��-=����

	/*5.PWM�޷�*/
	Xianfu_Pwm(&PID.PWM_value_UD);//�޷�PWM

	/*6.����PWM*/
	TIM_SetCompare2(TIM3,PID.PWM_value_UD);
}

/*
	PID��Ӧ������ֵ��ʼ��
*/
void Constant_Init(void)
{
	PID.Position_KP=0;			PID.Position_KI=0;	PID.Position_KD=0;	//����λ��PID��������PID����
	PID.Increment_KP=0;			PID.Increment_KI=0;											//��������PI��������PI����
	
	PID.Bias_LR=0;					PID.Bias_UD=0;													//����ƫ��Bias_LR:����LR:leftright������ƫ��Bias_UD:����UD:updown
	PID.User_X=240;					PID.User_Y=400;													//�û�����ֵ
	PID.Feedback_X=0;				PID.Feedback_Y=0;												//����ֵ	
	
	PID.PWM_value_LR=1400,	PID.PWM_value_UD=1400;									//����PWM���ռ���ֵ--����LR:leftright		����PWM���ռ���ֵ--����UD:updown
}



/*
	���ڲ��Զ���Ƿ�����
	����(-90��)��>(0��)��>(+90��)
*/
void PWM_TEST(void)
{
	TIM_SetCompare1(TIM3,500);	TIM_SetCompare2(TIM3,500);	delay_ms(500);	
	TIM_SetCompare1(TIM3,1400);	TIM_SetCompare2(TIM3,1400);	delay_ms(500);
	TIM_SetCompare1(TIM3,2200);	TIM_SetCompare2(TIM3,2200);	delay_ms(500);	
}



