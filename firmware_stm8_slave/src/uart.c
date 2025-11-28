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

#define UART_DIV(f_cpu, baud) ((f_cpu) / (baud))

void UART1_Init(UART_InitTypeDef *cfg) {
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
    UART1->CR2 |= (1<<2);//enable RX
    UART1->CR2 |= (1<<3);//enable TX
    UART1->CR2 |= (1<<5);//enable UART 
}

void UART1_Write(uint8_t data)
{
    while(!(UART1->SR & (1<<7))); //TXE
    UART1->DR = data;
}

void UART1_WriteString(const char *str)
{
    while(*str) {
        UART1_Write(*str++);
    }
}

uint8_t UART1_Read(void)
{
    while(!(UART1->SR & (1<<5))); //RXNE
    return UART1->DR;
}

uint8_t UART1_Available(void)
{
    return (UART1->SR & (1<<5)) != 0; 
}