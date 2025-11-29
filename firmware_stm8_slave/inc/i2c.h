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

#define I2C_SR2_BERR   (1 << 0)   // Bus error
#define I2C_SR2_ARLO   (1 << 1)   // Arbitration lost
#define I2C_SR2_AF     (1 << 2)   // Acknowledge failure (NACK)
#define I2C_SR2_OVR    (1 << 3)   // Overrun/Underrun


// I2C Status Register 3 (SR3)
#define I2C_SR3_MSL   (1 << 0)  // 0x01 - Master/Slave
#define I2C_SR3_BUSY  (1 << 1)  // 0x02 - Bus busy
#define I2C_SR3_TRA   (1 << 2)  // 0x04 - Transmitter/Receiver
#define I2C_SR3_GENCALL (1 << 4) // 0x10 - General call header
#define I2C_SR3_DUALF (1 << 7)  // 0x80 - Dual flag

//API 
void I2C_Init(uint32_t freq);
void I2C_Start(void);
uint8_t I2C_SendAddress(uint8_t addr);
void I2C_Write(uint8_t b);
void I2C_Stop(void);
void I2C_Scan(void);
uint8_t I2C_Probe(uint8_t addr);
#endif