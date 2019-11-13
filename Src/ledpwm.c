#include "ledpwm.h"


// Private variables

/*
 * Set the intensity for one led. The intensity value is mapped to a PWM value
 * according to human luminance perception.
 *
 * Intensity range is 0.0 to 1.0
 */

void ledpwm_set_intensity(unsigned int led, float intensity) {
	if (led >= LEDPWM_LED_NUM) {
		return;
	}

	if (intensity < 0.0) {
		intensity = 0.0;
	}

	if (intensity > 1.0) {
		intensity = 1.0;
	}
        switch(led){
        case 0:TIM3->CCR2 = intensity*LEDPWM_CNT_TOP;break;
        case 1:TIM3-> CCR3 =  intensity*LEDPWM_CNT_TOP;break;
        case 2:TIM3-> CCR4 =  intensity*LEDPWM_CNT_TOP;break;
        }    
}

void ledpwm_init(void){
    ledpwm_set_intensity(LED_GREEN, 0);
    ledpwm_set_intensity(LED_RED,0);
    ledpwm_set_intensity(LED_BLUE,0);
}



void LED_main(void){
   
   if(!HAL_GPIO_ReadPin(GPIOC,KEY_UP_Pin))
      ledpwm_set_intensity(LED_GREEN,1);
   else ledpwm_set_intensity(LED_GREEN, 0.5);
   
   ledpwm_set_intensity(LED_BLUE, 0);
}
