#include "stm32f10x.h"
#include "usart_rxtx.h"
#include "gpio_led.h"
#include "tim_timer.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "adc-dma.h"
#include "tim_pwm.h"
#include "tim_encoder.h"


TxData gTxData = {};
u8 statusPacket[50] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
		0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
u8 statusSize = 0;

vu16 adcValue[2] = {0x1122, 0x3344 };


/**
 * receive from PC
 */
void USART1_IRQHandler(void) {
	if ((USART1->SR & USART_FLAG_RXNE) != (u16) RESET) {
		char c = (char) 0xFF & USART_ReceiveData(USART1);
//		USART_SendData(USART1, c);
		if ( c == 'r' ){
//			TIM3->CCR1 += 10;
			TIM_Cmd(TIM2, ENABLE);
		}
		else if ( c == 'q' ){
//			TIM3->CCR1 += 10;
			TIM_Cmd(TIM2, DISABLE);
		}


	}
}

/**
 * receive parcket from Servo motor
 */
void USART3_IRQHandler(void) {
	static int index = 0;

	if ((USART3->SR & USART_FLAG_RXNE) != (u16) RESET) {
		statusPacket[index++] = (char) 0xFF & USART_ReceiveData(USART3);

	}

}

/**
 * timer counter
 */
void TIM2_IRQHandler(void) {
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	GPIOA->ODR ^= GPIO_Pin_12;

//	memcpy((void*)gTxData.raw+2, (void*)adcValue, 4);
//	gTxData.parse.enc = TIM_GetCounter(TIM4);

	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4, 8);
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

int main(void) {
	gTxData.raw[0] = 0xFF;
	gTxData.raw[1] = 0xFF;
	gTxData.raw[2] = 0x22;
	gTxData.raw[3] = 0x11;
	gTxData.raw[4] = 0x44;
	gTxData.raw[5] = 0x33;
	gTxData.raw[6] = 0x66;
	gTxData.raw[7] = 0x55;

	usart_init();
	timer_init();
	usart_servo_init();
	gpio_init();

	adc_init();
	pwm_init();
	encoder_init();
//	TorqueMode(1);
//
//	volatile int c = 0;
//	for(int i=0; i<0xFFFF; ++i)
//		for(int j=0; j<0xFFFF; ++j)
//			c = c + 1;
//
//	SetReturnLevel(1);

	while (1) {

	}

	return 0;
}
