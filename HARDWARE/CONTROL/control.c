#include "Con_Phoenix.h"


//由于OV7725传感器安装方式原因,OV7725_WINDOW_WIDTH相当于LCD的高度，OV7725_WINDOW_HEIGHT相当于LCD的宽度
//注意：此宏定义只对OV7725有效
#define  OV7725_WINDOW_WIDTH		320 // <=320		LCD显示的图像尺寸——高度
#define  OV7725_WINDOW_HEIGHT		240 // <=240		LCD显示的图像尺寸——宽度
#define  OV7725 1			
#define  OV7670 2			
extern u8 ov_sta;		//在exit.c里 面定义——帧中断标记，用于提高图像数据处理的速度。
extern u8 ov_frame;	//在timer.c里面定义——用于统计帧数，以打印帧率
extern u8 sensor;








///1.///////////////////////////////////////////////////////////////////////////
/*
	更新LCD显示(OV7725)
	在帧中断控制下，把FIFO里面的数据传递到MCU里面并显示在LCD上面去
*/
void OV7725_camera_refresh(void)
{
	u32 i,j;
 	u16 color;	 
	if(ov_sta)//有帧中断更新？//只要进入EXTI中断,该变量就自加非零，那么本函数就会判断到变量非零,从而执行本函数(图像数据读取过程)
	{//即"(图像采集过程)"与"(图像数据读取过程)"同时进行,而不是所有图像数据完全存入FIFO之后再去读,从而提高了图像数据处理的速度。
		LCD_Scan_Dir(U2D_L2R);//从上到下,从左到右
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//将显示区域设置到屏幕中央
		if(lcddev.id==0X1963)
        LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7725_RRST=0;				//开始复位读指针 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//复位读指针结束 
		OV7725_RCK_H; 
		for(i=0;i<OV7725_WINDOW_HEIGHT;i++)
		{
			for(j=0;j<OV7725_WINDOW_WIDTH;j++)
			{
				OV7725_RCK_L;
				color=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H; 
				color<<=8;  
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H; 
				LCD->LCD_RAM=color;  
			}
		}
 		ov_sta=0;	//清零帧中断标记,  使之往返的循环执行"(图像采集过程)函数"与"(图像数据读取过程)函数"
		ov_frame++; //帧中断, 帧计数器, 将用printf将“帧数”打印出去。
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向（默认是"从左到右,从上到下"扫描方式,即先第一行再第二行第三行...）  
	} 
}

/*
	更新LCD显示(OV7670)
	在帧中断控制下，把FIFO里面的数据传递到MCU里面并显示在LCD上面去
*/
void OV7670_camera_refresh(void)
{
	u32 j;
 	u16 color;	 
	if(ov_sta)//有帧中断更新
	{
		LCD_Scan_Dir(U2D_L2R);//从上到下,从左到右  
		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//将显示区域设置到屏幕中央
		else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK_H;
		for(j=0;j<76800;j++)
		{
			OV7670_RCK_L;
			color=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H; 
			color<<=8;  
			OV7670_RCK_L;
			color|=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H; 
			LCD->LCD_RAM=color;    
		}   							  
 		ov_sta=0;					//清零帧中断标记
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
	} 
}

