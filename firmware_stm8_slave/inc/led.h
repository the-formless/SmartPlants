#ifndef LED_H
#define LED_H
#include <stdint.h>
#define LED_PORT    GPIO_PORT_C
#define LED_PIN     4
void led_init(void);
void led_on(void);
void led_off(void);
void led_toggle(void);
#endif // LED_H