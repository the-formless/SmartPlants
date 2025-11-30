#include "tim4.h"

void TIM4_UPD_OVF_IRQHandler(void) __interrupt(23) //if something breaks, verify vector number
{
    TIM4_SR &= ~0x01; //clear update flag
    g_ticks++; //1ms tick
}