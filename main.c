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

vu16 adcValue[4] = { };
vu16 enc = 0;
unsigned char buf[21] = {
		0, 0, 0, 0, ',',
		0, 0, 0, 0, ',',
		0, 0, 0, 0, ',',
		0, 0, 0, 0, '\r', '\n' };

unsigned char temp[7] = { 0, 0, 0, 0, 0, '\r', '\n' };
unsigned char temp2[6] = { 0, 0, 0, 0, 0, '\r'};

void USART1_IRQHandler(void) {
	static uint16_t c;
	if ((USART1->SR & USART_FLAG_RXNE) != (u16) RESET) {
		c = USART_ReceiveData(USART1);
		//USART_SendData(USART1, c);
		if ( c == 'a' ){
			TIM3->CCR1 += 10;
		} else if (c == 's'){
			TIM3->CCR1 -= 10;
		} else if (c == 'd'){
			TIM3->CCR1 = 0;
		} else if (c == 'f'){
			TIM3->CCR1 = 3600;
		} else if (c == 'q'){
			GPIO_WriteBit(GPIOA,GPIO_Pin_11, Bit_SET);
			GPIO_WriteBit(GPIOA,GPIO_Pin_12, Bit_RESET);
		} else if (c == 'w'){
			GPIO_WriteBit(GPIOA,GPIO_Pin_11, Bit_RESET);
			GPIO_WriteBit(GPIOA,GPIO_Pin_12, Bit_SET);
		}
//		int2str2(temp, TIM3->CCR1);
		int2str2(temp, enc);
		UARTSend(temp, 7);
	}
}

void TIM2_IRQHandler(void) {
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	adc2str(buf, adcValue);
	//GPIOA->ODR ^= GPIO_Pin_11;
	static vu16 enc0;
	enc0 = enc;
	enc = TIM_GetCounter(TIM4);
	//enc = TIM3->CNT;
	int2str2(temp2, enc);
	UARTSend(temp2, 6);
	//GPIOA->ODR ^= GPIO_Pin_11;
}

void TIM4_IRQHandler(void){
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//	enc = TIM_GetCounter(TIM4);
//	int d = (TIM4->CR1 & TIM_CR1_DIR) ? 1 : -1;
//	enc += d;
}

int main(void) {
	usart_init();
	gpio_init();
	timer_init();
	adc_init();
	pwm_init();
	encoder_init();

	while (1) {

	}
}
