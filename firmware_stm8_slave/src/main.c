#include "gpio.h"
#include "clock.h"
#include "tim4.h"
#include "uart.h"
#include "tim2.h"
#include "ws2812.h"

static inline void enableInterrupts(void)
{
    __asm__("rim"); //set I bit
}

char cmd_buffer[32];
uint8_t cmd_index = 0;
uint8_t command_ready = 0;

void process_uart_input(void) {
    uint8_t c;
    while (UART1_ReadByteNonBlocking(&c)) {
        if(c== '\r' || c == '\n') {
            if(cmd_index > 0) {
                cmd_buffer[cmd_index] = 0; //null terminate
                command_ready = 1;
            }
            cmd_index = 0; //reset for next command
        }
        else if (cmd_index < sizeof(cmd_buffer) - 1) {
            cmd_buffer[cmd_index++] = c;
        }
    }
}


void main(void){
    //Initialize System Clock
    clock_init();
    

    UART_InitTypeDef uart_cfg = {
        .baudrate = 9600,
        .parity = UART_PARITY_NONE,
        .stopBits = UART_STOPBITS_1
    };

    UART1_Init(&uart_cfg);

    WS2812_init();

    // TIM2_Init(1000); //send 1 byte every 1000us if available(1ms)  1000bytes/sec pacing

    enableInterrupts();

    UART1_WriteString("WS2812 command mode ready\r\n");




    /**
      Side NOTES:
        & creates a pointer (from a normal variable)
        * uses a pointer (access the thing the pointer points to)
     */

     while(1) {

        //Step1: keep parsing UART input
        process_uart_input();
        //Step2: if command ready, process it
        if(command_ready) {
            UART1_WriteStringAsync("CMD: ");
            UART1_WriteStringAsync(cmd_buffer);
            UART1_WriteStringAsync("\r\n");

            apply_color(cmd_buffer);
            command_ready = 0;
        }
     }
}