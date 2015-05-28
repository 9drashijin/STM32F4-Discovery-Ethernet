#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_eth.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "Ethernet.h"

/* Ethernet Pins Configuration
 * ===========================
 * ETH_MDC---------------->PC1
 * ETH_MDIO--------------->PA2
 * ETH_RMII_REF_CLK------->PA1
 * ETH_RMII_CRS_DV-------->PA7
 * ETH_RMII_RXD0---------->PC4
 * ETH_RMII_RXD1---------->PC5
 * ETH_RMII_TX_EN--------->PB11
 * ETH_RMII_TXD0---------->PB12
 * ETH_RMII_TXD1---------->PB13
 * */

void init_CLock()
{
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();

	__SYSCFG_CLK_ENABLE();
	__ETH_CLK_ENABLE();
	__HAL_RCC_ETHMAC_CLK_ENABLE();
}

void init_PortA()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Alternate	= GPIO_AF11_ETH;
	GpioInfo.Mode		= GPIO_MODE_AF_PP;
	GpioInfo.Pin 		= GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
	GpioInfo.Pull		= GPIO_NOPULL;
	GpioInfo.Speed		= GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOA, &GpioInfo);
}

void init_PortB()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Alternate	= GPIO_AF11_ETH;
	GpioInfo.Mode		= GPIO_MODE_OUTPUT_PP;
	GpioInfo.Pin 		= GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
	GpioInfo.Pull		= GPIO_NOPULL;
	GpioInfo.Speed		= GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOB, &GpioInfo);
}

void init_PortC()
{
	GPIO_InitTypeDef GpioInfo;

	GpioInfo.Alternate	= GPIO_AF11_ETH;
	GpioInfo.Mode		= GPIO_MODE_INPUT;
	GpioInfo.Pin 		= GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
	GpioInfo.Pull		= GPIO_NOPULL;
	GpioInfo.Speed		= GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOC, &GpioInfo);
}

uint32_t Ethernet_Init()
{
	ETH_InitTypeDef init;
	ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB]; //Rx & Tx DMA Descriptors
	uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE]; // Receive buffers
	uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE]; // Transmit buffers

	uint8_t mac[6] = {0x12,0x34,0x13,0x14,0x00,0x10};

	init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;  // ETH_AUTONEGOTIATION_DISABLE
	init.Speed = ETH_SPEED_100M;							// ETH_SPEED_10M
	init.DuplexMode = ETH_MODE_FULLDUPLEX; 				// ETH_MODE_HALFDUPLEX
	init.PhyAddress = 0x0; //This parameter must be a number between Min_Data = 0 and Max_Data = 32
	init.MACAddr = mac; //MAC Address of used Hardware: must be pointer on an array of 6 bytes
	init.RxMode = ETH_RXPOLLING_MODE; 					//ETH_RXINTERRUPT_MODE
	init.ChecksumMode = ETH_CHECKSUM_BY_SOFTWARE; 		// ETH_CHECKSUM_BY_HARDWARE
	init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;		// ETH_MEDIA_INTERFACE_RMII

	heth.Init = init;

	HAL_ETH_Init(&heth);

	init_CLock();
	init_PortA();
	init_PortB();
	init_PortC();

	HAL_ETH_DMATxDescListInit(&heth, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
	HAL_ETH_DMARxDescListInit(&heth, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

	return 1;
}
