#include "stm32f10x.h"
#include "usart_rxtx.h"
#include "gpio_led.h"
#include "tim_timer.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "adc-dma.h"

vu16 adcValue[4] = { };
unsigned char buf[21] = {
		0, 0, 0, 0, ',',
		0, 0, 0, 0, ',',
		0, 0, 0, 0, ',',
		0, 0, 0, 0, '\r', '\n' };

void TIM2_IRQHandler(void) {
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	adc2str(buf, adcValue);
	GPIOA->ODR ^= GPIO_Pin_11;
	UARTSend(buf, 21);
	GPIOA->ODR ^= GPIO_Pin_11;

}

int main(void) {
	usart_init();
	gpio_init();
	timer_init();
	adc_init();

	while (1) {

	}
}
