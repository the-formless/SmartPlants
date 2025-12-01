#ifndef STM8_LCD_H
#define STM8_LCD_H

#include <stdint.h>


#define LCD_ADDR 0x27

#define LCD_RS 0x01
#define LCD_RW 0x02
#define LCD_EN 0x04
#define LCD_BL 0x08

typedef enum {
    LCD_STATE_IDLE = 0,
    LCD_STATE_CLEAR,
    LCD_STATE_CLEAR_WAIT,
    LCD_STATE_SET_DDRAM,
    LCD_STATE_WRITE_BYTES,
    LCD_STATE_DONE
} LCD_State;

typedef enum {
    LCD_INIT_IDLE = 0,
    LCD_INIT_DELAY_50MS,

    LCD_INIT_CMD_30_1,
    LCD_INIT_WAIT_5MS,
    LCD_INIT_CMD_30_2,
    LCD_INIT_WAIT_1MS_A,
    LCD_INIT_CMD_30_3,
    LCD_INIT_WAIT_1MS_B,
    LCD_INIT_CMD_20,
    LCD_INIT_WAIT_1MS_C,

    LCD_INIT_FUNCSET,
    LCD_INIT_DISPLAYON,
    LCD_INIT_ENTRYMODE,
    LCD_INIT_CLEAR,
    LCD_INIT_FINAL_WAIT,

    LCD_INIT_DONE
} LCD_InitState;

//uncomment this section after i2c state machine implementation
// void LCD_Init(void);
// void LCD_Clear(void);
// void LCD_WriteChar(char c);
// void LCD_WriteString(const char *s);
// void LCD_SetCursor(uint8_t row, uint8_t col);

// static void lcd_data(uint8_t data);

// Public API
void LCD_InitBegin(void);        // start non-blocking init
uint8_t LCD_InitDone(void);      // 1 = fully initialized
void LCD_InitTask(void);         // run init FSM

//non blocking state machine api
void LCD_Task(void); //called from main loop

//Request: print a line ( 16 chars) at row 0
void LCD_RequestPrintLine0(const char *text);
void LCD_RequestPrintLine1(const char *text);

//helper to know if Llcd is busy
uint8_t LCD_IsBusy(void);

#endif