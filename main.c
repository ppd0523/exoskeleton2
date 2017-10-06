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


u8 statusPacket[50] = { 0xAA, 0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
u8 statusSize = 0;
vu16 adcValue[4] = { };
vu16 enc = 0;

unsigned char buf[21] = {
		0, 0, 0, 0, ',',
		0, 0, 0, 0, ',',
		0, 0, 0, 0, ',',
		0, 0, 0, 0, '\r', '\n' };

unsigned char temp[7] = { 0, 0, 0, 0, 0, '\r', '\n' };
unsigned char temp2[6] = { 0, 0, 0, 0, 0, '\r'};

/**
 * receive from PC
 */
void USART1_IRQHandler(void) {
	static uint16_t c;
	if ((USART1->SR & USART_FLAG_RXNE) != (u16) RESET) {
		c = USART_ReceiveData(USART1);
		USART_SendData(USART1, c);
		if ( c == 'a' ){
			TIM3->CCR1 += 10;
		}

		if (c == 's' ){
			TIM3->CCR1 -= 10;
		}

		if (c == 'd' ){
			TIM3->CCR1 = 0;
		}

		if (c == '0' ){
//			TIM3->CCR1 = 0;
			statusSize = ServoLED(0);
		}
		if (c == '9' ){
			statusSize = ServoLED(255);
		}
		vu16 v;
		if (c == '7' ){
			v += 10000;
			statusSize = SetPosition(v);
		}
		if (c == '8' ){
			v -= 10000;
			statusSize = SetPosition(v);
		}
		if (c == '6' ){
			v = 0;
			SetPosition(v);
		}
		if (c == '5' ){
			TorqueMode(1);
		}
		if (c == '4' ){
			TorqueMode(0);
		}
		if (c == '1' ){
			CheckError();
		}
		if (c == 'v' ){
			u32 v;
			statusSize = GetPosition(&v);
		}
		if (c == 's' ){
			u32 v=15000;
			statusSize = SetVelocity(v);
		}
		if (c == 'x' ){
			u32 v=1000;
			statusSize = SetVelocity(v);
		}
		if (c == 'p' ){
			UARTSend(statusPacket, statusSize);
		}
//		int2str2(temp, TIM3->CCR1);
//		UARTSend(temp, 7);
	}
}

/**
 * receive parcket from Servo motor
 */
void USART3_IRQHandler(void) {
	static int index = 0;
	if ((USART3->SR & USART_FLAG_RXNE) != (u16) RESET){
		statusPacket[index++] = (char) 0xFF & USART_ReceiveData(USART3);
		//char c = (char) 0xFF & USART_ReceiveData(USART3);
		//USART_SendData(USART1, c);
		if(index >= statusSize){
			//USART_SendData(USART1, statusPacket);
			UARTSend(statusPacket, index);
			index = 0;
		}

//		int2str2(temp, TIM3->CCR1);
//		int2str2(temp, enc);
		UARTSend(temp, 7);

		int2str2(temp, TIM3->CCR1);
		UARTSend(temp, 7);

	}

}

/**
 * timer counter
 */
void TIM2_IRQHandler(void) {
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	static uint8_t cnt;
	static vu16 enc0;
//	adc2str(buf, adcValue);
	GPIOA->ODR ^= GPIO_Pin_11;
//	enc0 = enc;
//	enc = TIM_GetCounter(TIM4);
	//enc = TIM3->CNT;
//	int2str2(temp2, enc);
//	UARTSend(temp2, 6);
	GPIOA->ODR ^= GPIO_Pin_11;
}

void TIM4_IRQHandler(void){
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//	enc = TIM_GetCounter(TIM4);
//	int d = (TIM4->CR1 & TIM_CR1_DIR) ? 1 : -1;
//	enc += d;
}

int main(void) {
	usart_init();
	usart_servo_init();
	gpio_init();
	timer_init();
	adc_init();
	pwm_init();
	encoder_init();

	while (1) {

	}

	return 0;
}
