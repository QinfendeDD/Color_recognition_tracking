#include "colorcfg.h"
#include "sys.h"
#include "lcd.h"
#include <stdio.h>

RESULT_t result[TRACE_NUM];

u8 global_page=0;
TARGET_CONDITION_t condition[TRACE_NUM]={
  65496,//200,//20,			//Ŀ��ɫ�ȣ�H_MIN	
	40,//280,//160,    		//Ŀ��ɫ�ȣ�H_MAX  ��ɫ160  ��ɫ80


//��Ҫ���� ����������Ͷȷ�Χ�ڡ� �����塣	
	90,//140,//50,    	 	//Ŀ����С���Ͷȣ�S_MIN
	180,//250,//200,   	 	//Ŀ����󱥺Ͷȣ�S_MAX
	
	
//��Ҫ���� �����������(�Ҷ�)��Χ�ڡ� �����塣		           
	0,//0,//50,     			//Ŀ����С���ȣ�L_MIN
	80,//100,//200,    		//Ŀ��������ȣ�L_MAX
	
//��Ҫ���� ���������С��Χ�ڡ� �����塣
	40,     							//Ŀ����С��ȣ�WIDTH_MIN
	40,     							//Ŀ����С�߶ȣ�HEIGHT_MIN
	           
	400,    							//Ŀ������ȣ�WIDTH_MAX
	400,    							//Ŀ�����߶ȣ�HEIGHT_MAX
/////////////////////////////////////////
};

SEARCH_AREA_t area = {IMG_X, IMG_X+IMG_W, IMG_Y, IMG_Y+IMG_H};//�����������򣬿�ʼ��    ��ȫ�ּ�������ɫ,����������


//��ȡĳ�����ɫ
void ReadColor( uint16_t usX, uint16_t usY, COLOR_RGB_t* color_rgb )
{
	uint16_t rgb;
	rgb = LCD_READPOINT( usX, usY );					//��ȡ��ɫ����
	
	//ת����ֵ��Ϊ[0,255]����ԭɫֵ
	color_rgb->Red 		= (uint8_t)( ( rgb & 0xF800 ) >> 8 );
	color_rgb->Green    = (uint8_t)( ( rgb & 0x07E0 ) >> 3 );
	color_rgb->Blue 	= (uint8_t)( ( rgb & 0x001F ) << 3 );	
}
/*************************************/
//RGBת��ΪHLS
//H:ɫ��
//L������
//S�����Ͷ�
void RGB2HSL( const COLOR_RGB_t* color_rgb, COLOR_HLS_t* color_hls )
{
	int r, g, b;
	int h, l, s;
	int max, min, dif;
	
	r = color_rgb->Red;
	g = color_rgb->Green;
	b = color_rgb->Blue;
	
	max = maxOf3Values( r, g, b );
	min = minOf3Values( r, g, b );
	dif = max - min;
	
	//����l������
	l = ( max + min ) * 240 / 255 / 2;
	
	//����h��ɫ��
	if( max == min )//�޶��� RGBһ��  �ڻҰ�
	{
		s = 0;//���Ͷ�0
		h = 0;//ɫ��0
	}
	else
	{
		/*����ɫ��h*/
		if( max == r )//���Rֵ���
		{
			if( min == b )//h����0��40
			{
				h = 40 * ( g - b ) / dif;
			}
			else if( min == g )//h����200��240
			{
				h = 40 * ( g - b ) / dif + 240;
			}
			
		}
		else if( max == g )
		{
			h = 40 * ( b - r ) / dif + 80;
		}
		else if( max == b )
		{
			h = 40 * ( r - g ) / dif + 160;
		}
		
		//���㱥�Ͷ�s
		if( l == 0 )
		{
			s = 0;
		}
		else if( l <= 120 )
		{
			s = dif * 240 / ( max + min );
		}
		else
		{
			//s = dif * 240 / ( 480 - ( max + min ) );
            s = (dif)*240/(511 - (max+min));
		}		 
	}   
    color_hls->Hue = h;				//ɫ��
	color_hls->Lightness = l;			//����
	color_hls->Saturation = s;			//���Ͷ�
}

/************************************************/
 //  ��ɫƥ��
