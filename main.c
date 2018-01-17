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

unsigned char buf[25] = {
		0, 0, 0, 0, ',',
		0, 0, 0, 0, ',',
		0, 0, 0, 0, ',',
		0, 0, 0, 0, ',',
		0, 0, 0, 0, '\n' };

unsigned char temp[7] = { 0, 0, 0, 0, 0, '\r', '\n' };

/**
 * receive from PC
 */
void USART1_IRQHandler(void) {
	static uint16_t index = 0;
	static union{
		u8 raw[4];
		u32 angle;
	} data;

	if ((USART1->SR & USART_FLAG_RXNE) != (u16) RESET) {
		data.raw[index++] = (char) 0xFF & USART_ReceiveData(USART1);
		if(index >= 4){
			SetPosition(data.angle);
			index = 0;
		}

//		if ( c == 'a' ){
//			TIM3->CCR1 += 10;
//		}
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

//		int2str2(temp, TIM3->CCR1);
//		int2str2(temp, enc);
//		UARTSend(temp, 7);
//
//		int2str2(temp, TIM3->CCR1);
//		UARTSend(temp, 7);

	}

}

/**
 * timer counter
 */
void TIM2_IRQHandler(void) {
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	static vu16 enc;
	GPIOA->ODR ^= GPIO_Pin_12;

	adc2str(&buf, adcValue);
	enc = TIM_GetCounter(TIM4);
//	int2str(&buf[20], enc);
	UARTSend(buf, 25);
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
