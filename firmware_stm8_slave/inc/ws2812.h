#ifndef WS2812_H
#define WS2812_H

#include <stdint.h>

extern volatile const uint8_t *ws_data_ptr;
extern volatile uint16_t ws_num_bits;
extern volatile uint8_t ws_sending;

void WS2812_init(void);
void WS2812_send(uint8_t *data, uint16_t length);
void apply_color(const char *cmd);

#endif  // WS2812_H