#include "stm32f10x.h"
#include "usart_rxtx.h"
#include "gpio_led.h"
#include "tim_timer.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "adc-dma.h"
#include "tim_pwm.h"
#include "usart_servo.h"


vu16 adcValue[4] = { };
unsigned char buf[21] = {
		0, 0, 0, 0, ',',
		0, 0, 0, 0, ',',
		0, 0, 0, 0, ',',
		0, 0, 0, 0, '\r', '\n' };

unsigned char temp[7] = { 0, 0, 0, 0, 0, '\r', '\n' };

/**
 * receive from PC
 */
void USART1_IRQHandler(void) {
	static uint16_t c;
	if ((USART1->SR & USART_FLAG_RXNE) != (u16) RESET) {
		c = USART_ReceiveData(USART1);
		USART_SendData(USART1, c);

		if (c == 'd' ){
			TIM3->CCR1 = 0;
		}
		int2str2(temp, TIM3->CCR1);
		UARTSend(temp, 7);
	}
}

/**
 * receive parcket from Servo motor
 */
void USART3_IRQHandler(void) {
	static char raw[30];
	static int index = 0;
	if ((USART3->SR & USART_FLAG_RXNE) != (u16) RESET){
		raw[index++] = (char) 0xFF & USART_ReceiveData(USART3);
	}

}

/**
 * timer counter
 */
void TIM2_IRQHandler(void) {
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	static uint8_t cnt;
	adc2str(buf, adcValue);
	GPIOA->ODR ^= GPIO_Pin_11;
	//UARTSend(buf, 21);
	GPIOA->ODR ^= GPIO_Pin_11;

}

int main(void) {
	usart_init();
	usart_servo_init();
	gpio_init();
	timer_init();
	adc_init();
	pwm_init();

	while (1) {

	}
}
