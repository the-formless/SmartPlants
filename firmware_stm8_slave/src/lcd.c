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
    uint8_t d = nib | LCD_BL;
    uint8_t dummy;

    UART1_WriteString("lcd_write4: ");
    UART1_WriteHex8(nib);
    UART1_WriteString("\r\n");

    // 1) START
    UART1_WriteString(" → START\r\n");
    I2C_Start();  // this already waits for SB

    // 2) SEND ADDRESS (write)
    UART1_WriteString(" → SEND ADDR\r\n");
    I2C->DR = (LCD_ADDR << 1);

    // Wait for ADDR flag (address sent + ACK)
    while (!(I2C->SR1 & I2C_SR1_ADDR)) {
        if (I2C->SR2 & I2C_SR2_AF) {
            UART1_WriteString(" !!! NACK DURING ADDR !!!\r\n");
            I2C->SR2 &= ~I2C_SR2_AF;
            I2C_Stop();
            return;
        }
    }

    // Clear ADDR by reading SR1 then SR3 (MANDATORY)
    dummy = I2C->SR1;
    dummy = I2C->SR3;
    (void)dummy;

    // 3) Now we are in master-transmitter mode, safe to send data bytes.

    // EN = 1
    UART1_WriteString(" → EN=1 DR=");
    UART1_WriteHex8(d | LCD_EN);
    UART1_WriteString("\r\n");
    I2C_Write(d | LCD_EN);

    // EN = 0
    UART1_WriteString(" → EN=0 DR=");
    UART1_WriteHex8(d & ~LCD_EN);
    UART1_WriteString("\r\n");
    I2C_Write(d & ~LCD_EN);

    // 4) STOP
    UART1_WriteString(" → STOP\r\n\r\n");
    I2C_Stop();

    tim4_delay(1); // latch delay
}


// ------------------------------------------
// COMMAND
// ------------------------------------------
static void lcd_cmd(uint8_t cmd)
{
    UART1_WriteString("\r\nCMD ");
    UART1_WriteHex8(cmd);
    UART1_WriteString("\r\n");

    lcd_write4(cmd & 0xF0);
    lcd_write4((cmd << 4) & 0xF0);
}

// ------------------------------------------
// DATA
// ------------------------------------------
static void lcd_data(uint8_t data)
{
    UART1_WriteString("\r\nDATA ");
    UART1_WriteHex8(data);
    UART1_WriteString("\r\n");

    lcd_write4((data & 0xF0) | LCD_RS);
    lcd_write4(((data << 4) & 0xF0) | LCD_RS);
}

// ------------------------------------------
// INIT
// ------------------------------------------
void LCD_Init()
{
    tim4_delay(50);

    UART1_WriteString("Init: 0x30 #1\r\n");
    lcd_write4(0x30);
    tim4_delay(5);

    UART1_WriteString("Init: 0x30 #2\r\n");
    lcd_write4(0x30);
    tim4_delay(1);

    UART1_WriteString("Init: 0x30 #3\r\n");
    lcd_write4(0x30);
    tim4_delay(1);

    UART1_WriteString("Init: 0x20 (4-bit)\r\n");
    lcd_write4(0x20);
    tim4_delay(1);

    UART1_WriteString("Init: 0x28\r\n");
    lcd_cmd(0x28);

    UART1_WriteString("Init: 0x0C\r\n");
    lcd_cmd(0x0C);

    UART1_WriteString("Init: 0x06\r\n");
    lcd_cmd(0x06);

    UART1_WriteString("Init: 0x01\r\n");
    lcd_cmd(0x01);
    tim4_delay(2);
}


void LCD_WriteString(char *s)
{
    UART1_WriteString("LCD_WriteString: ");
    UART1_WriteString(s);
    UART1_WriteString("\r\n");

    while (*s)
        lcd_data(*s++);
}
