#include "gpio.h"
#include "tim4.h"

void HeartbeatTask(void) 
{
    GPIO_TogglePin(PD2);
}