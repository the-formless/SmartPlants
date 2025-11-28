#include "i2c.h"
#include "clock.h"
#include "gpio.h"

void I2C_Init(uint32_t freq) 
{
    //enable i2c clock
    i2c_clock_init();
    

    GPIO_InitTypeDef i2c_cfg = {
        .mode = GPIO_MODE_OUTPUT,             // Open-drain output
        .pull = GPIO_NOPULL,                  // No internal pull (I2C requires external pull-ups; module has them)
        .outputType = GPIO_OUTPUT_OPENDRAIN,  // Open-drain mode
        .speed = GPIO_SPEED_FAST              // Fast edge for I2C timing
    };

    GPIO_InitPin(PB4, &i2c_cfg);  // SCL
    GPIO_InitPin(PB5, &i2c_cfg);  // SDA
    // Enable alternate function for I2C pins
    GPIOB->CR2 |= (1 << 4);   // PB4 SCL AF
    GPIOB->CR2 |= (1 << 5);   // PB5 SDA AF


    // Peripheral frequency (MHz)
    I2C->FREQR = 16 & 0x3F;

    //Own Address , not used
    I2C->OARH = 0x40;
    I2C->OARL = 0x00;


    // Standard mode (100 kHz)
    uint16_t ccr = 16000000UL / (freq * 2UL);
    if(ccr < 4) ccr = 4;
    I2C->CCRL = ccr & 0xFF;
    I2C->CCRH = (ccr >> 8) & 0x0F;

    // Maximum rise time (1000ns → Freq+1)
    I2C->TRISER = 17;

    // Enable ACK for reads
    I2C->CR2 |= I2C_CR2_ACK;

    // Enable I2C peripheral
    I2C->CR1 = I2C_CR1_PE;
}

void I2C_Start(void)
{
    I2C->CR2 |= I2C_CR2_START;
    while (!(I2C->SR1 & I2C_SR1_SB));
}

void I2C_Stop(void)
{
    I2C->CR2 |= I2C_CR2_STOP;
}

// Wait until bus is free, generate START and send 7-bit write address
void I2C_BeginWrite(uint8_t addr7)
{
    // Wait until bus not busy
    while (I2C->SR3 & I2C_SR3_BUSY) {
        // optional timeout here
    }

    // Generate START
    I2C->CR2 |= I2C_CR2_START;
    while (!(I2C->SR1 & I2C_SR1_SB));   // wait for Start Bit
    (void)I2C->SR1;                     // clear SB by reading SR1

    // Send address (R/W = 0)
    I2C->DR = (addr7 << 1);
    while (!(I2C->SR1 & I2C_SR1_ADDR)); // wait address sent
    (void)I2C->SR1;                     // clear ADDR
    (void)I2C->SR3;
}


void I2C_Write(uint8_t data)
{
    I2C->DR = data;
    while (!(I2C->SR1 & I2C_SR1_TXE));
}

uint8_t I2C_ReadAck(void)
{
    I2C->CR2 |= I2C_CR2_ACK;
    while (!(I2C->SR1 & I2C_SR1_RXNE));
    return I2C->DR;
}

uint8_t I2C_ReadNack(void)
{
    I2C->CR2 &= ~I2C_CR2_ACK;
    while (!(I2C->SR1 & I2C_SR1_RXNE));
    return I2C->DR;
}