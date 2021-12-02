#ifndef GPIO_H_
#define GPIO_H_


#include "global.h"

void Button_init();
void Button_GPIOA_Init();
void EXTI0_1_IRQHandler();

#endif /* GPIO_H_ */
