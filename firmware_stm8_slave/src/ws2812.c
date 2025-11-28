#include "gpio.h"
#include "tim2.h"
#include "ws2812.h"
#include <string.h>

void WS2812_init(void) {
    //PD3 output
    GPIO_InitTypeDef ws2812_cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_NOPULL,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_SPEED_FAST
    };
    GPIO_InitPin(PD3, &ws2812_cfg);

    //TIM2: prescaler = 0 -> 16MHz (1 tick = 62.5ns)
    TIM2_PSCR = 0; //no prescaling

    //ARR = 20 ticks -> 20 x 62.5ns = 1.25us period
    TIM2_ARRH = 0;
    TIM2_ARRL = 19;

    //PWM mode on CH2
    //OC2M = 110 (PWM mode 1)
    TIM2_CCMR2 = 0x60;

    //Enable CH2 output
    TIM2_CCER1 |= (1 << 4); //CC2E = 1

    //enable update interrupt
    TIM2_IER |= 0x01; //UIE = 1
}

void WS2812_send(uint8_t *data, uint16_t length) {
    //Set global data pointer and bit count
    ws_data_ptr = data;
    ws_num_bits = length * 8; //length in bytes -> bits
    ws_sending = 1;

    TIM2_CCR2L = 0; //preload
    //Start TIM2
    TIM2_CR1 |= 0x01; //CEN = 1

    while(ws_sending); //wait until done
}

void apply_color(const char *cmd) {
    uint8_t data[3];

    if(strcmp(cmd, "red") == 0) {
        data[0] = 0x00;   //G
        data[1] = 0x50; //R
        data[2] = 0x00;   //B
    } else if(strcmp(cmd, "green") == 0) {
        data[0] = 0x50; //G
        data[1] = 0x00;   //R
        data[2] = 0x00;   //B
    } else if(strcmp(cmd, "blue") == 0) {
        data[0] = 0x00;   //G
        data[1] = 0x00;   //R
        data[2] = 0x50; //B
    } else {
        //default to off
        data[0] = 0x00;
        data[1] = 0x00;
        data[2] = 0x00;
    }

    WS2812_send(data, 3);
}