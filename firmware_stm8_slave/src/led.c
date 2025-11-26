#include "../inc/led.h"
#include "../inc/hal_gpio.h"
void led_init(void) {
    hal_gpio_set_mode(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT_PP);
    led_off();
}
void led_on(void) {
    hal_gpio_write(LED_PORT, LED_PIN, GPIO_HIGH);
}
void led_off(void) {
    hal_gpio_write(LED_PORT, LED_PIN, GPIO_LOW);
}
void led_toggle(void) {
    static uint8_t state = 0;
    state = !state;
    hal_gpio_write(LED_PORT, LED_PIN, state);
}