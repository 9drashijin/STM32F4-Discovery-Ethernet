#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

extern ETH_HandleTypeDef heth;

uint32_t Ethernet_Init();
void init_CLock();
void init_PortA();
void init_PortB();
void init_PortC();
