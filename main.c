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


TxData gTxData = {{0xFF, 0xFF, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66}, };
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
		USART_SendData(USART1, c);
		if ( c == 'r' ){
//			TIM3->CCR1 += 10;
			TIM_Cmd(TIM2, ENABLE);
		}
		if ( c == 'q' ){
//			TIM3->CCR1 += 10;
			TIM_Cmd(TIM2, DISABLE);
		}
//
//		if (c == 's' ){
//			TIM3->CCR1 -= 10;
//		}
//
//		if (c == 'd' ){
//			TIM3->CCR1 = 0;
//		}
//
//		if (c == '0' ){
////			TIM3->CCR1 = 0;
//			statusSize = ServoLED(0);
//		}
//		if (c == '9' ){
//			statusSize = ServoLED(255);
//		}
//		vu16 v;
//		if (c == '7' ){
//			v += 10000;
//			statusSize = SetPosition(v);
//		}
//		if (c == '8' ){
//			v -= 10000;
//			statusSize = SetPosition(v);
//		}
//		if (c == '6' ){
//			v = 0;
//			SetPosition(v);
//		}
//		if (c == '5' ){
//			TorqueMode(1);
//		}
//		if (c == '4' ){
//			TorqueMode(0);
//		}
//		if (c == '1' ){
//			CheckError();
//		}
//		if (c == 'v' ){
//			u32 v;
//			statusSize = GetPosition(&v);
//		}
//		if (c == 's' ){
//			u32 v=15000;
//			statusSize = SetVelocity(v);
//		}
//		if (c == 'x' ){
//			u32 v=1000;
//			statusSize = SetVelocity(v);
//		}
//		if (c == 'p' ){
//			UARTSend(statusPacket, statusSize);
//		}
//		int2str2(temp, TIM3->CCR1);

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