/*
	OV7725初始化||7670初始化
*/
void Ov7725_7670_Choose_Init(void)
{
	u8 lightmode=0,effect=0;
	s8 saturation=0,brightness=0,contrast=0;
	
	POINT_COLOR=RED;		 
  LCD_ShowString(30,10,200,16,16,"OV7725_OV7670 Init...");	
	while(1)//初始化OV7725_OV7670
	{
		if(OV7725_Init()==0)//7725初始化
		{
			sensor=OV7725;
			LCD_ShowString(30,10,200,16,16,"OV7725 Init OK       ");
			
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGA模式输出
			
			OV7725_Light_Mode(lightmode);
			OV7725_Color_Saturation(saturation);
			OV7725_Brightness(brightness);
			OV7725_Contrast(contrast);
			OV7725_Special_Effects(effect);
			OV7725_CS=0;//一直使能
			break;
		}
		else if(OV7670_Init()==0)//7670初始化
		{
			sensor=OV7670;
			LCD_ShowString(30,230,200,16,16,"OV7670 Init OK       ");
			delay_ms(1500);	 	   
			OV7670_Light_Mode(lightmode);
			OV7670_Color_Saturation(saturation);
			OV7670_Brightness(brightness);
			OV7670_Contrast(contrast);
			OV7670_Special_Effects(effect);
			OV7670_Window_Set(12,176,240,320);//设置窗口	"240,320"是用户设置(就是我们要设置的分辨率)  
			//"12,176"是官方源码设置,不可修改;若修改的数值偏离该数值比较远的话,则会发现图像会被分割等影响。
			OV7670_CS=0;//一直使能
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
	OV7725更新显示||7670更新显示
*/
void OV7725_7670_Camera_Refresh_Phoenix(void)
{
	if(sensor==OV7725)					OV7725_camera_refresh();//7725更新显示
	else if(sensor==OV7670)		OV7670_camera_refresh();//7670更新显示	
}






///2.///////////////////////////////////////////////////////////////////////////
/*
	按键修改函数，可自行设置指向功能
*/
void Key_Modify_Phoenix(void)
{
	u8 key; 
	key=KEY_Scan(1);
	if(key==WKUP_PRES)	Color_Capture_Reconfigure();//将图像的（5，5）的颜色设置为新的识别颜色（使用该方法可提高帧率）
	if(key==KEY2_PRES)	PID.Position_KP+=0.1; 	//设置比例系数			Kp	
	if(key==KEY1_PRES)	PID.Position_KI+=0.1;		//设置积分时间常数	Ki
	if(key==KEY0_PRES)	PID.Position_KD+=0.1;		//设置微分时间常数	Kd
}





///3.///////////////////////////////////////////////////////////////////////////
/*
	将图像的（5，5）的颜色设置为新的识别颜色
*/
void Color_Capture_Reconfigure(void)
{  
	COLOR_RGB_t rgb_tmp;
	COLOR_HLS_t hls_tmp;

	ReadColor(IMG_X+5,IMG_Y+5, &rgb_tmp);	//RGB读取

	RGB2HSL( &rgb_tmp, &hls_tmp );				//HSL转换函数（完全拷贝网上大牛的）

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

	printf("H_MIN：%d,H_MAM：%d",condition[global_page].H_MIN,condition[global_page].H_MAX);

	/*RGB数据显示*/
	LCD_ShowxNum(100,600,rgb_tmp.Red,3,24,0x80);							//RGB--Red		分量
	LCD_ShowxNum(100+12+12*3,600,rgb_tmp.Green,3,24,0x80);		//RGB--Green	分量
	LCD_ShowxNum(100+12*2+12*6,600,rgb_tmp.Blue,3,24,0x80);		//RGB--Blue		分量

	/*HSL数据显示*/
	LCD_ShowxNum(100,630,hls_tmp.Hue,3,24,0x80);									//HSL--	Hue					data
	LCD_ShowxNum(100+12+12*3,630,hls_tmp.Saturation,3,24,0x80);		//HSL--	Saturation	data
	LCD_ShowxNum(100+12*2+12*6,630,hls_tmp.Lightness,3,24,0x80);	//HSL--	Lightness		data

	/*H_data_MAX&MIN显示*/
	LCD_ShowxNum(100,660,condition[global_page].H_MIN,3,24,0x80);					//H_MIN
	LCD_ShowxNum(100+12+12*3,660,condition[global_page].H_MAX,3,24,0x80);	//H_MAX


	LCD_ShowString((IMG_X+IMG_W)/2,(IMG_Y+IMG_H)/2,200,16,16,(u8 *)"set complete!!");
	if(++global_page>=TRACE_NUM)
			global_page=0;
}

/*
	画一个十字准星
*/
void Draw_Sign_Cross(u16 x,u16 y)
{
	LCD_DrawLine(x-5,y,x+5,y);
	LCD_DrawLine(x,y-5,x,y+5);
}

/*
	打印	颜目标色的中心点（十字中心点）的所有参数	到LCD
*/
void color_Value(u16 x,u16 y)
{
    COLOR_RGB_t rgb_tmp;
    COLOR_HLS_t hls_tmp;
    
    ReadColor(x+1,y+1,&rgb_tmp);//读图像的rgb

    RGB2HSL(&rgb_tmp,&hls_tmp);//转换成hsl
		
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
	特征识别
*/
void Feature_Recognize_Start_Phoenix(void)
{
	u8 i_2=0;	 
	for(i_2=0;i_2<TRACE_NUM;i_2++)
	{
		if(Trace(&condition[i_2], &result[i_2]))//执行颜色识别  画个矩形、画个十字准星。
		{            
			LCD_DrawRectangle( result[i_2].x-result[i_2].w/2, result[i_2].y-result[i_2].h/2, result[i_2].x-result[i_2].w/2+result[i_2].w,  result[i_2].y-result[i_2].h/2+result[i_2].h);//画矩形                     
			Draw_Sign_Cross(result[i_2].x, result[i_2].y);//画十字准星
			
			printf("Capture_HSL= %d,%d\r\n",result[i_2].x,result[i_2].y);//将目标中心点坐标打印到串口。
			LCD_ShowxNum(0,600,result[i_2].x,3,24,0x00);	//将目标中心点(x)坐标打印到LCD。
			LCD_ShowxNum(40,600,result[i_2].y,3,24,0x00);	//将目标中心点(y)坐标打印到LCD。
			PID.Feedback_X=result[i_2].x;		PID.Feedback_Y=result[i_2].y;
			
			color_Value(PID.Feedback_X,PID.Feedback_Y);//将目标的中心点(接近中心点)的颜色的参数打印到LCD。
			LED0=1;//识别到，LED0不亮   
		}
		else
		{
			LED0=0;//未识别到 LED0亮			
		} 
	}
}








///4.///////////////////////////////////////////////////////////////////////////
/*
	PID横向环
*/
void PID_LR_figure_Phoenix(void)
{
	/*1.获取偏差*/
	PID.Bias_LR=PID.Feedback_X-PID.User_X;

	/*2.进入PID*/
	PID.PID_LR_current_out=Position_PID(PID.Feedback_X,PID.User_X);//User_X：期望坐标    Feedback_X：红球当前坐标

	/*3.将输出量取绝对值*/
	PWM_abs(&PID.PID_LR_current_out);//取绝对值

	/*4.方向*/
	if(PID.Bias_LR>0){PID.PWM_value_LR+=PID.PID_LR_current_out;}//在右，得向右，增大（若7725引脚朝下放置，则本行的“+=”改为“-=”）
	if(PID.Bias_LR<0){PID.PWM_value_LR-=PID.PID_LR_current_out;}//在左，得向左，减小（若7725引脚朝下放置，则本行的“-=”改为“+=”）

	/*5.PWM限幅*/
	Xianfu_Pwm(&PID.PWM_value_LR);//限幅PWM

	/*6.加载PWM*/
	TIM_SetCompare1(TIM3,PID.PWM_value_LR);
}

/*
	PID纵向环
*/
void PID_UD_figure_Phoenix(void)
{
	/*1.获取偏差*/
	PID.Bias_UD=PID.Feedback_Y-PID.User_Y;

	/*2.进入PID*/
	PID.PID_UD_current_out=Position_PID(PID.Feedback_Y,PID.User_Y);

	/*3.将输出量取绝对值*/
	PWM_abs(&PID.PID_UD_current_out);//取绝对值

	/*4.方向*/
	if(PID.Bias_UD>0){PID.PWM_value_UD-=PID.PID_UD_current_out;}//在下，得向下，减小（若7725引脚朝下放置，则本行的“-=”改为“+=”）
	if(PID.Bias_UD<0){PID.PWM_value_UD+=PID.PID_UD_current_out;}//在上，得向上，增大（若7725引脚朝下放置，则本行的“+=”改为“-=”）

	/*5.PWM限幅*/
	Xianfu_Pwm(&PID.PWM_value_UD);//限幅PWM

	/*6.加载PWM*/
	TIM_SetCompare2(TIM3,PID.PWM_value_UD);
}

/*
	PID相应常数赋值初始化
*/
void Constant_Init(void)
{
	PID.Position_KP=0;			PID.Position_KI=0;	PID.Position_KD=0;	//用于位置PID控制器的PID参数
	PID.Increment_KP=0;			PID.Increment_KI=0;											//用于增量PI控制器的PI参数
	
	PID.Bias_LR=0;					PID.Bias_UD=0;													//横向环偏差Bias_LR:左右LR:leftright，纵向环偏差Bias_UD:上下UD:updown
	PID.User_X=240;					PID.User_Y=400;													//用户期望值
	PID.Feedback_X=0;				PID.Feedback_Y=0;												//反馈值	
	
	PID.PWM_value_LR=1400,	PID.PWM_value_UD=1400;									//横向环PWM最终加载值--左右LR:leftright		纵向环PWM最终加载值--上下UD:updown
}



/*
	用于测试舵机是否正常
	次序：(-90度)—>(0度)—>(+90度)
*/
void PWM_TEST(void)
{
	TIM_SetCompare1(TIM3,500);	TIM_SetCompare2(TIM3,500);	delay_ms(500);	
	TIM_SetCompare1(TIM3,1400);	TIM_SetCompare2(TIM3,1400);	delay_ms(500);
	TIM_SetCompare1(TIM3,2200);	TIM_SetCompare2(TIM3,2200);	delay_ms(500);	
}



