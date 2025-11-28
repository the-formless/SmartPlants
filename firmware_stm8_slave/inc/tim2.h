#ifndef TIM2_H
#define TIM2_H

#include <stdint.h>

#define TIM2_CR1 (*(volatile uint8_t *)0x5300) // Control Register 1
#define TIM2_IER (*(volatile uint8_t *)0x5303) // Interrupt Enable Register
#define TIM2_SR1 (*(volatile uint8_t *)0x5304) // Status Register 1
#define TIM2_PSCR (*(volatile uint8_t *)0x530E) // Prescaler Register
#define TIM2_ARRH (*(volatile uint8_t *)0x530F) // Auto-Reload Register High
#define TIM2_ARRL (*(volatile uint8_t *)0x5310) // Auto-Reload Register Low
#define TIM2_CCER1 (*(volatile uint8_t *)0x530A) // Capture/Compare Enable Register 1
#define TIM2_CCMR2 (*(volatile uint8_t *)0x5308) // Capture/Compare Mode Register 2
#define TIM2_CCR2H (*(volatile uint8_t *)0x5313) // Capture/Compare Register 2 High
#define TIM2_CCR2L (*(volatile uint8_t *)0x5314) // Capture/Compare Register 2 Low


void TIM2_Init(uint8_t period_us);

void TIM2_UPD_OVF_IRQHandler(void) __interrupt(13);     // Timer2 Update/Overflow Interrupt Handler

#endif