#include "uart.h"
#include "tim2.h"
#include "ws2812.h"

volatile const uint8_t *ws_data_ptr;
volatile uint16_t ws_num_bits;
volatile uint8_t ws_sending = 0;

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

    if(ws_sending) {

        if(ws_num_bits == 0) {
            //finish
            ws_sending = 0;
            TIM2_CR1 &= ~0x01; //disable TIM2
            return;
        }

        uint8_t byte_index = ws_num_bits / 8;
        uint8_t bit_index = 7 - (ws_num_bits % 8); //MSB first

        uint8_t bit_value = (ws_data_ptr[byte_index] >> bit_index) & 1;

        if(bit_value)
            TIM2_CCR2L = 11;
        else
            TIM2_CCR2L = 6;

        ws_num_bits--;
    }
}