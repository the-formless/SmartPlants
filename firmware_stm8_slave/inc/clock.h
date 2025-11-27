#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

#define CLK_ICKR      (*(volatile uint8_t*)0x50C0) // Internal Clock Register
#define CLK_ECKR      (*(volatile uint8_t*)0x50C1) // External Clock Register
#define CLK_CMSR      (*(volatile uint8_t*)0x50C3) // Clock Master Status Register
#define CLK_SWR       (*(volatile uint8_t*)0x50C4) // Clock Switch Register
#define CLK_SWCR      (*(volatile uint8_t*)0x50C5) // Clock Switch Control Register
#define CLK_CKDIVR    (*(volatile uint8_t*)0x50C6) // Clock Divider Register
#define CLK_PCKENR1   (*(volatile uint8_t*)0x50C7) // Peripheral Clock Gating Register 1
#define CLK_CSSR      (*(volatile uint8_t*)0x50C8) // Clock Security System Register
#define CLK_CCOR      (*(volatile uint8_t*)0x50C9) // Configurable Clock Output/control Register    
#define CLK_PCKENR2   (*(volatile uint8_t*)0x50CA) // Peripheral Clock Gating Register 2
#define CLK_HSITRIMR  (*(volatile uint8_t*)0x50CC) // HSI Calibration Trimmer Register
#define CLK_SWIMCCR   (*(volatile uint8_t*)0x50CD) // SWIM Clock Control Register

void clock_init(void);

#endif