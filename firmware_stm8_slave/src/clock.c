#include "clock.h"

void clock_init(void) {
    CLK_ICKR |= 0x01; //Enable HSI
    while(!(CLK_ICKR & 0x02)); //wait for HSI ready
    
    //No division -> full 16MHz internal clock
    CLK_CKDIVR = 0x00;
}

void uart_clock_init(void) {
    //Enable UART1 clock
    CLK_PCKENR1 |= (1 << 3); //UART1 clock enable
}