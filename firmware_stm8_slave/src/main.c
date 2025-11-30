#include "gpio.h"
#include "clock.h"
#include "tim4.h"
#include "uart.h"
#include "tim2.h"
#include "i2c.h"
// #include "lcd.h"

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
    TIM2_Init(1000);

    UART1_WriteString("STM8 UART + LCD Ready\r\n");

    UART1_WriteString("SDA before init = \r\n");
    UART1_WriteHex8(GPIO_ReadPin(PB5));
    UART1_WriteString("\r\n");

    UART1_WriteString("SCL before init = \r\n");
    UART1_WriteHex8(GPIO_ReadPin(PB4));
    UART1_WriteString("\r\n");
    // // 4) I2C + LCD init
    I2C_Init(100000);
    UART1_WriteString("I2c init complete\r\n");
    // LCD_Init();
    // LCD_RequestPrintLine0("Ready...?....");
    UART1_WriteString("SDA after init = \r\n");
    UART1_WriteHex8(GPIO_ReadPin(PB5));
    UART1_WriteString("\r\n");

    UART1_WriteString("SCL after init = \r\n");
    UART1_WriteHex8(GPIO_ReadPin(PB4));
    UART1_WriteString("\r\n");

    // 5) Enable global interrupts (for UART RX)
    enableInterrupts();

   uint32_t last_toggle = 0;
    uint8_t pattern = 0;
    uint8_t buf[1];

    UART1_WriteString("Starting main loop\r\n");
    UART1_WriteString("MILLIS = \r\n");
    UART1_WriteHex8((uint8_t)millis());

    if (I2C_BeginWrite(0x27, buf, 1)) {
        UART1_WriteString("BeginWrite OK\r\n");
    } else {
        UART1_WriteString("BeginWrite BUSY\r\n");
    }

    while (1)
    {
        uint32_t now = millis();

        // drive I2C state machine
        I2C_Task();

        // every 500 ms, schedule a new I2C write if bus is free
        if ((int32_t)(now - last_toggle) >= 0) {
            if (!I2C_IsBusy()) {
                pattern ^= 0x08;    // toggle bit 3 (backlight on PCF8574)
                buf[0] = pattern;

                if (I2C_BeginWrite(0x27, buf, 1)) {
                    UART1_WriteString("I2C tx start: 0x\r\n");
                    UART1_WriteHex(buf[0]);
                    UART1_WriteString("\r\n");
                    GPIO_TogglePin(PD2);
                    last_toggle = now + 500;  // next toggle in 500 ms
                } else {
                    UART1_WriteString("I2C busy, cannot start\r\n");
                }
            }
        }
    }
}
