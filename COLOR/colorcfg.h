#ifndef colorcfg_h
#define colorcfg_h

#include "color.h"


/*����ɫ���ѯ�ķ�Χ  ͼ����LCD������*/
#define IMG_X 120			      		//ͼƬ���Ͻ�x����	��120��	******4.3������Ϊ��120,240��******2.8���޸�Ϊ��0,0��******3.5���޸�Ϊ��40,80��
#define IMG_Y 240               //ͼƬ���Ͻ�y����	��240��
#define IMG_W 230             	//ͼƬͼ����ʾ��ȣ�240��
#define IMG_H 310            	 	//ͼƬͼ����ʾ�߶ȣ�320��

#define ALLOW_FAIL_PER       2     //�ݴ���Խ�� ��ɫƥ��Խ�ߣ�ҲԽ��ʶ�� ȡֵ>1
#define ITERATER_NUM         8     //�������� Խ�ྫ��Խ׼
#define COLOR_RANG           30    //�趨��ɫ��ƫ�Ʒ�Χ Խ��Խ����ʶ�� ̫��������ʶ��
#define TRACE_NUM            1     //�趨׷����ɫ����Ŀ

extern u8 global_page;//��ǰ��ɫ��
extern SEARCH_AREA_t area;//������������
extern RESULT_t result[TRACE_NUM];//�����������
extern TARGET_CONDITION_t condition[TRACE_NUM];//����Ŀ�����

#define LCD_READPOINT( usX, usY )  LCD_ReadPoint(usX,usY)//������㺯��

#endif

