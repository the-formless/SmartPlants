#include "uart.h"

volatile uint8_t uart1_rx_buffer[UART1_RX_BUFFER_SIZE];
volatile uint8_t uart1_rx_head = 0;
volatile uint8_t uart1_rx_tail = 0;

volatile uint8_t uart1_tx_buffer[UART1_TX_BUFFER_SIZE];
volatile uint8_t uart1_tx_head = 0;
volatile uint8_t uart1_tx_tail = 0;

void UART1_RX_IRQHandler(void) __interrupt(18) {
    
    if(UART1->SR & UART1_SR_RXNE) {
        uint8_t data = UART1->DR;
        uint8_t next = (uart1_rx_head +1) % UART1_RX_BUFFER_SIZE;

        //Overrun check. true-> drop oldest byte by advancing tail
        if(next == uart1_rx_tail) {
            uart1_rx_tail = (uart1_rx_tail + 1) % UART1_RX_BUFFER_SIZE;
        }

        uart1_rx_buffer[uart1_rx_head] = data;
        uart1_rx_head = next;
    } else {
        //Clear any noise
        (void)UART1->DR;
    }
}

void UART1_TX_IRQHandler(void) __interrupt(17)
{
    if(uart1_tx_head != uart1_tx_tail) {
        //there is data to send
        UART1->DR = uart1_tx_buffer[uart1_tx_tail];
        uart1_tx_tail = (uart1_tx_tail + 1) % UART1_TX_BUFFER_SIZE;
    } else {
        //no more data to send, disable TXE interrupt
        UART1->CR2 &= ~UART1_CR2_TIEN;
    }
}