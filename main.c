#include "stm32f10x.h"
#include "usart_rxtx.h"
#include "gpio_led.h"
#include "tim_timer.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "adc-dma.h"

vu16 adcValue[4] = {};
unsigned char buf[21] = {};

void TIM2_IRQHandler(void) {
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	static u16 cnt = 0;
	GPIOA->ODR ^= GPIO_Pin_11;
	if(cnt++ > 250){
		cnt = 0;

		int2str(&buf[0], adcValue[0]);
		int2str(&buf[5], adcValue[1]);
		int2str(&buf[10], adcValue[2]);
		int2str(&buf[15], adcValue[3]);
		UARTSend(buf, 21);
	}
}

int main(void) {
	usart_init();
	gpio_init();
	timer_init();
	buf[4] = ',';
	buf[9] = ',';
	buf[14] = ',';
	buf[19] = '\r';
	buf[20] = '\n';
	adc_init();


	while (1) {

	}
}
