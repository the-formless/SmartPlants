#include "lcd.h"
#include "i2c.h"
#include "tim4.h"

#define LCD_ADDR 0x27  // PCF8574T

#define LCD_RS 0x01
#define LCD_RW 0x02
#define LCD_EN 0x04
#define LCD_BL 0x08   // backlight

static void lcd_write4(uint8_t data)
{
    uint8_t d = data | LCD_BL;

    // 1. START + address phase
    I2C_BeginWrite(LCD_ADDR);

    // 2. Pulse the enable line
    I2C_Write(d | LCD_EN);    // EN = 1
    I2C_Write(d & ~LCD_EN);   // EN = 0

    // 3. STOP
    I2C_Stop();

    tim4_delay(1);  // small latch delay
}

static void lcd_writecmd(uint8_t cmd)
{
    lcd_write4(cmd & 0xF0);
    lcd_write4((cmd << 4) & 0xF0);
}

static void lcd_writedata(uint8_t data)
{
    lcd_write4((data & 0xF0) | LCD_RS);
    lcd_write4(((data << 4) & 0xF0) | LCD_RS);
}

void LCD_Init()
{
    tim4_delay(50);
    lcd_writecmd(0x33);
    lcd_writecmd(0x32);
    lcd_writecmd(0x28);
    lcd_writecmd(0x0C);
    lcd_writecmd(0x06);
    lcd_writecmd(0x01);
    tim4_delay(2);
}

void LCD_WriteString(char *s)
{
    while (*s)
        lcd_writedata(*s++);
}
