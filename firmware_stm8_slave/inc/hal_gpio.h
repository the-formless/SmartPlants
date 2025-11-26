#ifndef HAL_GPIO_H
#define HAL_GPIO_H
#include <stdint.h>
// GPIO Pin definitions for STM8S003F3P6
#define GPIO_PORT_A     0
#define GPIO_PORT_B     1
#define GPIO_PORT_C     2
#define GPIO_PORT_D     3
// Pin modes
#define GPIO_MODE_INPUT_FLOATING   0x00
#define GPIO_MODE_OUTPUT_PP        0x10
#define GPIO_MODE_OUTPUT_OD        0x14
// Pin states
#define GPIO_LOW    0
#define GPIO_HIGH   1
void hal_gpio_init(void);
void hal_gpio_set_mode(uint8_t port, uint8_t pin, uint8_t mode);
void hal_gpio_write(uint8_t port, uint8_t pin, uint8_t state);
uint8_t hal_gpio_read(uint8_t port, uint8_t pin);
#endif // HAL_GPIO_H