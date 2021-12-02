#ifndef GLOBAL_H_
#define GLOBAL_H_


#include "stm32f0xx.h"


/**************************** FOR TFT DISPLAY
 * PA4 - CS
 * PA2 - RESET
 * PA3 - DC (data command)
 * PA7 - MOSI
 * PA5 - SCK
 * PA6 - MISO
 * PA1 - LED
 * ***************************************
 * for HX711
 *
 * PB0 - SCK
 * PB1 - DOUT
 *
 * **/


//значения переменной mode
#define get_weight 1   //получить вес
#define get_weight_ready 2 //вес получен
#define words 3 //вывести слова на экран
#define setSc 4 // установить scale
#define calib 5 //откалибровать (начальное значение переменной)
#define memory_weight_ready 6 // вес тары задан


//высота букв и цифр
#define FONT_Y 8
#define FONT_X 8

//включена или выключена кнопка
#define Button_On 1
#define Button_Off 0

// значение сброса для экрана
#define RESET_ACTIVE() GPIOA->ODR&=~GPIO_ODR_2
#define RESET_IDLE() GPIOA->ODR|=GPIO_ODR_2

//значение пина DC (команда или данные)
#define DC_COMMAND() GPIOA->ODR&=~GPIO_ODR_3
#define DC_DATA() GPIOA->ODR|=GPIO_ODR_3

// установка/сброс CS
#define SET_CS() GPIOA->ODR|=GPIO_ODR_4
#define RESET_CS() GPIOA->ODR&=~GPIO_ODR_4

// включение подсветки
#define LED_on() GPIOA->ODR|=GPIO_ODR_1
#define LED_off() GPIOA->ODR&=~GPIO_ODR_1

//установка/сброс SCK для внешнего АЦП
#define SCK_HIGH() GPIOB->ODR|=GPIO_ODR_8
#define SCK_LOW() GPIOB->ODR&=~GPIO_ODR_8

// старт отправки данных по DMA и SPI
#define Start_DMA_Send_Data() DMA1_Channel3->CCR |= DMA_CCR_EN



#endif /* GLOBAL_H_ */
