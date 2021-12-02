#include "main.h"


int32_t weight_grams=0; //глобальная переменная, в которой хранится значение веса
uint8_t button=Button_Off; //режим для кнопки
uint8_t mode = calib; //режим работы, начала с колибровки весов


void clock48Mhz_init() { //установка максимальной частоты
	RCC->CR &= ~RCC_CR_PLLON;
	while(RCC->CR & RCC_CR_PLLRDY);

	RCC->CFGR |= RCC_CFGR_PLLMUL12;

	RCC->CR |= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

	RCC->CFGR |= RCC_CFGR_SW_1;	//as PLL

	SystemCoreClockUpdate();
}

int main(void){
	clock48Mhz_init();
	TFT_init(); //инциализация экрана
	Button_init(); //инциализация кнопки
	HX711_init(); //инциализация внешнего АЦП
	while(1){
		HX711_get_data(&weight_grams); // получение данных с АЦП
		Print(&weight_grams); //вывод данных на экран
	}
}

