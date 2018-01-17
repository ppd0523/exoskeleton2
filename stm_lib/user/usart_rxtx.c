/**
 ******************************************************************************
 * @file    USART/USART_RxTx/main.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    30-October-2010
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "usart_rxtx.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/
/**
 * @brief  This function handles USARTx global interrupt request.
 * @param  None
 * @retval None
 */


/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : usart_rxtx
 * Description    : Print "Welcome to CooCox!"  on Hyperterminal via USART1.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void usart_init(void) {
	const unsigned char menu[] = " Welcome to CooCox!\r\n";

	/* Enable USART1 and GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,
			ENABLE);

	/* NVIC Configuration */
	NVIC_Configuration();

	/* Configure the GPIOs */
	GPIO_Configuration();

	/* Configure the USART1 */
	USART_Configuration();

	/* Enable the USART1 Receive interrupt: this interrupt is generated when the
	 USART1 receive data register is not empty */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* print welcome information */
	UARTSend(menu, sizeof(menu));

}

/*******************************************************************************
 * Function Name  : GPIO_Configuration
 * Description    : Configures the different GPIO ports.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void GPIO_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
 * Function Name  : USART_Configuration
 * Description    : Configures the USART1.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void USART_Configuration(void) {
	USART_InitTypeDef USART_InitStructure;

	/* USART1 configuration ------------------------------------------------------*/
	/* USART1 configured as follow:
	 - BaudRate = 115200 baud
	 - Word Length = 8 Bits
	 - One Stop Bit
	 - No parity
	 - Hardware flow control disabled (RTS and CTS signals)
	 - Receive and transmit enabled
	 - USART Clock disabled
	 - USART CPOL: Clock is active low
	 - USART CPHA: Data is captured on the middle
	 - USART LastBit: The clock pulse of the last data bit is not output to
	 the SCLK pin
	 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);

	/* Enable USART1 */
	USART_Cmd(USART1, ENABLE);
}

/**
 * @brief  Configures the nested vectored interrupt controller.
 * @param  None
 * @retval None
 */
void NVIC_Configuration(void) {
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
 * Function Name  : UARTSend
 * Description    : Send a string to the UART.
 * Input          : - pucBuffer: buffers to be printed.
 *                : - ulCount  : buffer's length
 * Output         : None
 * Return         : None
 *******************************************************************************/
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount) {
	//
	// Loop while there are more characters to send.
	//
	while (ulCount--) {
		USART_SendData(USART1, *pucBuffer); // Last Version USART_SendData(USART1,(uint16_t) *pucBuffer++);
		++pucBuffer;
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {
		}
	}
}

void adc2str(unsigned char buf[], uint16_t adc[]) {
	uint16_t n[4];// = {adc[0], adc[1], adc[2], adc[3]};
	memcpy(n, adc, 8);
//	int2str(&buf[0],n[0]);
//	int2str(&buf[5],n[1]);
//	int2str(&buf[10],n[2]);
//	int2str(&buf[15],n[3]);

	buf[0] = '0' + (n[0] / 1000);
	n[0] %= 1000;
	buf[1] = '0' + (n[0] / 100);
	n[0] %= 100;
	buf[2] = '0' + (n[0] / 10);
	n[0] %= 10;
	buf[3] = '0' + n[0];

	buf[5] = '0' + (n[1] / 1000);
	n[1] %= 1000;
	buf[6] = '0' + (n[1] / 100);
	n[1] %= 100;
	buf[7] = '0' + (n[1] / 10);
	n[1] %= 10;
	buf[8] = '0' + n[1];

	buf[10] = '0' + (n[2] / 1000);
	n[2] %= 1000;
	buf[11] = '0' + (n[2] / 100);
	n[2] %= 100;
	buf[12] = '0' + (n[2] / 10);
	n[2] %= 10;
	buf[13] = '0' + n[2];

	buf[15] = '0' + (n[3] / 1000);
	n[3] %= 1000;
	buf[16] = '0' + (n[3] / 100);
	n[3] %= 100;
	buf[17] = '0' + (n[3] / 10);
	n[3] %= 10;
	buf[18] = '0' + n[3];
}

void int2str(unsigned char buf[], uint16_t n) {
	buf[0] = '0' + (n / 1000);
	n %= 1000;
	buf[1] = '0' + (n / 100);
	n %= 100;
	buf[2] = '0' + (n / 10);
	n %= 10;
	buf[3] = '0' + n;
}

void int2str2(unsigned char buf[], uint16_t n){
	buf[0] = '0' + (n / 10000);
	n %= 10000;
	buf[1] = '0' + (n / 1000);
	n %= 1000;
	buf[2] = '0' + (n / 100);
	n %= 100;
	buf[3] = '0' + (n / 10);
	n %= 10;
	buf[4] = '0' + n;
}
/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
