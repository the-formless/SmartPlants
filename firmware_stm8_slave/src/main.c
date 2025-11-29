#include "gpio.h"
#include "clock.h"
#include "tim4.h"
#include "uart.h"
#include "tim2.h"
#include "i2c.h"
#include "lcd.h"

static inline void enableInterrupts(void)
{
    __asm__("rim"); //set I bit
}

void main(void){
    //Initialize System Clock
    clock_init();
    //Initialize Timer4 for delays
    // enabling tim4 produces a flickering(led) out on TIM2 pins PA3 and PC5
    tim4_init();

    GPIO_InitTypeDef led_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_NOPULL,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_SPEED_SLOW
    };

    //initialize Pins
    GPIO_InitPin(PD2, &led_config); //LED Pin

    UART_InitTypeDef uart_cfg = {
        .baudrate = 9600,
        .parity = UART_PARITY_NONE,
        .stopBits = UART_STOPBITS_1
    };
    UART1_Init(&uart_cfg);

    TIM2_Init(1000); //send 1 byte every 1000us if available(1ms)  1000bytes/sec pacing


    UART1_WriteString("STM8 UART Gated TX Ready\r\n");

    I2C_Init(100000);

    UART1_WriteString("SR3=");
    UART1_WriteHex8(I2C->SR3);
    UART1_WriteString("\r\n");


    UART1_WriteString("Testing START...\r\n");

    I2C_Start();

    UART1_WriteString("START OK\r\n");

    UART1_WriteString("Testing LCD ACK...\r\n");

    I2C_Stop();
    tim4_delay(1);
    UART1_WriteString("SR3=");
    UART1_WriteHex8(I2C->SR3);
    UART1_WriteString("\r\n");

    I2C_Start();
    UART1_WriteString("START OK\r\n");

    // send address WRITE (0x27 << 1 = 0x4E)
    I2C->DR = (0x27 << 1);

    tim4_delay(1);

    // Check NACK
    if (I2C->SR2 & I2C_SR2_AF) {
        UART1_WriteString("LCD NACKed!\r\n");
        I2C->SR2 &= ~I2C_SR2_AF; // clear AF
    } else {
        UART1_WriteString("LCD ACKed!\r\n");
    }

    // Clear everything
    I2C_Stop();
    tim4_delay(1);
    
    
    LCD_Init();
    // LCD_Clear();
    
    // I2C_Scan();
    // LCD_SetCursor(0, 0);
    // LCD_WriteString("Hello!");

    // LCD_SetCursor(1, 0);
    // LCD_WriteString("STM8 OK");

    LCD_WriteString("Hello STM8!");
    LCD_WriteString("NIRAKAAR");
    /**
      Side NOTES:
        & creates a pointer (from a normal variable)
        * uses a pointer (access the thing the pointer points to)
     */

    enableInterrupts();
     while(1) {

        if(UART1_Available()) {
            uint8_t c = UART1_Read();
            UART1_WriteString(&c); //blocking echo back
            // GPIO_TogglePin(PD2); //toggle LED on RX
        }

        tim4_delay(3000);
        GPIO_TogglePin(PD2);
        // GPIO_WritePin(PB4, 1);
        // GPIO_WritePin(PB5, 1);
        tim4_delay(3000);
        // GPIO_WritePin(PB4, 0);
        // GPIO_WritePin(PB5, 0);
        GPIO_TogglePin(PD2);
     }
}