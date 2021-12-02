#ifndef LIBRARY_H_
#define LIBRARY_H_

#include "global.h"

void Draw_Simbol(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, uint8_t ascii,  uint8_t size);
void Draw_String(uint16_t x, uint16_t y, uint16_t color,uint16_t phone,char *string,  uint8_t size);
void Draw_Simbol_count(uint16_t x, uint16_t y, uint16_t color, uint16_t phone,  uint8_t size, uint8_t ascii);

#endif /* LIBRARY_H_ */
