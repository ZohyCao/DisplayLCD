#include "GUI_Frame.h" 

extern GUI_BITMAP bmroboconnew;
void show_robocon_image(void){
  GUI_DrawBitmapEx(&bmroboconnew,240,0,0,0,500,500);
}

void drawCoordiantes(int x0,int y0,int x1,int y1){
  int i,j;
  GUI_SetColor(GUI_BLACK);
  GUI_SetPenSize(1);
  for(i=x0+20;i<=x1;i+=20)
      GUI_DrawLine(i,0,i,320);
  
  for(j=y0+20;j<=y1;j+=20)
      GUI_DrawLine(0,j,480,j);
  GUI_SetPenSize(3);
  GUI_DrawLine((x0+x1)/2,0,(x0+x1)/2,320);
  GUI_DrawLine(0,(y0+y1)/2,480,(y0+y1)/2);  
  //GUI_DrawLine()
}