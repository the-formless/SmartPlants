#include "tim2.h"

void TIM2_Init(uint8_t period_us) {

    //Prescaler: 16 -> 16MHz / 16 = 1MHz (1us per tick)
    TIM2_PSCR = 4; //Set prescaler to 4 (2^4 = 16)

    //Set Auto-Reload Register
    TIM2_ARRH = (period_us >> 8) & 0xFF; //high byte
    TIM2_ARRL = (period_us & 0xFF);        //low byte

    //Enable Update Interrupt
    TIM2_IER |= 0x01; //UIE = 1

    //Enable Counter
    TIM2_CR1 |= 0x01; //CEN = 1
}