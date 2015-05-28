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

#define yieldLED() 		state = __LINE__; } break; case __LINE__:
#define startTaskLED()		switch(state) { case 0:
#define endTaskLED()		state = 0;} break;}

typedef enum{
	LED_INITIAL,
	LED_ON_STATE,
	LED_OFF_STATE
	//LED_FINAL
}State;

typedef struct{
	uint32_t state;
}TaskBlock;

void initControl();
void initLED1();
void initLED2();
void initLED3();
void initLED4();
uint32_t getCurrentTime();
int delay(uint32_t delayCycle, uint32_t previousTime);
void delays(uint32_t delayCycle);
void blink_LED1();
void blink_LED2();
void blink_LED3();
void blink_LED1_yield();
void yieldTest(TaskBlock *tb);
