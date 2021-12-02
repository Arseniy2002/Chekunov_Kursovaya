#include "tft.h"

static uint32_t count_pixels;
extern uint8_t mode;


uint16_t MAX_Y=240;
uint16_t MAX_X=320;

void TFT_init(){
	spi1_master_init(8); // настраиваем spi на 8 бит данных
	LED_on();
	delay(100);

	// сбрасываем экран в ручную
	RESET_ACTIVE();
	delay(10);
	RESET_IDLE();
	delay(10000);

	tft_display_normal_mode();
	tft_pixel_format();

	tft_sleep_out(); // пробуждаем из спящего режима
	delay(120);
	tft_display_on();
	delay(10000); // задержка, чтобы экран успел включиться

	tft_colorise(WHITE); // заливаем белым цветом
	delay(120);
}



void delay(uint32_t tick){
	tick*=(SystemCoreClock / 1000000); // делим, чтобы сделать 1 тик 1 мкс
	while (tick--);
}

void tft_write_cmd(uint8_t cmd, uint8_t *data, uint8_t size) {
	if (size == 0) {
		return;
	}
	// присылаем команду
	DC_COMMAND();
	spi1_SendDataDMA_1Byte(&cmd, 1);
	delay(10);
	// присылаем данные
	DC_DATA();
	spi1_SendDataDMA_1Byte(&data[0], size);
	delay(10);
}

void tft_sleep_out() {
	DC_COMMAND();
	uint8_t data=Sleep_out;
	spi1_SendDataDMA_1Byte(&data, 1);
	delay(10000);
}

void tft_display_off() {
	tft_write_cmd(Display_OFF, 0, 0);
}

void tft_display_on() {
	uint8_t data=ROTATION;
	tft_write_cmd(Display_ON, &data, 1);
}

void tft_display_normal_mode() {
	DC_COMMAND();
	uint8_t data=Normal_Display_mode;
	spi1_SendDataDMA_1Byte(&data, 1);
}

void tft_pixel_format() {
	uint8_t data = Pixel_16_bits;
	tft_write_cmd(Pixel_Format_Set, &data, 1);
}

// функция для установки начала и конца для столбцов
void tft_set_column(uint16_t col_start, uint16_t col_end) {
	uint8_t data_column[4] = {	(uint8_t)((col_start >> 8) & 0xFF),
								(uint8_t)(col_start & 0xFF),
								(uint8_t)((col_end >> 8) & 0xFF),
								(uint8_t)(col_end & 0xFF)};

	tft_write_cmd(Column_Address_Set, &data_column[0], 4);
}

// функция для установки начала и конца для строк
void tft_set_row(uint16_t row_start, uint16_t row_end) {
	uint8_t data_row[4] = {	(uint8_t)(row_start >> 8),
							(uint8_t)(row_start & 0xFF),
							(uint8_t)(row_end >> 8),
							(uint8_t)(row_end & 0xFF)};

	tft_write_cmd(Page_Adress_Set, &data_row[0], 4);
}

// записываем в дисплей данные
void tft_ram_write(uint8_t *data, uint8_t size) {
	tft_write_cmd(Memory_Write, &data[0], size);
}


// устанавливаем область, которую необходимо залить
void tft_set_region(uint16_t row_start, uint16_t row_end, uint16_t col_start, uint16_t col_end) { // выбор области
	count_pixels = (row_end - row_start + 1) * (col_end - col_start + 1);
	uint8_t data = 0x00;
	tft_write_cmd(Memory_Access_Control, &data, 1);

	tft_set_column(col_start, col_end);
	tft_set_row(row_start, row_end);
	tft_ram_write(&data, 1);
}

// заливаем весь экран цветом
void tft_colorise(uint16_t color) {
	uint8_t data = 0x00;
	tft_ram_write(&data, 1);
	tft_set_region(0,MAX_X,0,MAX_Y);
	uint8_t color_array[2];
	color_array[0] = (uint8_t)((color >> 8) & 0xFF) ;
	color_array[1] = (uint8_t)(color & 0xFF);
	DC_DATA();
	for(uint32_t i = 0; i < count_pixels + 1; i++){
		spi1_SendDataDMA_1Byte(&color_array[0],2);
	}
}

// заливаем какую то область цветом
void tft_color_XY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color) {
	if (x1>MAX_X || y1>MAX_Y || x2>MAX_X || y2>MAX_Y) return;
	if (x1>x2) swap(&x1,&x2);
	if (y1>y2) swap(&y1,&y2);
	tft_set_region(x1,x2,y1,y2);
	uint8_t color_array[2];
	color_array[0] = (uint8_t)((color >> 8) & 0xFF) ;
	color_array[1] = (uint8_t)(color & 0xFF);
	DC_DATA();
	for(uint32_t i = 0; i < count_pixels + 1; i++){
		spi1_SendDataDMA_1Byte(&color_array[0],2);
	}
}

