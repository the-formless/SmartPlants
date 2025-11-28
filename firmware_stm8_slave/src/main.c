#include "gpio.h"
#include "clock.h"
#include "tim4.h"
#include "uart.h"
#include "tim2.h"

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

    enableInterrupts();

    UART1_WriteString("STM8 UART Gated TX Ready\r\n");




    /**
      Side NOTES:
        & creates a pointer (from a normal variable)
        * uses a pointer (access the thing the pointer points to)
     */

     while(1) {

        if(UART1_Available()) {
            uint8_t c = UART1_Read();
            UART1_WriteAsync(c); //non blocking echo back
            GPIO_TogglePin(PD2); //toggle LED on RX
        }
     }
}