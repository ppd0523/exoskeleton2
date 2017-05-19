#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "tim_timer.h"

void TIM2_IRQHandler(void);

void timer_init(){
	NVIC_InitTypeDef NVIC_InitSructure;

	NVIC_InitSructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitSructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitSructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitSructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitSructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period = 7200;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_PrescalerConfig(TIM2, 4, TIM_PSCReloadMode_Immediate);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}
