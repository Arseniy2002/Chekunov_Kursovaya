#ifndef HX711_H_
#define HX711_H_

#include "stm32f0xx.h"
#include "global.h"

#define weight_of_standard 183 // вес по умолчанию для калибровки

void HX711_init(); //инциализация АЦП
int32_t HX711_Get_3byte(); //получение данных (возвращает данные в формате int32_t)
void pulse(uint8_t time); //допольнительный импульс для выбора канала преобразования АЦП
int32_t HX711_read_average(uint8_t times); // взять данные с АЦП times раз и вернуть среднее значение (int32_t)
int32_t get_gramms(uint8_t times); //получить вес в граммах
void HX711_get_data(int32_t *weight_grams); //получить данные с АЦП
void power_down(); //сброс АЦП
void Calibration(); //калибровка АЦП
void Set_offset(int32_t data); //установка начального значения
void Set_scale(int32_t scale); // установка масштаба (количество отсчетов АЦП в 1 грамме)
int32_t calibration_array(); //полученние колибровочного коэффициента
void GPIOB_init_HX711(); //настройка портов для АЦП

#endif /* HX711_H_ */
