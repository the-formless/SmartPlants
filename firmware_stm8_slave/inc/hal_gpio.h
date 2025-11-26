#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>

/*
 * STM8S003 GPIO Register Map
 *
 * Each GPIO port has these registers at fixed addresses:
 *
 * ODR – Output Data Register
 * IDR – Input Data Register
 * DDR – Data Direction Register
 * CR1 – Control Register 1
 * CR2 – Control Register 2
 *
 * Port base addresses (from STM8S003 reference manual):
 */
#define GPIOA_BASE   0x5000
#define GPIOB_BASE   0x5005
#define GPIOC_BASE   0x500A
#define GPIOD_BASE   0x500F
#define GPIOE_BASE   0x5014

// Register structure for a GPIO port
typedef struct {
    volatile uint8_t ODR;   // +0x00
    volatile uint8_t IDR;   // +0x01
    volatile uint8_t DDR;   // +0x02
    volatile uint8_t CR1;   // +0x03
    volatile uint8_t CR2;   // +0x04
} GPIO_Port;

// Pointers to actual ports
#define GPIOA   ((GPIO_Port*) GPIOA_BASE)
#define GPIOB   ((GPIO_Port*) GPIOB_BASE)
#define GPIOC   ((GPIO_Port*) GPIOC_BASE)
#define GPIOD   ((GPIO_Port*) GPIOD_BASE)
#define GPIOE   ((GPIO_Port*) GPIOE_BASE)

// Enums for readability
typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1
} GPIO_Direction;

typedef enum {
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} GPIO_State;

// HAL API
void hal_gpio_init(GPIO_Port *port, uint8_t pin, GPIO_Direction direction);
void hal_gpio_write(GPIO_Port *port, uint8_t pin, GPIO_State state);
void hal_gpio_toggle(GPIO_Port *port, uint8_t pin);

#endif