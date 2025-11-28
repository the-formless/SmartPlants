#ifndef LCD_H
#define LCD_H
#include <stdint.h>

void LCD_Init(void);
void LCD_Clear(void);
void LCD_WriteChar(char c);
void LCD_WriteString(char *s);
void LCD_SetCursor(uint8_t row, uint8_t col);

#endif