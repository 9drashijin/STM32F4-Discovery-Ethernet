#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#define turnOnLED1() 	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
#define turnOffLED1() 	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
#define turnOnLED2() 	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
#define turnOffLED2()	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
#define turnOnLED3() 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
#define turnOffLED3()	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
#define turnOnLED4() 	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
#define turnOffLED4()	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);

#define switchControl()	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

#define initTaskBlock(x) 	((x)->state = 0)
#define yield(x) 			(x)->state = __LINE__; break; case __LINE__:
#define startTask(x) 		switch((x)->state) { case 0:
#define endTask(x) 			}

typedef enum{
	LED_INITIAL,
	LED_ON_STATE,
	LED_OFF_STATE
	//LED_FINAL
}State;

typedef struct{
	State state;
}TaskBlock;

uint32_t currentTime = 0;
int FAST_BLINK = 20;
static int FAST_BLINKS = 20;

void initControl()
{
	GPIO_InitTypeDef GpioInfo;

	__GPIOA_CLK_ENABLE();

	GpioInfo.Alternate	= GPIO_MODE_AF_PP;
	GpioInfo.Mode		= GPIO_MODE_INPUT;
	GpioInfo.Pin 		= GPIO_PIN_0;
	GpioInfo.Pull		= GPIO_NOPULL;
	GpioInfo.Speed		= GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOA, &GpioInfo);
}

void initLED1()
{
	GPIO_InitTypeDef GpioInfo;

	__GPIOG_CLK_ENABLE();

	GpioInfo.Alternate	= GPIO_MODE_AF_PP;
	GpioInfo.Mode		= GPIO_MODE_OUTPUT_PP;
	GpioInfo.Pin 		= GPIO_PIN_13;
	GpioInfo.Pull		= GPIO_NOPULL;
	GpioInfo.Speed		= GPIO_SPEED_HIGH;

	// Initialize LED on PG13
	HAL_GPIO_Init(GPIOG, &GpioInfo);
}

void initLED2()
{
	GPIO_InitTypeDef GpioInfo;

	__GPIOG_CLK_ENABLE();

	GpioInfo.Alternate	= GPIO_MODE_AF_PP;
	GpioInfo.Mode		= GPIO_MODE_OUTPUT_PP;
	GpioInfo.Pin 		= GPIO_PIN_14;
	GpioInfo.Pull		= GPIO_NOPULL;
	GpioInfo.Speed		= GPIO_SPEED_HIGH;

	// Initialize LED on PG14
	HAL_GPIO_Init(GPIOG, &GpioInfo);
}

void initLED3()
{
	GPIO_InitTypeDef GpioInfo;

	__GPIOB_CLK_ENABLE();

	GpioInfo.Alternate	= GPIO_MODE_AF_PP;
	GpioInfo.Mode		= GPIO_MODE_OUTPUT_PP;
	GpioInfo.Pin 		= GPIO_PIN_13;
	GpioInfo.Pull		= GPIO_NOPULL;
	GpioInfo.Speed		= GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOB, &GpioInfo);
}

void initLED4()
{
	GPIO_InitTypeDef GpioInfo;

	__GPIOC_CLK_ENABLE();

	GpioInfo.Alternate	= GPIO_MODE_AF_PP;
	GpioInfo.Mode		= GPIO_MODE_OUTPUT_PP;
	GpioInfo.Pin 		= GPIO_PIN_5;
	GpioInfo.Pull		= GPIO_NOPULL;
	GpioInfo.Speed		= GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOC, &GpioInfo);
}

uint32_t getCurrentTime()
{
	static uint32_t counter = 0;
	static uint32_t currentTime = 0;

	if(counter++ >= 1000)
	{
		counter = 0;
		currentTime++;
	}
	return currentTime;
}

int delay(uint32_t delayCycle, uint32_t previousTime){

	if(((currentTime = getCurrentTime()) - previousTime) < delayCycle)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

//void delays(uint32_t delayCycle)
//{
//	static uint32_t previousTime = 0;
//
//	while((getCurrentTime() - previousTime) < delayCycle);
//	previousTime = getCurrentTime();
//}

void blink_LED1()
{
	static State state = LED_INITIAL;
	static uint32_t previousTime = 0;
	switch(state)
	{
		case LED_INITIAL:   turnOnLED1();turnOnLED3();
							if(delay(10, previousTime))
							{
								state = LED_ON_STATE;
								previousTime = currentTime;
							}
							break;

		case LED_ON_STATE:  turnOnLED1();turnOnLED3();
							if(delay(10, previousTime))
							{
								state = LED_OFF_STATE;
								previousTime = currentTime;
							}
							break;

		case LED_OFF_STATE: turnOffLED1();turnOffLED3();
							if(delay(10, previousTime))
							{
								state = LED_INITIAL;
								previousTime = currentTime;
							}
							break;
	}
}

void blink_LED2()
{
	static State state = LED_INITIAL;
	static uint32_t previousTime = 0;

	if(switchControl() == GPIO_PIN_SET)
	{
		FAST_BLINK = 5;
	}
	else
	{
		FAST_BLINK = 20;
	}

		switch(state)
		{
			case LED_INITIAL:   if(delay(FAST_BLINK,previousTime))
								{
									turnOnLED2();
									state = LED_ON_STATE;
									previousTime = currentTime;
								}
								break;

			case LED_ON_STATE:  if(delay(FAST_BLINK,previousTime))
								{
									turnOnLED2();
									state = LED_OFF_STATE;
									previousTime = currentTime;
								}
								break;

			case LED_OFF_STATE: if(delay(FAST_BLINK,previousTime))
								{
									turnOffLED2();
									state = LED_INITIAL;
									previousTime = currentTime;
								}
								break;
		}
}

void blink_LED3()
{
	static State state = LED_INITIAL;
	static uint32_t previousTime = 0;
	static uint32_t count;

	if(switchControl() == GPIO_PIN_SET)
	{
		FAST_BLINKS = 5;
		count = 5;
	}
	else
	{
		FAST_BLINKS = 20;
	}

		switch(state)
		{
			case LED_INITIAL:   if(delay(FAST_BLINKS,previousTime))
								{
									turnOnLED4();
									state = LED_ON_STATE;
									previousTime = currentTime;
								}
								break;

			case LED_ON_STATE:  if(delay(FAST_BLINKS,previousTime))
								{
									turnOnLED4();
									count--;
									state = LED_OFF_STATE;
									previousTime = currentTime;

								}
								if(count == 0)
								{
									state = LED_INITIAL;
								}
								break;

			case LED_OFF_STATE: if(delay(FAST_BLINKS,previousTime))
								{
									turnOffLED4();
									state = LED_INITIAL;
									previousTime = currentTime;
								}
								break;
		}
}
void yieldTest(TaskBlock *tb)
{
	static uint32_t state = 0;
	static int here = 0;
	while(1)
	{
		startTask(tb);

		here = 0;
		yield(tb);
		here = 1;
		yield(tb);
		here = 2;
		yield(tb);
		here = 3;
		yield(tb);

		endTask();

	}
}

int main(void)
{
	TaskBlock tb;
	initTaskBlock(&tb);

	initControl();
	initLED1();
	initLED2();
	initLED3();
	initLED4();

	//yieldTest(&tb); //stop here

	while(1)
		{
			blink_LED1();
			blink_LED2();
			blink_LED3();
		}
}
