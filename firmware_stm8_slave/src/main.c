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
    // UART1_Init(&uart_cfg);

    // // 4) I2C + LCD init
    I2C_Init(100000);
    // UART1_WriteStringAsync("I2C initialized\r\n");


    // TIM2 still used for gated TX
    // TIM2_Init(1000);

    volatile uint32_t toggleTime = millis();

    // UART → LCD line buffer
    char uart_line[17];
    uint8_t uart_index = 0;
    uint8_t c;

    enableInterrupts();
    LCD_InitBegin();   // start init FSM

    while (1)
    {

        if(LCD_InitDone()) {
            LCD_Task();        // runs runtime FSM
        } else {
            LCD_InitTask();    // runs init FSM
        }

        // if(UART1_Available()) {
        //     c = UART1_Read();
        //     UART1_WriteAsync(c); // echo back
        //     if (c == '\r' || c == '\n') {
        //         uart_line[uart_index] = '\0';

        //         // Ask LCD to update line (non-blocking)
        //         // LCD_RequestPrintLine0(uart_line);

        //         uart_index = 0;
        //     }
        //     else if (uart_index < 16) {
        //         uart_line[uart_index++] = c;
        //     }
        // }

        if(millis() - toggleTime > 500){
            GPIO_TogglePin(PD2);
            toggleTime = millis();
        }
    }
}
