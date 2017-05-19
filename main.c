#include "stm32f10x.h"
#include "usart_rxtx.h"
#include "gpio_led.h"
#include "tim_timer.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

void TIM2_IRQHandler(void) {
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	GPIOA->ODR ^= GPIO_Pin_11;
}

int main(void) {
	usart_init();
	gpio_init();
	timer_init();

	while (1) {

	}
}
