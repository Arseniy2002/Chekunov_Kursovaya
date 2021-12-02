#include "HX711.h"


extern void delay(uint32_t tick);
extern uint8_t mode; //режим работы
int32_t array_weight[5]={0}; //массив с данными с АЦП
int32_t offset=0; //значение 0
extern int32_t memory_weight; //вес, который был последним при нажатии кнопки
int32_t weight=0; //вес в отсчетах АЦП

int32_t SCALE; //количество отсчетов АЦП в 1 грамме
int32_t calibration_value[5]; //колибровачный массив
int32_t calibration_factor=0; //колибровочный фактор

void HX711_init(){
	GPIOB_init_HX711(); // инциализируем пины
}



long HX711_Get_3byte(){
	int32_t data=0;
	int cnt=0;
	SCK_LOW(); //запуск на старт - на выход SCK подан низкий сигнал
	while(GPIOB->IDR & GPIO_IDR_9){// ждем, когда АЦП ответит 0, если не ответил выходим, и заново считываем
		cnt++;
		if(cnt>1000){
			SCK_HIGH();
			return 0;
		}
	}
	for (int i=0; i<24;i++){//24 bits
		data=data<<1; //сдвигиваем на 1 бит
		//говорим, что готовы принять еще 1 бит (на SCK подаем 1, ждем 1 мкс, считываем, подаем 0, ждем 30 мкс)
		SCK_HIGH();
		delay(1);
		if(GPIOB->IDR & GPIO_IDR_9){
			data++;
		}
		SCK_LOW();
		delay(40);
	}
	// 1 доп импульс для канала А и коэф 128
	pulse(1);
	data = data ^ 0x800000;
	return data;
}

void pulse(uint8_t time){
	for(uint8_t i=0; i<time;i++){
		SCK_HIGH();
		delay(1);
		SCK_LOW();
		delay(1);
	}
}

int32_t HX711_read_average(uint8_t times){ // считываем данные с АЦП times раз
	int32_t data=0;
	for(int i=0;i<times;i++){
		array_weight[i]=HX711_Get_3byte();
		if(array_weight[i]==0){
			i--;
		}
	}
	for(int i=0;i<times;i++){
		data+=array_weight[i];
	}
	data = data/times;
	data = data - offset; // вычитаем смещение
	delay(1000);
	return data;
}

int32_t get_gramms(uint8_t times){ //получаем данные в граммах
	int32_t data = 0;
	data = HX711_read_average(times);
	data = data / SCALE;
	delay(100);
	return data;
}

void HX711_get_data(int32_t *weight_grams){
	if(mode == calib){
		Calibration();
		return;
	}
	if (mode == setSc){
		int32_t data = 0;
		data = calibration_array();
		Set_scale(data); // считываем значение с весом и обновляем калибровочный коэф
		mode = get_weight;
		return;
	}
	if(mode == get_weight){
		int32_t data = 0;
		data = get_gramms(1);
		*weight_grams = data;
		if (memory_weight!=0)
		{
			*weight_grams = *weight_grams - memory_weight;
		}
		mode = get_weight_ready;
	}
}

void power_down(){
	SCK_HIGH();
	delay(90);
}

void Calibration(){
	Set_scale(1);
	int32_t data = 0;
	data = HX711_read_average(5);
	Set_offset(data);
	mode = words;
}


void Set_offset(long data){
	offset = data;
}

void Set_scale(long scale){
	SCALE = scale;
}

int32_t calibration_array(){
	for (int i = 0; i < 5; i++) {
		// запускаем цикл, в котором
		int32_t data = 0;
		data = HX711_read_average(1);
		data = data / weight_of_standard;
	    calibration_value[i] = data;   // считываем значение с тензодатчика и переводим его в граммы
	    delay(100);
	    calibration_factor += calibration_value[i]; // суммируем все значения
	  }
	calibration_factor = calibration_factor / 5;
	delay(100);
	return calibration_factor;
}

void GPIOB_init_HX711(){
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_MODER8_0;//SCK
	GPIOB->MODER &= ~GPIO_MODER_MODER9;//DOUT
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR9_0; //pull up 01
	SCK_HIGH();
	delay(100);
}
