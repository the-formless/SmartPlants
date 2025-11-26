#include <stdint.h>
#include "../inc/hal_gpio.h"
#include "../inc/led.h"
// Simple delay function (approximate for 16MHz STM8)
void delay_ms(uint16_t ms) {
    volatile uint16_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 1600; j++) {  // Approximate 1ms at 16MHz
            __asm__("nop");
        }
    }
}
int main(void) {
    // Initialize peripherals
    hal_gpio_init();
    led_init();
    
    // Main loop - blink LED every 1000ms
    while (1) {
        led_toggle();
        // led_off();
        delay_ms(1000);
    }
    
    return 0;
}