#ifndef STM8_UART_H
#define STM8_UART_H

#include <stdint.h>

typedef struct {
    // UART register definitions offsets (implicit)
    volatile uint8_t SR;   // Status Register
    volatile uint8_t DR;   // Data Register
    volatile uint8_t BRR1; // Baud Rate Register 1
    volatile uint8_t BRR2; // Baud Rate Register 2
    volatile uint8_t CR1;  // Control Register 1
    volatile uint8_t CR2;  // Control Register 2
    volatile uint8_t CR3;  // Control Register 3
    volatile uint8_t CR4;  // Control Register 4
    volatile uint8_t CR5;  // Control Register 5
    volatile uint8_t GTR;  // Guard Time Register
    volatile uint8_t PSCR; // Prescaler Register
} UART_TypeDef;

#define UART1 ((UART_TypeDef*) 0x5230)

typedef enum {
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN,
    UART_PARITY_ODD
} UART_Parity;

typedef enum {
    UART_STOPBITS_1 = 0,
    UART_STOPBITS_2
} UART_StopBits;

typedef struct {
    uint32_t baudrate;
    UART_Parity parity;
    UART_StopBits stopBits;
} UART_InitTypeDef;

void UART1_Init(UART_InitTypeDef *config);
void UART1_Write(uint8_t data);
void UART1_WriteString(const char *str);
uint8_t UART1_Read(void);
uint8_t UART1_Available(void);

#endif