#ifndef HAL_GPIO_H
#define HAL_GPIO_H
#include <stdint.h>
// GPIO Pin definitions for STM8S003F3P6
#define GPIO_PORT_A     0
#define GPIO_PORT_B     1
#define GPIO_PORT_C     2
#define GPIO_PORT_D     3
// Pin modes
// bit 4 = output (1)
// bit 1 = CR1 (push‐pull = 1)
// bit 0 = CR2 (fast = 1)

#define GPIO_MODE_INPUT_FLOATING     0x00      // DDR0 CR10 CR20
#define GPIO_MODE_OUTPUT_PP_SLOW     0x12      // DDR1 CR11 CR20
#define GPIO_MODE_OUTPUT_PP_FAST     0x13      // DDR1 CR11 CR21
#define GPIO_MODE_OUTPUT_OD_SLOW     0x10      // DDR1 CR10 CR20
#define GPIO_MODE_OUTPUT_OD_FAST     0x11      // DDR1 CR10 CR21
// Pin states
#define GPIO_LOW    0
#define GPIO_HIGH   1
void hal_gpio_init(void);
void hal_gpio_set_mode(uint8_t port, uint8_t pin, uint8_t mode);
void hal_gpio_write(uint8_t port, uint8_t pin, uint8_t state);
uint8_t hal_gpio_read(uint8_t port, uint8_t pin);
#endif // HAL_GPIO_H