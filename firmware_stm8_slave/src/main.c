#include <stdint.h>
#include "../inc/hal_gpio.h"
// Simple delay function (approximate for 16MHz STM8)
void delay_ms(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++)
        for (uint16_t j = 0; j < 1600; j++)
            __asm__("nop");
}

int main(void)
{
    // LED = PC4
    hal_gpio_init(GPIOC, 4, GPIO_OUTPUT);

    while (1)
    {
        hal_gpio_toggle(GPIOC, 4);
        delay_ms(1000);
    }
}