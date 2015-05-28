#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "LED.h"

int main(void)
{
	TaskBlock tb;
	initTaskBlock(&tb);

	initControl();
	initLED1();
	initLED2();
	initLED3();
	initLED4();

	while(1)
		{
			//blink_LED1();
			//blink_LED2();
			//blink_LED3();

			blink_LED1_yield();
			//yieldTest(&tb); //stop here
		}
}
