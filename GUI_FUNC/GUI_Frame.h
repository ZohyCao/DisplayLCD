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

void drawCoordiantes(int x0,int y0,int x1,int y1);//绘制xy坐标系
void drawMainFrame(void);//绘制主框架
void menu_function(void);//窗口切换控制
void menu_update(MENU_STATE state,int mode);//窗口更新
void menu_pageup(void);//窗口上翻页
void menu_pagedown(void);//窗口下翻页
void menu_clear_window(void);//窗口清空
void show_time(signed short x,signed short y);//时钟显示
void usart_window_update(char *text);//串口控制显示
void can_window_update(uint32_t Std_ID,uint8_t aData[]);//can控制显示
void GUI_main();//GUI主函数
void GUI_Reload();//重新初始化屏幕，程序白屏时恢复显示
void draw_point_onMap(float x,float y);//在XY图上画点
