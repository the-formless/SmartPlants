#include "clock.h"

void clock_init(void) {
    //No division -> full 16MHz internal clock
    CLK_CKDIVR = 0x00;
}