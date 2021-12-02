#ifndef TFT_H_
#define TFT_H_

#include <stdint.h>
#include "stm32f0xx.h"
#include "global.h"
#include "command.h"
#include "spi_dma.h"


#define BLACK 0x0000
#define GREEN 0x001F
#define RED 0xF800
#define BLUE 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xFFE0
#define WHITE 0xFFFF

void TFT_init();
void delay(uint32_t tick);

void tft_write_cmd(uint8_t cmd, uint8_t *data, uint8_t size);

void tft_sleep_out();	//0x11
void tft_display_normal_mode();	//0x13
void tft_display_off();	//0x28
void tft_display_on();//0x29
void tft_pixel_format();	//0x3A
void tft_RGB();

void tft_set_region(uint16_t row_start, uint16_t row_end, uint16_t col_start, uint16_t col_end);
void tft_set_column(uint16_t col_start, uint16_t col_end);	//2A
void tft_set_row(uint16_t row_start, uint16_t row_end);	//2B
void tft_ram_write(uint8_t *data, uint8_t size);	//2c
void tft_SetRotation(uint8_t r);
void tft_colorise(uint16_t color);
void tft_color_XY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void tft_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void swap(uint16_t *a, uint16_t *b);
void tft_draw_pixel_4_size(uint16_t x, uint16_t y, uint16_t color);
void tft_draw_pixel_2_size(uint16_t x, uint16_t y, uint16_t color);
void Print_string(char *string);
void Print_weight(int32_t data_weight);
void Print(int32_t *weight);

#endif /* TFT_H_ */
