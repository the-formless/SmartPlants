#include "uart.h"

void UART1_RX_IRQHandler(void) __interrupt(18) {
    
    uint8_t sr = UART1->SR; //read status register
    if(sr & UART1_SR_RXNE) {
        uint8_t data = UART1->DR; //reading DR clears RXNE

        uart1_rx_byte = data;
        uart1_rx_flag = 1;
    } else {
        // some error - read DR to clear flags
        (void)UART1->DR;
    }
}