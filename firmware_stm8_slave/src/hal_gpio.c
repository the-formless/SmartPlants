#include "../inc/hal_gpio.h"
// STM8 GPIO register definitions
#define GPIO_BASE       0x5000
#define GPIO_CR1_OFFSET 0x03
#define GPIO_CR2_OFFSET 0x04
#define GPIO_ODR_OFFSET 0x00
#define GPIO_IDR_OFFSET 0x01
#define GPIO_DDR_OFFSET 0x02
#define GPIO_PORT_SIZE  0x05
#define GPIO_CR1(port)  (*(volatile uint8_t*)(GPIO_BASE + (port * GPIO_PORT_SIZE) + GPIO_CR1_OFFSET))
#define GPIO_CR2(port)  (*(volatile uint8_t*)(GPIO_BASE + (port * GPIO_PORT_SIZE) + GPIO_CR2_OFFSET))
#define GPIO_ODR(port)  (*(volatile uint8_t*)(GPIO_BASE + (port * GPIO_PORT_SIZE) + GPIO_ODR_OFFSET))
#define GPIO_IDR(port)  (*(volatile uint8_t*)(GPIO_BASE + (port * GPIO_PORT_SIZE) + GPIO_IDR_OFFSET))
#define GPIO_DDR(port)  (*(volatile uint8_t*)(GPIO_BASE + (port * GPIO_PORT_SIZE) + GPIO_DDR_OFFSET))
void hal_gpio_init(void) {
    // Enable GPIO peripheral clock (already enabled by default on STM8S003)
}
void hal_gpio_set_mode(uint8_t port, uint8_t pin, uint8_t mode) {
    uint8_t pin_mask = (uint8_t)(1 << pin);
    
    // Clear current mode
    GPIO_CR1(port) &= (uint8_t)~pin_mask;
    GPIO_CR2(port) &= (uint8_t)~pin_mask;
    
    // Set new mode
    GPIO_CR1(port) |= (uint8_t)(mode & 0x01) << pin;
    GPIO_CR2(port) |= (uint8_t)((mode >> 4) & 0x01) << pin;
}
void hal_gpio_write(uint8_t port, uint8_t pin, uint8_t state) {
    if (state) {
        GPIO_ODR(port) |= (uint8_t)(1 << pin);
    } else {
        GPIO_ODR(port) &= (uint8_t)~(1 << pin);
    }
}
uint8_t hal_gpio_read(uint8_t port, uint8_t pin) {
    return (GPIO_IDR(port) >> pin) & 0x01;
}