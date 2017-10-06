#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "tim_encoder.h"

void encoder_init() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB/* | RCC_APB2Periph_AFIO*/, ENABLE);

	//PB6: TIM4_ch1, PB7: TIM4_ch2
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	//GPIO_Mode_IN_FLOATING; GPIO_Mode_IPD,GPIO_Mode_AF_OD
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*Configure peripheral I/O remapping */
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM4, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_TimeBaseStructure.TIM_Period = (1024 - 1); // pgm-12 encode_env.pos (ppr * step * gear) = 13*2*264 = 768
//	TIM_TimeBaseStructure.TIM_Prescaler = 0;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//ICFilter Specifies the input capture filter 0~0xf
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 5;
	TIM_ICInit(TIM4, &TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter = 5;//3
	TIM_ICInit(TIM4, &TIM_ICInitStruct);

	// TIM_ICPolarity_Rising,TIM_ICPolarity_Falling,TIM_ICPolarity_BothEdge
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,
			TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
	//TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);

	/* Enable the TIM4 gloabal Interrupt */

	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //TIM_IT_CC1;TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //co to k?
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	NVIC_EnableIRQ(TIM4_IRQn);
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}
