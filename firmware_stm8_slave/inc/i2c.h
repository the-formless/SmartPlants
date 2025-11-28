#ifndef STM8_I2C_H
#define STM8_I2C_H

#include <stdint.h>

typedef struct {
    volatile uint8_t CR1;
    volatile uint8_t CR2;
    volatile uint8_t FREQR;
    volatile uint8_t OARL;
    volatile uint8_t OARH;
    volatile uint8_t RESERVEDREGISTER;
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

#define I2C_CR1_PE          (1 << 0)   // Peripheral Enable
#define I2C_CR1_ENGC        (1 << 6)   // General Call Enable
#define I2C_CR1_NOSTRETCH   (1 << 7)   // Clock Stretch Disable

#define I2C_CR2_ACK         (1 << 2)   // Acknowledge Enable
#define I2C_CR2_STOP        (1 << 1)   // Generate STOP
#define I2C_CR2_START       (1 << 0)   // Generate START

#define I2C_SR1_SB          (1 << 0)   // Start Condition Sent
#define I2C_SR1_ADDR        (1 << 1)   // Address Sent / Matched
#define I2C_SR1_BTF         (1 << 2)   // Byte Transfer Finished
#define I2C_SR1_RXNE        (1 << 6)   // Data Register Not Empty (received)
#define I2C_SR1_TXE         (1 << 7)   // Data Register Empty (ready to send)

#define I2C_SR2_BERR        (1 << 0)   // Bus Error
#define I2C_SR2_ARLO        (1 << 1)   // Arbitration Lost
#define I2C_SR2_AF          (1 << 2)   // Acknowledge Failure (NACK)
// #define I2C_SR2_BUSY        (1 << 1)   // Bus Busy
#define I2C_SR2_OVR         (1 << 3)   // Overrun

#define I2C_SR3_MSL         (1 << 0)   // Master/Slave
#define I2C_SR3_BUSY        (1 << 1)   // Bus Busy
#define I2C_SR3_TRA         (1 << 2)   // Transmitter / Receiver

#define I2C_CCRH_FS         (1 << 7)  // Fast mode
#define I2C_CCRH_DUTY       (1 << 6)  // Fast mode duty


void I2C_Init(uint32_t clockHz);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(uint8_t data);
uint8_t I2C_ReadAck(void);
uint8_t I2C_ReadNack(void);
void I2C_BeginWrite(uint8_t addr7);


#endif