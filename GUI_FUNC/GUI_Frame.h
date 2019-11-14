#include "GUI.h"
#include "main.h"
#include "stm32f407xx.h"
#include "ILI93xx.h"

typedef enum {
    MENU_NONE,
	MENU_CAN,
	MENU_USART,
    MENU_MOTOR,
    MENU_XT,
    MENU_XY,
    MENU_OTHER
} MENU_STATE;

extern int time_1s_flag;
extern int time_1s_cnt;
extern int time_1ms_cnt;

void drawCoordiantes(int x0,int y0,int x1,int y1);//����xy����ϵ
void drawMainFrame(void);//���������
void menu_function(void);//�����л�����
void menu_update(MENU_STATE state,int mode);//���ڸ���
void menu_pageup(void);//�����Ϸ�ҳ
void menu_pagedown(void);//�����·�ҳ
void menu_clear_window(void);//�������
void show_time(signed short x,signed short y);//ʱ����ʾ
void usart_window_update(char *text);//���ڿ�����ʾ
void can_window_update(uint32_t Std_ID,uint8_t aData[]);//can������ʾ
void GUI_main();//GUI������
void GUI_Reload();//���³�ʼ����Ļ���������ʱ�ָ���ʾ
void draw_point_onMap(float x,float y);//��XYͼ�ϻ���
