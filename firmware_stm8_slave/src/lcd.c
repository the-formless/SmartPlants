#include "lcd.h"
#include "i2c.h"
#include "tim4.h"
#include <string.h>


static volatile LCD_State lcd_state = LCD_STATE_IDLE;
static char lcd_line_buf[17];
static uint8_t lcd_len = 0;
static uint8_t lcd_pos = 0;
static uint32_t lcd_next_time = 0;

static uint8_t lcd_cmd_buf[8];
static uint8_t lcd_data_buf[8];

static volatile LCD_InitState lcd_init_state = LCD_INIT_IDLE;
static uint32_t lcd_init_next_time = 0;

// Build the I2C bytes for a single LCD command (PCF8574 4-bit mode)
static uint8_t build_cmd_bytes(uint8_t cmd, uint8_t *out)
{
    uint8_t high = (cmd & 0xF0);           // high nibble
    uint8_t low  = (cmd << 4) & 0xF0;      // low nibble
    uint8_t i = 0;

    // High nibble + EN pulse
    out[i++] = high | LCD_BL | LCD_EN;     // EN = 1
    out[i++] = high | LCD_BL;              // EN = 0

    // Low nibble + EN pulse
    out[i++] = low | LCD_BL | LCD_EN;      // EN = 1
    out[i++] = low | LCD_BL;               // EN = 0

    return i;  // always 4 bytes
}
// Build PCF8574 bytes for a DATA byte
// ------------------------------------------
static uint8_t build_data_bytes(uint8_t data, uint8_t *out)
{
    uint8_t high = (data & 0xF0);          // high nibble
    uint8_t low  = (data << 4) & 0xF0;     // low nibble
    uint8_t i = 0;

    // High nibble + EN pulse (RS = 1)
    out[i++] = high | LCD_BL | LCD_RS | LCD_EN;
    out[i++] = high | LCD_BL | LCD_RS;

    // Low nibble + EN pulse (RS = 1)
    out[i++] = low | LCD_BL | LCD_RS | LCD_EN;
    out[i++] = low | LCD_BL | LCD_RS;

    return i;
}

void LCD_RequestPrintLine0(const char *text) 
{
    uint8_t i = 0;

    //copy upto 16 chars, pad with spaces
    while(i < 16 && text[i] != '\0') {
        lcd_line_buf[i] = text[i];
        i++;
    }
    while(i<16) {
        lcd_line_buf[i++] = ' ';
    }
    lcd_line_buf[16] = '\0';

    lcd_len = 16;
    lcd_pos = 0;

    //Start new transactioon
    lcd_state = LCD_STATE_CLEAR;
}


uint8_t LCD_IsBusy(void)
{
    return (lcd_state != LCD_STATE_IDLE);
}


void LCD_Task(void)
{
    if(!LCD_InitDone())
        return;

    uint32_t now = millis();

    switch (lcd_state)
    {
        case LCD_STATE_IDLE:
            // nothing
            break;

        case LCD_STATE_CLEAR:
            if (I2C_IsIdle()) {
                uint8_t len = build_cmd_bytes(0x01, lcd_cmd_buf);  // clear display
                if (I2C_BeginWrite(LCD_ADDR, lcd_cmd_buf, len) == 0) {
                    // Wait >1.52ms for clear/home
                    lcd_next_time = now + 2;
                    lcd_state = LCD_STATE_CLEAR_WAIT;
                }
            }
            break;

        case LCD_STATE_CLEAR_WAIT:
            if ((int32_t)(now - lcd_next_time) >= 0) {
                lcd_state = LCD_STATE_SET_DDRAM;
            }
            break;

        case LCD_STATE_SET_DDRAM:
            if (I2C_IsIdle()) {
                // DDRAM address line 0, col 0 = 0x80
                uint8_t len = build_cmd_bytes(0x80, lcd_cmd_buf);
                if (I2C_BeginWrite(LCD_ADDR, lcd_cmd_buf, len) == 0) {
                    lcd_pos = 0;
                    lcd_state = LCD_STATE_WRITE_BYTES;
                }
            }
            break;

        case LCD_STATE_WRITE_BYTES:
            if (I2C_IsIdle()) {
                if (lcd_pos < lcd_len) {
                    uint8_t len = build_data_bytes(lcd_line_buf[lcd_pos++], lcd_data_buf);
                    if (I2C_BeginWrite(LCD_ADDR, lcd_data_buf, len) == 0) {
                        // Space out characters by ~1ms (overkill, but safe)
                        lcd_next_time = now + 1;
                    }
                } else {
                    lcd_state = LCD_STATE_DONE;
                }
            } else {
                // still sending last char
            }

            // enforce inter-char delay
            if ((int32_t)(now - lcd_next_time) < 0) {
                // not yet time for next character, stay here
            }
            break;

        case LCD_STATE_DONE:
            lcd_state = LCD_STATE_IDLE;
            break;
    }
}

