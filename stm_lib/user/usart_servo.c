#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "misc.h"
#include "usart_servo.h"

void NVIC_Configuration_USART3(void);
void GPIO_Configuration_USART3(void);
void USART3_Configuration(void);
void UART3Send(const unsigned char *pucBuffer, unsigned long ulCount);
void USART3_IRQHandler(void);

void usart_servo_init(void) {
	/* Enable USART3 and GPIOA clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* NVIC Configuration */
	NVIC_Configuration_USART3();

	/* Configure the GPIOs */
	GPIO_Configuration_USART3();

	/* Configure the USART3 */
	USART3_Configuration();

	/* Enable the USART3 Receive interrupt: this interrupt is generated when the
	 USART3 receive data register is not empty */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

void GPIO_Configuration_USART3(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure USART3 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USART3 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure GPIO Direction for HALF DUPLEX (PB.5) as push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	// ax-12a 와 통신하기 위한 핀. 모터에게 데이터를 보낼때 set,모터한테 데이터를 받을때 reset
}

void USART3_Configuration(void) {
	USART_InitTypeDef USART3_InitStructure;

	/* USART3 configuration ------------------------------------------------------*/
	/* USART3 configured as follow:
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
	USART3_InitStructure.USART_BaudRate = 115200;
	USART3_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART3_InitStructure.USART_StopBits = USART_StopBits_1;
	USART3_InitStructure.USART_Parity = USART_Parity_No;
	USART3_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART3_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART3_InitStructure);

	/* Enable USART */
	USART_Cmd(USART3, ENABLE);
}

void NVIC_Configuration_USART3(void) {
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART3Send(const unsigned char *pucBuffer, unsigned long ulCount) {
	//
	// Loop while there are more characters to send.
	//
	//GPIO_SetBits(GPIOB, GPIO_Pin_6);
	while (ulCount--) {
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART3, *pucBuffer++);
	}
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		;
	//GPIO_ResetBits(GPIOB, GPIO_Pin_6);
}
