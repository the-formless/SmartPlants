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
void UART1_ReadString(char *buffer, uint8_t maxLen);
uint8_t UART1_Read(void);
uint8_t UART1_Available(void);

//interrup driven RX support
extern volatile uint8_t uart1_rx_byte;
extern volatile uint8_t uart1_rx_flag;

#define UART_DIV(f_cpu, baud) ((f_cpu) / (baud))

//CR2 Bits
#define UART1_CR2_REN (1 << 2)  //RX Enable
#define UART1_CR2_TEN (1 << 3)  //TX Enable
#define UART1_CR2_RIEN (1 << 5) //Receiver Interrupt Enable
#define UART1_CR2_TIEN (1 << 7) //Transmitter Interrupt Enable

//SR Bits 
#define UART1_SR_RXNE (1 << 5) 
#define UART1_SR_TXE  (1 << 7) 

#define UART1_RX_BUFFER_SIZE 64

// ISR prototype 
void UART1_RX_IRQHandler(void) __interrupt(18);
void UART1_TX_IRQHandler(void) __interrupt(17);



//ring buffer state
extern volatile uint8_t uart1_rx_buffer[UART1_RX_BUFFER_SIZE];
extern volatile uint8_t uart1_rx_head;
extern volatile uint8_t uart1_rx_tail;

#define UART1_TX_BUFFER_SIZE 64

//tx ring buffer state 
extern volatile uint8_t uart1_tx_buffer[UART1_TX_BUFFER_SIZE];
extern volatile uint8_t uart1_tx_head;
extern volatile uint8_t uart1_tx_tail;

void UART1_WriteAsync(uint8_t b);
void UART1_WriteStringAsync(const char *s);

void UART1_WriteHex(uint8_t value);



#endif