void LCD_InitBegin(void)
{
    lcd_init_state = LCD_INIT_DELAY_50MS;
    lcd_init_next_time = millis() + 50;
}

uint8_t LCD_InitDone(void) 
{
    return (lcd_init_state == LCD_INIT_DONE);
}


void LCD_InitTask(void)
{
    uint32_t now = millis();

    switch (lcd_init_state)
    {
        case LCD_INIT_IDLE:
            break;

        case LCD_INIT_DELAY_50MS:
            if ((int32_t)(lcd_init_next_time - now) >= 0) {
                lcd_init_state = LCD_INIT_CMD_30_1;
            }
            break;

        case LCD_INIT_CMD_30_1:
            if (I2C_IsIdle()) {
                uint8_t len = build_cmd_bytes(0x30, lcd_cmd_buf);
                if (I2C_BeginWrite(LCD_ADDR, lcd_cmd_buf, len) == 0) {
                    lcd_init_next_time = now + 5;
                    lcd_init_state = LCD_INIT_WAIT_5MS;
                }
            }
            break;

        case LCD_INIT_WAIT_5MS:
            if ((int32_t)(lcd_init_next_time - now) >= 0)
                lcd_init_state = LCD_INIT_CMD_30_2;
            break;

        case LCD_INIT_CMD_30_2:
            if (I2C_IsIdle()) {
                uint8_t len = build_cmd_bytes(0x30, lcd_cmd_buf);
                if (I2C_BeginWrite(LCD_ADDR, lcd_cmd_buf, len) == 0) {
                    lcd_init_next_time = now + 1;
                    lcd_init_state = LCD_INIT_WAIT_1MS_A;
                }
            }
            break;

        case LCD_INIT_WAIT_1MS_A:
            if ((int32_t)(lcd_init_next_time - now) >= 0)
                lcd_init_state = LCD_INIT_CMD_30_3;
            break;

        case LCD_INIT_CMD_30_3:
            if (I2C_IsIdle()) {
                uint8_t len = build_cmd_bytes(0x30, lcd_cmd_buf);
                if (I2C_BeginWrite(LCD_ADDR, lcd_cmd_buf, len) == 0) {
                    lcd_init_next_time = now + 1;
                    lcd_init_state = LCD_INIT_WAIT_1MS_B;
                }
            }
            break;

        case LCD_INIT_WAIT_1MS_B:
            if ((int32_t)(lcd_init_next_time - now) >= 0)
                lcd_init_state = LCD_INIT_CMD_20;
            break;

        case LCD_INIT_CMD_20:
            if (I2C_IsIdle()) {
                uint8_t len = build_cmd_bytes(0x20, lcd_cmd_buf);
                if (I2C_BeginWrite(LCD_ADDR, lcd_cmd_buf, len) == 0) {
                    lcd_init_next_time = now + 1;
                    lcd_init_state = LCD_INIT_WAIT_1MS_C;
                }
            }
            break;

        case LCD_INIT_WAIT_1MS_C:
            if ((int32_t)(lcd_init_next_time - now) >= 0)
                lcd_init_state = LCD_INIT_FUNCSET;
            break;

        case LCD_INIT_FUNCSET:
            if (I2C_IsIdle()) {
                uint8_t len = build_cmd_bytes(0x28, lcd_cmd_buf); // 4-bit, 2 lines
                if (I2C_BeginWrite(LCD_ADDR, lcd_cmd_buf, len) == 0) {
                    lcd_init_state = LCD_INIT_DISPLAYON;
                }
            }
            break;

        case LCD_INIT_DISPLAYON:
            if (I2C_IsIdle()) {
                uint8_t len = build_cmd_bytes(0x0C, lcd_cmd_buf); // display on, cursor off
                if (I2C_BeginWrite(LCD_ADDR, lcd_cmd_buf, len) == 0) {
                    lcd_init_state = LCD_INIT_ENTRYMODE;
                }
            }
            break;

        case LCD_INIT_ENTRYMODE:
            if (I2C_IsIdle()) {
                uint8_t len = build_cmd_bytes(0x06, lcd_cmd_buf); // entry mode inc, no shift
                if (I2C_BeginWrite(LCD_ADDR, lcd_cmd_buf, len) == 0) {
                    lcd_init_state = LCD_INIT_CLEAR;
                }
            }
            break;

        case LCD_INIT_CLEAR:
            if (I2C_IsIdle()) {
                uint8_t len = build_cmd_bytes(0x01, lcd_cmd_buf); // clear
                if (I2C_BeginWrite(LCD_ADDR, lcd_cmd_buf, len) == 0) {
                    lcd_init_next_time = now + 2;  // >1.52ms
                    lcd_init_state = LCD_INIT_FINAL_WAIT;
                }
            }
            break;

        case LCD_INIT_FINAL_WAIT:
            if ((int32_t)(lcd_init_next_time - now) >= 0)
                lcd_init_state = LCD_INIT_DONE;
            break;

        case LCD_INIT_DONE:
            break;
    }
}
