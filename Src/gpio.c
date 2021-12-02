#include "gpio.h"

void Button_init(){
	Button_GPIOA_Init();
	TIM6_init();
}

void Button_GPIOA_Init(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &=~GPIO_MODER_MODER0;
	GPIOA->PUPDR|=GPIO_PUPDR_PUPDR0_1;
	SYSCFG->EXTICR[1]&=~(0x0F<<0); // разрешение прерывание по пину РА0

	EXTI->IMR|= EXTI_IMR_IM0; // установка маски прерывания
	EXTI->EMR|= EXTI_EMR_EM0; // установка маски события

	EXTI->RTSR|= EXTI_RTSR_RT0; // прерывание по возрастающему фронту

	NVIC_SetPriority(EXTI0_1_IRQn, 1); // установка приоритета прерывания
	//самый высокий приоритет, чтобы по нажатию кнопки выключить и включить считывание пространства
	NVIC_EnableIRQ(EXTI0_1_IRQn);//разрешение прерывания
}

void EXTI0_1_IRQHandler(){
	EXTI->PR |= EXTI_PR_PIF0;
	TIM6->CR1 |= TIM_CR1_CEN;// включение таймера
}