//color_hls ��COLOR_HLS�ṹ�壬�洢HLS��ʽ��ɫ����
//condition ��TARGET_CONDITION�ṹ�壬���ϣ������ɫ������ֵ
// 1�����ص���ɫ��Ŀ�귶Χ�ڣ�0�����ص���ɫ����Ŀ�귶Χ�ڡ�
int ColorMatch(const COLOR_HLS_t* color_hls, const TARGET_CONDITION_t* condition )
{

    
	if(	
			color_hls->Lightness > condition->L_MIN &&
			color_hls->Lightness < condition->L_MAX &&
			color_hls->Saturation > condition->S_MIN &&
			color_hls->Saturation < condition->S_MAX
	)//�Ƚϱ��ͶȺ�����
    {
                
        if( color_hls->Hue > condition->H_MIN &&
			color_hls->Hue < condition->H_MAX  )//��ɫ�ڷ�Χ��
            return 1;
        else
        if (condition->H_MAX < condition->H_MIN)  //�趨�������ɫС����С��ɫ ˵����������� ������Ҫ�͸�λ��ɫƥ��            
        {
            if(color_hls->Hue < condition->H_MAX )
                return 1;
            if(color_hls->Hue > (condition->H_MIN-65295) )
                return 1;
        }else
        if(condition->H_MAX>240)//�趨�������ɫ����240 ˵����������� ������Ҫ�͵�λ��ɫƥ��
        {
            if(color_hls->Hue > condition->H_MAX )
                return 1;
            if(color_hls->Hue < (condition->H_MAX-240) )
                return 1;
        }
   
		return 0;
    }
	else
		return 0;
}



/****************************************************/
//  Ѱ�Ҹ�ʴ����
//  x ����ʴ����x����
//  y ����ʴ����y����
//  condition ��TARGET_CONDITION�ṹ�壬���ϣ������ɫ������ֵ
//  area ��SEARCH_AREA�ṹ�壬���Ҹ�ʴ���ĵ�����
// 1���ҵ��˸�ʴ���ģ�x��yΪ��ʴ���ĵ����ꣻ0��û���ҵ���ʴ���ġ�
int SearchCenter(uint16_t* x, uint16_t* y, const TARGET_CONDITION_t* condition,const SEARCH_AREA_t* area )
{
	uint16_t i, j, k;
	uint16_t FailCount=0;
	uint16_t SpaceX, SpaceY;
	COLOR_RGB_t rgb;
	COLOR_HLS_t hls;
	
	SpaceX = condition->WIDTH_MIN / 3;//����С��ȳ���3 Ϊ �����ѯ�Ĳ�����һ����λ
	SpaceY = condition->HEIGHT_MIN / 3;//����С�߶ȳ���3 Ϊ ��ֱ��ѯ�Ĳ�����һ����λ
	
	/*���򲽽���λ+��ֱ������λ �����һ�����ε�ɫ��*/
	for(i=area->Y_Start; i<area->Y_End; i+=SpaceY)
	{
		for(j=area->X_Start; j<area->X_End; j+=SpaceX)
		{
			FailCount = 0;
			for(k=0; k<SpaceX+SpaceY; k++)
			{
				if(k<SpaceX)
					ReadColor( j+k, i+SpaceY/2, &rgb );//��ѯɫ���м�һ�����ɫ
				else
					ReadColor( j+SpaceX/2, i+k-SpaceX, &rgb );//��ѯɫ���м�һ������ɫ
				RGB2HSL( &rgb, &hls );
				
				if(!ColorMatch( &hls, condition ))
					FailCount++;//��ɫ��ƥ�� ʧ�ܼ���+1
				
				if(FailCount>( (SpaceX+SpaceY) >> ALLOW_FAIL_PER ))//ʧ�ܼ������� ɫ����Ҫ��ѯ���ܵ���/2^�ݴ���
					break;//ʧ�ܴ���̫�� �˳�
				
			}
			
			if(k == SpaceX+SpaceY)//k��ֵ���ѯ��ϣ�˵������ƥ��
			{
				/*��¼��ɫ������ĵ�Ϊ��ʴ����*/
				*x = j + SpaceX / 2;
				*y = i + SpaceY / 2;
				return 1;   //��¼����һ����ʴ���ĺ��˳�����
			}
			
		}
			
	}
	
	return 0;
		
}

/***************************************************/
// �Ӹ�ʴ�������ⸯʴ���õ��µĸ�ʴ����
//  oldX ����ǰ�ĸ�ʴ����x����
//  oldX ����ǰ�ĸ�ʴ����y����
//  condition ��TARGET_CONDITION�ṹ�壬���ϣ������ɫ������ֵ
//  result ��RESULT�ṹ�壬��ż����
// 1�����ɹ���0�����ʧ�ܡ�
 
