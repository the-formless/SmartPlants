#include "../inc/hal_gpio.h"

void hal_gpio_init(GPIO_Port *port, uint8_t pin, GPIO_Direction direction)
{
    if (direction == GPIO_OUTPUT) {
        port->DDR |=  (1 << pin);   // Set as output
        port->CR1 |=  (1 << pin);   // Push-pull
        port->CR2 &= ~(1 << pin);   // Low speed
    } else {
        port->DDR &= ~(1 << pin);   // Input
        port->CR1 |=  (1 << pin);   // Pull-up
        port->CR2 &= ~(1 << pin);   // No interrupt
    }
}

void hal_gpio_write(GPIO_Port *port, uint8_t pin, GPIO_State state)
{
    if (state == GPIO_HIGH)
        port->ODR |=  (1 << pin);
    else
        port->ODR &= ~(1 << pin);
}

void hal_gpio_toggle(GPIO_Port *port, uint8_t pin)
{
    port->ODR ^= (1 << pin);
}
