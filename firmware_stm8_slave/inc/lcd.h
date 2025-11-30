#ifndef STM8_LCD_H
#define STM8_LCD_H

#include <stdint.h>
//uncomment this section after i2c state machine implementation
// void LCD_Init(void);
// void LCD_Clear(void);
// void LCD_WriteChar(char c);
// void LCD_WriteString(const char *s);
// void LCD_SetCursor(uint8_t row, uint8_t col);

// static void lcd_data(uint8_t data);

// //non blocking state machine api
// void LCD_Task(void); //called from main loop

// //Request: print a line ( 16 chars) at row 0
// void LCD_RequestPrintLine0(const char *test);

// //helper to know if Llcd is busy
// uint8_t LCD_IsBusy(void);

//delete this after i2c state machine immplementation

static inline void LCD_Init(void);
static inline void LCD_Clear(void);
static inline void LCD_WriteChar(char c);
static inline void LCD_WriteString(const char *s);
static inline void LCD_SetCursor(uint8_t row, uint8_t col);

static inline void lcd_data(uint8_t data);

//non blocking state machine api
static inline void LCD_Task(void); //called from main loop

//Request: print a line ( 16 chars) at row 0
static inline void LCD_RequestPrintLine0(const char *test);

//helper to know if Llcd is busy
static inline uint8_t LCD_IsBusy(void);

#endif