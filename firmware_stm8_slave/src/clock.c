#include "clock.h"

void clock_init(void) {
    CLK_ICKR |= 0x01; //Enable HSI
    while(!(CLK_ICKR & 0x02)); //wait for HSI ready
    
    //No division -> full 16MHz internal clock
    CLK_CKDIVR = 0x00;
}

void uart_clock_init(void) {
    //Enable UART1
    CLK_PCKENR1 |= (1 << 3); 
}

void i2c_clock_init(void) {
    // Check if already enabled
    uint8_t before = CLK_PCKENR1;
    
    // Enable I2C clock (bit 0)
    CLK_PCKENR1 |= (1 << 0);
    
    // Verify it stuck
    uint8_t after = CLK_PCKENR1;
    
    // Small delay for clock to stabilize
    for(volatile uint16_t i = 0; i < 1000; i++);
}
