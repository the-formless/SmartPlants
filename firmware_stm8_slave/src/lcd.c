#include "lcd.h"
#include "i2c.h"
#include "tim4.h"
#include "uart.h"

#define LCD_ADDR 0x27

#define LCD_RS 0x01
#define LCD_RW 0x02
#define LCD_EN 0x04
#define LCD_BL 0x08

// ------------------------------------------
// LOW LEVEL 4-BIT TRANSFER
// ------------------------------------------
static void lcd_write4(uint8_t nib)
{
    uint8_t d = nib | LCD_BL; // Backlight ON
    volatile uint8_t dummy;

    // ---- START ----
    I2C->CR2 |= I2C_CR2_START;
    while (!(I2C->SR1 & I2C_SR1_SB));

    // ---- SEND ADDRESS ----
    I2C->DR = (LCD_ADDR << 1); // Write mode
    while (!(I2C->SR1 & I2C_SR1_ADDR)) {
        if (I2C->SR2 & I2C_SR2_AF) {
            I2C->SR2 &= ~I2C_SR2_AF;
            I2C->CR2 |= I2C_CR2_STOP;
            return;
        }
    }

    // MUST CLEAR ADDR FLAG
    dummy = I2C->SR1;
    dummy = I2C->SR3;
    (void)dummy;

    // ---- BYTE: EN = 1 ----
    I2C->DR = d | LCD_EN;
    while (!(I2C->SR1 & I2C_SR1_TXE));

    // ---- BYTE: EN = 0 ----
    I2C->DR = d & ~LCD_EN;
    while (!(I2C->SR1 & I2C_SR1_TXE));

    // ---- STOP ----
    I2C->CR2 |= I2C_CR2_STOP;

    tim4_delay(1);
}



// ------------------------------------------
// COMMAND
// ------------------------------------------
static void lcd_cmd(uint8_t cmd)
{

    lcd_write4(cmd & 0xF0);
    lcd_write4((cmd << 4) & 0xF0);
}

// ------------------------------------------
// DATA
// ------------------------------------------
static void lcd_data(uint8_t data)
{
    lcd_write4((data & 0xF0) | LCD_RS);
    lcd_write4(((data << 4) & 0xF0) | LCD_RS);
}

// ------------------------------------------
// INIT
// ------------------------------------------
void LCD_Init(void)
{
    tim4_delay(50);

    lcd_write4(0x30);
    tim4_delay(5);

    lcd_write4(0x30);
    tim4_delay(1);

    lcd_write4(0x30);
    tim4_delay(1);

    lcd_write4(0x20);
    tim4_delay(1);

    lcd_cmd(0x28);

    lcd_cmd(0x0C);

    lcd_cmd(0x06);

    lcd_cmd(0x01);
    tim4_delay(2);
}


void LCD_WriteString(const char *s)
{

    while (*s)
        lcd_data(*s++);
}

void LCD_Clear(void)
{
    lcd_cmd(0x01);
    tim4_delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0 ? 0x00 : 0x40) + col;
    lcd_cmd(0x80 | addr);
}

