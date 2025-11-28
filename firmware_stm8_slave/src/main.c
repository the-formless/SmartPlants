#include "gpio.h"
#include "clock.h"
#include "tim4.h"
#include "uart.h"

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

    enableInterrupts();

    UART1_WriteString("STM8 UART Ready\r\n");




    /**
      Side NOTES:
        & creates a pointer (from a normal variable)
        * uses a pointer (access the thing the pointer points to)
     */

     while(1) {

        // if(UART1_Available()) {
        //     // uint8_t c = UART1_Read();
        //     // UART1_WriteString(c + "-echo"); //echo back
        //     char buf[32];
        //     UART1_ReadString(buf, sizeof(buf));
        //     UART1_WriteString("Received: ");
        //     UART1_WriteString(buf);
        // }

        if (uart1_rx_flag) {
            uint8_t c = uart1_rx_byte;
            uart1_rx_flag = 0;

            // Echo back to Arduino
            UART1_Write(c);

            // Toggle PD2 LED
            GPIO_TogglePin(PD2);
        }

        // //Toggle all LEDs
        // // GPIO_WritePin(PD2, 1);
        // GPIO_WritePin(PA1, 1);
        // GPIO_WritePin(PA2, 1);
        // GPIO_WritePin(PA3, 1);
        // GPIO_WritePin(PB4, 1);
        // GPIO_WritePin(PB5, 1);
        // GPIO_WritePin(PC3, 1);
        // GPIO_WritePin(PC4, 1);
        // GPIO_WritePin(PC5, 1);
        // GPIO_WritePin(PC6, 1);
        // GPIO_WritePin(PC7, 1);
        // GPIO_WritePin(PD3, 1);
        // GPIO_WritePin(PD4, 1);
        // // GPIO_WritePin(PD5, 1);
        // // GPIO_WritePin(PD6, 1);

        // tim4_delay(500);

        // // GPIO_WritePin(PD2, 0);
        // GPIO_WritePin(PA1, 0);
        // GPIO_WritePin(PA2, 0);
        // GPIO_WritePin(PA3, 0);
        // GPIO_WritePin(PB4, 0);
        // GPIO_WritePin(PB5, 0);
        // GPIO_WritePin(PC3, 0);
        // GPIO_WritePin(PC4, 0);
        // GPIO_WritePin(PC5, 0);
        // GPIO_WritePin(PC6, 0);
        // GPIO_WritePin(PC7, 0);
        // GPIO_WritePin(PD3, 0);
        // GPIO_WritePin(PD4, 0);
        // // GPIO_WritePin(PD5, 0);
        // // GPIO_WritePin(PD6, 0);

        // tim4_delay(500);
     }
}