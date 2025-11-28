/**
 * 
 * STM8 UART uses:

    BRR1 → high 8 bits of divider

    BRR2 → low 4 bits high nibble (mantissa) + low 4 bits low nibble (fraction)

    Formula for divider:
        USARTDIV = f_CPU / baud
        BRR1 = high 8 bits
        BRR2 = low 4 bits high nibble | fraction in low nibble
 */

#include "uart.h"
#include "gpio.h"
#include "clock.h"


volatile uint8_t uart1_rx_byte = 0;
volatile uint8_t uart1_rx_flag = 0;

void UART1_Init(UART_InitTypeDef *cfg) {

    //Enable UART1 PLCLK 
    uart_clock_init();

    uint32_t div = UART_DIV(16000000UL, cfg->baudrate);

    //-----
    //Enable TX/RX pins
    //-----
    GPIO_InitTypeDef tx_cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_NOPULL,
        .outputType = GPIO_OUTPUT_PUSHPULL,
        .speed = GPIO_SPEED_FAST
    };
    GPIO_InitPin(PD5, &tx_cfg); //STM8 TX Pin - PD5 (pin 2)

    GPIO_InitTypeDef rx_cfg = {
        .mode = GPIO_MODE_INPUT,
        .pull = GPIO_PULLUP,
        .outputType = GPIO_OUTPUT_OPENDRAIN, //irrelevant for input
        .speed = GPIO_SPEED_SLOW
    };
    GPIO_InitPin(PD6, &rx_cfg); //STM8 RX Pin - PD6 (pin 3)

    //-----
    //Set Parity
    //-----
    if(cfg->parity == UART_PARITY_NONE) {
        UART1->CR1 &= ~(1<<2); //PCEN = 0 -> parity disabled
    } else {
        UART1->CR1 |=(1<<2); //PCEN = 1 -> parity enabled
        if(cfg->parity == UART_PARITY_ODD)
            UART1->CR1 |= (1<<1); //PS = 1
        else
            UART1->CR1 &= ~(1 << 1); //PS = 0
    }

    //-----
    //Set Stop Bits
    //-----
    if(cfg->stopBits == UART_STOPBITS_2)
        UART1->CR3 |= (1<<5); //STOP = 10(2 stop bits)
    else
        UART1->CR3 &= ~(1<<5); //STOP = 00(1 stop bit)

    //-----
    //Set Baud Rate Registers
    //-----
    UART1->BRR2 = (uint8_t)(((div & 0xF000) >> 8) | (div & 0x000F));
    UART1->BRR1 = (uint8_t)(div >> 4);
    
    //-----
    //Enable UART
    //-----
    // UART1->CR2 |= (1<<2);//enable RX
    // UART1->CR2 |= (1<<3);//enable TX
    // UART1->CR2 |= (1<<5);//enable UART 
    //Enable interrupt UART
    UART1->CR2 |= UART1_CR2_REN | UART1_CR2_TEN | UART1_CR2_RIEN | UART1_CR2_TIEN; //UART Enable + RX/TX Enable + RX Interrupt Enable + TIEN

    //Clear pending Junk
    (void)UART1->SR;
    (void)UART1->DR;

}

void UART1_Write(uint8_t data)
{
    while(!(UART1->SR & UART1_SR_TXE)); //wait TXE 
    UART1->DR = data;
}

void UART1_WriteString(const char *str)
{
    while(*str) {
        UART1_Write((uint8_t)*str++);
    }
}

void UART1_ReadString(char *buffer, uint8_t maxLen) {
    uint8_t idx = 0;
    
    while(1) {
        //wait for next character
        while(!UART1_Available());

        uint8_t c = UART1_Read();

        //Stop if terminator found 
        if( c == '\n' || c == '\r') {
            break;
        }

        //store character
        if(idx < maxLen - 1) { //leave space for null terminator
            buffer[idx++] = c;
        }
    }
    buffer[idx] = '\0'; //null terminate
}

uint8_t UART1_Read(void)
{
    while (uart1_rx_head == uart1_rx_tail) {
        // wait or return 0. We can check this if something fails
    }

    uint8_t data = uart1_rx_buffer[uart1_rx_tail];
    uart1_rx_tail = (uart1_rx_tail +1) % UART1_RX_BUFFER_SIZE;

    return data;
}

uint8_t UART1_Available(void)
{
    return (uart1_rx_head != uart1_rx_tail); 
}

void UART1_WriteAsync(uint8_t b) {
    uint8_t next = (uart1_tx_head + 1) % UART1_TX_BUFFER_SIZE;

    //wait for space in buffer
    while(next == uart1_tx_tail);

    uart1_tx_buffer[uart1_tx_head] = b;
    uart1_tx_head = next;

    //enable TXE interrupt
    UART1->CR2 |= UART1_CR2_TIEN;
}

void UART1_WriteStringAsync(const char *s) {
    while(*s) {
        UART1_WriteAsync((uint8_t)*s++);
    }
}