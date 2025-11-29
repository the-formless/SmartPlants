#include "i2c.h"
#include "clock.h"
#include "gpio.h"
#include "uart.h"   // only for debug prints 
#include "tim4.h"

void I2C_Init(void) {
    UART1_WriteString("I2C init STart\r\n");

    //Enable Clock
    i2c_clock_init();
    UART1_WriteString("Clock enabled\r\n");

    //disable peripheral during setup
    I2C->CR1 &= ~I2C_CR1_PE;

    //configure scl/sda pins as open drain
    GPIO_InitTypeDef cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_NOPULL,
        .outputType = GPIO_OUTPUT_OPENDRAIN,
        .speed = GPIO_SPEED_FAST
    };

    GPIO_InitPin(PB4, &cfg);
    GPIO_InitPin(PB5, &cfg);

    UART1_WriteString("GPIO set\r\n");

    //frequency register
    I2C->FREQR = 16;

    //ccr for 100kHz
    uint8_t ccr = 16000000UL / (100000UL*2UL);
    if(ccr < 4) ccr = 4;

    I2C->CCRL = (uint8_t)(ccr & 0xFF);
    I2C->CCRH = (uint8_t)((ccr >> 8) & 0x0F);

    //rise time
    I2C->TRISER = 17;

    //own address
    I2C->OARL = 0x00;
    I2C->OARH = 0x40;       //ADDCONF = 1, ADDMODE=0

    //enable peripehral
    I2C->CR1 |= I2C_CR1_PE;

    //enable ACK
    I2C->CR2 |= I2C_CR2_ACK;

    UART1_WriteString("I2C_init DONE!\r\n");
}

uint8_t I2C_Start(void)
{
    UART1_WriteString("I2C_start\r\n");

    I2C->CR2 |= I2C_CR2_START;

    uint16_t timeout = 30000;
    while(!(I2C->SR1 & I2C_SR1_SB)) {
        if(--timeout == 0) {
            UART1_WriteString("SB Timeout\r\n");
            return 0;
        }
    }

    UART1_WriteString("SB Okay\r\n");
    return 1;
}

void I2C_Stop(void) {
    UART1_WriteString("STOP\r\n");
    I2C->CR2 |= I2C_CR2_STOP;
}

