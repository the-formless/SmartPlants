#include "i2c.h"
#include "clock.h"
#include "gpio.h"
#include "uart.h"   // only for debug prints 
#include "tim4.h"

void I2C_Init(uint32_t freq)
{
    UART1_WriteString("I2C init START\r\n");

    i2c_clock_init();

    // Step 1: Release the bus (handles LCD stuck-low state)
    GPIO_I2C_ReleaseBus(PB4, PB5);

    // Step 2: Configure PB4/PB5 for I2C
    GPIO_SetPinMode(PB4, GPIO_MODE_INPUT);
    GPIO_SetPinMode(PB5, GPIO_MODE_INPUT);

    GPIO_SetPinAF(PB4);
    GPIO_SetPinAF(PB5);

    // Step 3: I2C peripheral registers
    I2C->CR1 = 0;
    I2C->CR2 = 0;
    I2C->OARL = 0;
    I2C->OARH = 0;
    I2C->FREQR = 16;

    uint16_t ccr = 16000000 / (freq * 2);
    I2C->CCRL = ccr & 0xFF;
    I2C->CCRH = (ccr >> 8) & 0x0F;

    I2C->TRISER = 17;

    I2C->CR1 = I2C_CR1_PE;

    UART1_WriteString("I2C_init DONE!\r\n");
}

void I2C_Start(void)
{
    I2C->CR2 |= I2C_CR2_START;
    while(!(I2C->SR1 & I2C_SR1_SB));  // Wait START
}

void I2C_Stop(void)
{
    I2C->CR2 |= I2C_CR2_STOP;
}

void I2C_Write(uint8_t data)
{
    I2C->DR = data;
    UART1_WriteString("  SR3: ");
    UART1_WriteHex8(I2C->SR3);
    UART1_WriteString("\r\n");

    UART1_WriteString("  SR2: ");
    UART1_WriteHex8(I2C->SR2);
    UART1_WriteString("\r\n");

    uint32_t timeout = 50000;
    while (timeout--)
    {
        if (I2C->SR1 & I2C_SR1_TXE)
            return;

        if (I2C->SR2 & I2C_SR2_AF)
        {
            UART1_WriteString(" !!! DATA NACK (AF) !!!\r\n");
            I2C->SR2 &= ~I2C_SR2_AF; // clear AF
            return;
        }
    }

    UART1_WriteString(" !!! TXE TIMEOUT !!!\r\n");
}


uint8_t I2C_Probe(uint8_t addr)
{
    // Generate START
    I2C->CR2 |= I2C_CR2_START;
    while (!(I2C->SR1 & I2C_SR1_SB));

    // Send address
    I2C->DR = (addr << 1); // write mode

    // Wait for either ADDR or AF (NACK)
    uint32_t timeout = 50000;
    while (timeout--)
    {
        // NACK?
        if (I2C->SR2 & I2C_SR2_AF)
        {
            I2C->SR2 &= ~I2C_SR2_AF;   // clear
            I2C->CR2 |= I2C_CR2_STOP;  // stop
            return 0;                  // not found
        }

        // ACK?
        if (I2C->SR1 & I2C_SR1_ADDR)
        {
            volatile uint8_t dummy;
            dummy = I2C->SR1; // clear ADDR by reading both
            dummy = I2C->SR3;

            I2C->CR2 |= I2C_CR2_STOP;
            return 1; // found!
        }
    }

    // TIMEOUT → treat as NACK
    I2C->CR2 |= I2C_CR2_STOP;
    return 0;
}

void I2C_Scan(void)
{
    UART1_WriteString("Scanning...\r\n");

    for (uint8_t a = 1; a < 127; a++)
    {
        if (I2C_Probe(a))
        {
            UART1_WriteString("FOUND: 0x");
            UART1_WriteHex(a);
            UART1_WriteString("\r\n");
        }
    }

    UART1_WriteString("Scan done.\r\n");
}
