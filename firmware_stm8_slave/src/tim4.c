#include "tim4.h"

volatile uint32_t g_ticks = 0;

void tim4_init(void) {
    TIM4_PSCR = 0x07; // Prescaler: 128 -> 16MHz / 128 = 125kHz (8us per tick)
    TIM4_ARR = 125; //125 ticks x 8us ~ 1ms
    TIM4_CR1 |= 0x01; // Enable Timer
    TIM4_IER |= 0x01; //update interrupt enable
}


void tim4_delay(uint16_t ms) {
    while(ms--) {
        TIM4_CNTR = 0;
        TIM4_SR &= ~0x01; // Clear update flag
        while(!(TIM4_SR & 0x01)); // Wait for update flag / overflow = 1 ms
    }
}