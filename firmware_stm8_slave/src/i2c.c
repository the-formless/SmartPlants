#include "i2c.h"
#include "clock.h"
#include "gpio.h"
#include "uart.h"   // only for debug prints 
// #include "tim4.h"

void I2C_Init(void) {
    UART1_WriteString("I2C init STart\r\n");
    
    // Disable TIM2 to release PB4 (TIM2_CH1 alternate function)
    UART1_WriteString("Disabling TIM2...\r\n");
    CLK_PCKENR1 &= ~(1 << 5);  // Disable TIM2 clock
    UART1_WriteString("TIM2 disabled\r\n");
    
    // Enable I2C Clock
    i2c_clock_init();
    UART1_WriteString("Clock enabled\r\n");
    
    // Disable I2C peripheral during setup
    I2C->CR1 &= ~I2C_CR1_PE;
    
    // Configure PB4 (SCL) and PB5 (SDA) as floating inputs
    UART1_WriteString("Configuring GPIO...\r\n");
    
    GPIOB->DDR &= ~((1<<4) | (1<<5));  // Input
    GPIOB->CR1 &= ~((1<<4) | (1<<5));  // Floating
    GPIOB->CR2 |= (1<<4) | (1<<5);     // Fast mode
    
    UART1_WriteString("GPIO configured - DDR=");
    UART1_WriteHex(GPIOB->DDR);
    UART1_WriteString(" CR1=");
    UART1_WriteHex(GPIOB->CR1);
    UART1_WriteString(" CR2=");
    UART1_WriteHex(GPIOB->CR2);
    UART1_WriteString("\r\n");
    
    UART1_WriteString("GPIO set\r\n");
    
    // Set peripheral input clock frequency (in MHz)
    I2C->FREQR = 16;
    
    // Calculate CCR for 100kHz I2C (standard mode)
    // CCR = Fmaster / (2 * Fi2c) where Fmaster = 16MHz, Fi2c = 100kHz
    uint16_t ccr = 16000000UL / (100000UL * 2UL);  // = 80
    if(ccr < 4) ccr = 4;
    
    I2C->CCRL = (uint8_t)(ccr & 0xFF);
    I2C->CCRH = (uint8_t)((ccr >> 8) & 0x0F);
    
    // Rise time = (max rise time / Tmaster) + 1
    // For 100kHz: max rise = 1000ns, Tmaster = 62.5ns (16MHz)
    // TRISER = 1000/62.5 + 1 = 17
    I2C->TRISER = 17;
    
    // Own address (not critical for master mode)
    I2C->OARL = 0x00;
    I2C->OARH = 0x40;  // ADDCONF = 1
    
    // Clear any pending flags
    (void)I2C->SR1;
    (void)I2C->SR3;
    
    // Enable peripheral
    I2C->CR1 |= I2C_CR1_PE;
    
    // Enable ACK for receiving
    I2C->CR2 |= I2C_CR2_ACK;
    
    UART1_WriteString("I2C_init DONE!\r\n");
}

uint8_t I2C_Start(void) {
    UART1_WriteString("I2C_start\r\n");
    
    // Clear any existing START condition
    I2C->CR2 &= ~I2C_CR2_START;
    
    // Make sure bus is not busy
    uint16_t timeout = 10000;
    while(I2C->SR3 & I2C_SR3_BUSY) {
        if(--timeout == 0) {
            UART1_WriteString("Bus BUSY timeout\r\n");
            return 0;
        }
    }
    
    // Generate START condition
    I2C->CR2 |= I2C_CR2_START;
    
    // Wait for SB flag (START byte sent)
    timeout = 30000;
    while(!(I2C->SR1 & I2C_SR1_SB)) {
        if(--timeout == 0) {
            UART1_WriteString("SB Timeout - SR1=");
            UART1_WriteHex(I2C->SR1);
            UART1_WriteString(" SR3=");
            UART1_WriteHex(I2C->SR3);
            UART1_WriteString("\r\n");
            return 0;
        }
    }
    
    UART1_WriteString("SB Okay\r\n");
    return 1;
}

void I2C_Stop(void) {
    UART1_WriteString("STOP\r\n");
    I2C->CR2 |= I2C_CR2_STOP;
    
    // Wait for STOP to complete
    uint16_t timeout = 10000;
    while(I2C->SR3 & I2C_SR3_BUSY) {
        if(--timeout == 0) break;
    }
}