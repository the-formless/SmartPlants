#include "uart.h"
#include "tim2.h"

void TIM2_UPD_OVF_IRQHandler(void) __interrupt(13) {
    //Clear update Flag
    TIM2_SR1 &= ~(1<<0);

    //if TX buffer has data, enable TXE interrupt
    if(uart1_tx_head != uart1_tx_tail) {
        uint8_t data = uart1_tx_buffer[uart1_tx_tail];
        uart1_tx_tail = (uart1_tx_tail + 1) % UART1_TX_BUFFER_SIZE;

        //Wait for UART to be ready
        while(!(UART1->SR & UART1_SR_TXE));
        UART1->DR = data;
    }
}