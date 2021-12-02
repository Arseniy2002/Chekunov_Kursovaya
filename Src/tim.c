#include "tim.h"


extern int32_t weight_grams;
int32_t memory_weight=0;
extern uint8_t mode;

extern uint8_t button;

void TIM6_init()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->CR1 |= TIM_CR1_OPM; //one pulse mode
	TIM6->PSC = 800-1;
	TIM6->ARR = 10-1;

	TIM6->DIER |= TIM_DIER_UIE;
	NVIC_SetPriority(TIM6_DAC_IRQn, 3);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

void TIM6_DAC_IRQHandler(){
	TIM6->SR &= ~TIM_SR_UIF;
	button=Button_On;
	if(weight_grams!=0){
		memory_weight=weight_grams;
	}
	mode = memory_weight_ready; //режим работы с памятью о значении веса
	TIM6->CR1 &= ~TIM_CR1_CEN;
}