int Corrode(uint16_t oldX, uint16_t oldY, const TARGET_CONDITION_t* condition, RESULT_t* result )
{
	uint16_t Xmin, Xmax, Ymin, Ymax;
	uint16_t i;
	uint16_t FailCount=0;
	COLOR_RGB_t rgb;
	COLOR_HLS_t hls;
	
	for(i=oldX; i>IMG_X; i--)//�Ӿ�x������ʴ
	{
		ReadColor(i, oldY, &rgb);//����
		RGB2HSL(&rgb, &hls);//ת��
		if(!ColorMatch(&hls, condition))
			FailCount++;//��ƥ������Լ�1
		
        if( FailCount> ((condition->WIDTH_MIN)/ALLOW_FAIL_PER) )//��ʶ��ʧ�ܵ������С���/ALLOW_FAIL_PER������            
			break;
	}
	Xmin=i;//����������ӵ�x����ߵ�ֵ
	
	FailCount=0;
	for(i=oldX; i<IMG_X+IMG_W; i++)//�Ӿ�x�����Ҹ�ʴ
	{
		ReadColor(i, oldY, &rgb);
		RGB2HSL(&rgb, &hls);
		if(!ColorMatch(&hls, condition))
			FailCount++;

        if( FailCount> ((condition->WIDTH_MIN)/ALLOW_FAIL_PER) )
			break;
	}
	Xmax=i;
	
	FailCount=0;
	for(i=oldY; i>IMG_Y; i--)//�Ӿ�y�����ϸ�ʴ
	{
		ReadColor(oldX, i, &rgb);
		RGB2HSL(&rgb, &hls);
		if(!ColorMatch(&hls, condition))
			FailCount++;

        if( FailCount> ((condition->WIDTH_MIN)/ALLOW_FAIL_PER) )
			break;
	}
	Ymin=i;
	
	FailCount=0;
	for(i=oldY; i<IMG_Y+IMG_H; i++)//�Ӿ�y�����¸�ʴ
	{
		ReadColor(oldX, i, &rgb);
		RGB2HSL(&rgb, &hls);
		if(!ColorMatch(&hls, condition))
			FailCount++;
        
        if( FailCount> ((condition->WIDTH_MIN)/ALLOW_FAIL_PER) )
			break;
	}
	Ymax=i;
	
	FailCount=0;
	
	//��ø�ʴ������е��xy��Χ
	result->x = (Xmin + Xmax) / 2;
	result->y = (Ymin + Ymax) / 2;
	result->w = (Xmax - Xmin);
	result->h = (Ymax - Ymin);
	
	if( (result->w > condition->WIDTH_MIN) && (result->w < condition->WIDTH_MAX) &&
			(result->h > condition->HEIGHT_MIN) && (result->h < condition->HEIGHT_MAX)  )
		return 1;//�����ʴ�������û�г�������޶�������û��С����С�޶����� ��Ч����
	else
		return 0;
}

/*
 *����0ʶ��ʧ�ܣ�1�ɹ�
 *�õ�ƥ��ɫ�����Ϣ
 */
int Trace(const TARGET_CONDITION_t* condition, RESULT_t* result_final)
{
	uint16_t i;
	static uint16_t x0, y0;	
	RESULT_t result;
	
    if(!SearchCenter(&x0, &y0, condition, &area))//Ѱ�Ҹ�ʴ����
    {
        area.X_Start = IMG_X;
		area.X_End   = IMG_X+IMG_W;
		area.Y_Start = IMG_Y;
        area.Y_End   = IMG_Y+IMG_H;
        return 0;
	}
	//�ҵ���ʴ���� �õ��е�
	result.x = x0;
	result.y = y0;
	
	for(i=0; i<ITERATER_NUM; i++)//��ε��� ��ȷ����
	{
		Corrode(result.x, result.y, condition, &result);	//�Ӹ�ʴ�������ⸯʴ���õ��µĸ�ʴ����
		
	}
	
	if( Corrode(result.x, result.y, condition, &result) )//���¸�ʴ�ɹ�
	{
		result_final->x = result.x;
		result_final->y = result.y;
		result_final->w = result.w;
		result_final->h = result.h;
#if TRACE_NUM==1 //ֻ��һ��ͼ��ʱ��ʹ�ÿ��ٲ���		
		/*Ϊ�˿��ٶ���һ��ͼ������Ҹ�ʴ���ģ�ֱ�Ӷ��屾��ͼ��ĸ�ʴ����Ϊ��һ��ͼ���ɨ������*/
		area.X_Start = result.x - ((result.w)>>1);
		area.X_End   = result.x + ((result.w)>>1);
		area.Y_Start = result.y - ((result.h)>>1);
		area.Y_End   = result.y + ((result.h)>>1);
#endif		
		return 1;
	}
	else
	{
		return 0;
	}
	
}
