
#ifndef LEDPWM_H_
#define LEDPWM_H_

#include <string.h>
#include <math.h>
#include "main.h"
#include "stm32f407xx.h"

// Settings
#define LEDPWM_LED_NUM		3
#define LEDPWM_CNT_TOP		1000

#define LED_GREEN			1
#define LED_RED				0
#define LED_BLUE            2

// Functions
void ledpwm_init(void);
void ledpwm_set_intensity(unsigned int led, float intensity);
void LED_main(void);

#endif /* LEDPWM_H_ */
