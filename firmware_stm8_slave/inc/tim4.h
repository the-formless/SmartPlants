#ifndef TIM4_H
#define TIM4_H

#include <stdint.h>

#define TIM4_CR1 (*(volatile uint8_t*)0x5340) // Control Register 1
#define TIM4_IER (*(volatile uint8_t*)0x5343) // Interrupt Enable Register
#define TIM4_SR  (*(volatile uint8_t*)0x5344) // Status Register
#define TIM4_EGR (*(volatile uint8_t*)0x5345) // Event Generation Register
#define TIM4_CNTR (*(volatile uint8_t*)0x5346) // Counter Register
#define TIM4_PSCR (*(volatile uint8_t*)0x5347) // Prescaler Register
#define TIM4_ARR  (*(volatile uint8_t*)0x5348) // Auto-Reload Register

void tim4_init(void);
void tim4_delay(uint16_t ms);

#endif