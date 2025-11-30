#include "gpio.h"
#include "clock.h"
#include "tim4.h"
#include "uart.h"
#include "tim2.h"
#include "i2c.h"
#include "lcd.h"

static inline void enableInterrupts(void)
{
    __asm__("rim"); // set I bit
}

static inline void disableInterrupts(void)
{
    __asm__("sim");   // Set Interrupt Mask (disable all IRQs)
}

void main(void)
{
    // 1) Core init
    clock_init();
    tim4_init();

    // 2) LED for RX debug
    GPIO_InitTypeDef led_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_NOPULL,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_SPEED_SLOW
    };
    GPIO_InitPin(PD2, &led_config); // LED on PD2

    // 3) UART init
    UART_InitTypeDef uart_cfg = {
        .baudrate = 9600,
        .parity = UART_PARITY_NONE,
        .stopBits = UART_STOPBITS_1
    };
    UART1_Init(&uart_cfg);

    // TIM2 still used for gated TX
    // TIM2_Init(1000);

    
    // 4) I2C + LCD init
    I2C_Init(100000);
    LCD_Init();
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_WriteString("Wotking. UART working still?");
    
    // 5) Enable global interrupts (for UART RX)
    enableInterrupts();
    
    UART1_WriteString("STM8 UART + LCD Ready\r\n");
    volatile uint8_t c;

    while (1)
    {
        if (UART1_Available())
        {
            c = UART1_Read();

            // Debug: echo back to PC and toggle LED so we SEE activity
            GPIO_TogglePin(PD2);
            UART1_Write(c);
            // // enableInterrupts();
        }
    }
}