// функция для отрисовки 1 пикселя
void tft_draw_pixel(uint16_t x, uint16_t y, uint16_t color){
	RESET_CS();
	tft_set_region(x, x+1, y, y+1);
	DC_DATA();
	uint8_t color_array[2];
	color_array[0] = (uint8_t)((color >> 8) & 0xFF) ;
	color_array[1] = (uint8_t)(color & 0xFF);
	DC_DATA();
	for(uint32_t i = 0; i < count_pixels + 1; i++){
		spi1_SendDataDMA_1Byte(&color_array[0],2);
	}
}


// рисуем 1 пиксель в масштабе 4 (для вывода больший букв и чисел)
void tft_draw_pixel_4_size(uint16_t x, uint16_t y, uint16_t color){
	RESET_CS();
	tft_set_region(x, x+4, y, y+4);
	DC_DATA();
	uint8_t color_array[2];
	color_array[0] = (uint8_t)((color >> 8) & 0xFF) ;
	color_array[1] = (uint8_t)(color & 0xFF);
	DC_DATA();
	for(uint32_t i = 0; i < count_pixels + 1; i++){
		spi1_SendDataDMA_1Byte(&color_array[0],2);
	}
}

// поменьше масштаб, для больших надписей
void tft_draw_pixel_2_size(uint16_t x, uint16_t y, uint16_t color){
	RESET_CS();
	tft_set_region(x, x+2, y, y+2);
	DC_DATA();
	uint8_t color_array[2];
	color_array[0] = (uint8_t)((color >> 8) & 0xFF) ;
	color_array[1] = (uint8_t)(color & 0xFF);
	DC_DATA();
	for(uint32_t i = 0; i < count_pixels + 1; i++){
		spi1_SendDataDMA_1Byte(&color_array[0],2);
	}
}


// меняем местами 2 переменные
void swap(uint16_t *a, uint16_t *b) {
	uint16_t t;
	t=*a;
	*a=*b;
	*b=t;
}


// пишем на экране заданную строку
void Print_string(char *string){
	tft_colorise(WHITE);
	delay(100);
	Draw_String(10, 100, BLACK, WHITE ,string, 2);
}

// пишем на экране заданный вес
void Print_weight(int32_t data_weight){
	int32_t data;
	if (data_weight>1000){ // если больше 1 кг
		char arr[] = "Weight too much";
		Draw_String(80, 100, RED, WHITE, &arr[0], 2);
		delay(100000);
		tft_color_XY(10,100,320, 200, WHITE);
		return;
	}
	else{ // если меньше 1 кг, то есть просто граммы
		data = data_weight;
		if(data<0){
			char array[7];
			data*=(-1);
			array[0] = 0x2D;
			array[1] = (data/100)%10;
			array[2] = (data/10)%10;
			array[3] = data%10;
			array[4] = 0x67;
			array[5] = 0x72;
			array[6] = 0;
			char arr[] = {array[0], array[1] +0x30, array[2] + 0x30, array[3] +0x30 ,array[4], array[5], array[6]};
			Draw_String(80, 100, BLACK, WHITE, &arr[0], 4);
			return;
		}
		char array[7];
		array[0] = (data/100)%10;
		array[1] = (data/10)%10;
		array[2] = data%10;
		array[3] = 0x20;
		array[4] = 0x67;
		array[5] = 0x72;
		array[6] = 0;
		char arr[] = {array[0] + 0x30, array[1] +0x30, array[2] + 0x30, array[3],array[4], array[5], array[6]};
		Draw_String(80, 100, BLACK, WHITE, &arr[0], 4);
		return;
	}
}

void countdown(){
	tft_colorise(WHITE);
	for(int i=1;i<10;i++){
		Draw_Simbol_count(150,100, BLACK, WHITE, 4, i+0x30);
		delay(100000);
	}
	tft_colorise(WHITE);
}



void Print(int32_t *weight){
	if(mode == words){ // если дана команда вывести на экран надпись
		char str[]="Put the reference  weight on the scale!";
		Print_string(&str[0]);
		countdown();
		mode = setSc;
		return;
	}
	if(mode == get_weight_ready){ // если вес получен с АЦП
		 int32_t data = *weight;
		 Print_weight(data);
		 mode = get_weight;
		 return;
	}
	if(mode == memory_weight_ready){ //если нажата кнопка, вывести на весы 0
		 Print_weight(0);
		 delay(300000);
		 mode = get_weight;
		 return;
	}
}
