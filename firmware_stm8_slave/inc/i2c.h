#ifndef STM8_I2C_H
#define STM8_I2C_H

#include <stdint.h>

//I2C Register map
typedef struct {
    volatile uint8_t CR1;
    volatile uint8_t CR2;
    volatile uint8_t FREQR;
    volatile uint8_t OARL;
    volatile uint8_t OARH;
    volatile uint8_t RESERVED;
    volatile uint8_t DR;
    volatile uint8_t SR1;
    volatile uint8_t SR2;
    volatile uint8_t SR3;
    volatile uint8_t ITR;
    volatile uint8_t CCRL;
    volatile uint8_t CCRH;
    volatile uint8_t TRISER;
    volatile uint8_t PECR;
} I2C_TypeDef;

#define I2C ((I2C_TypeDef*)0x5210)

//BIT masks
#define I2C_CR1_PE (1<<0)
#define I2C_CR2_START (1<<0)
#define I2C_CR2_STOP (1<<1)
#define I2C_CR2_ACK (1<<2)

#define I2C_SR1_SB (1<<0)
#define I2C_SR1_ADDR (1<<1)
#define I2C_SR1_BTF (1<<2)
#define I2C_SR1_TXE (1<<7)

//API 
void I2C_Init(void);
uint8_t I2C_Start(void);
uint8_t I2C_SendAddress(uint8_t addr);
uint8_t I2C_WriteByte(uint8_t b);
void I2C_Stop(void);

#endif