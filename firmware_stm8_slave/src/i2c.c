// START →
//    SB (start bit sent) →
//       ADDR sent →
//          ADDR ACK → 
//             TXE (data register empty) →
//                TXE → TXE … (each byte) →
// STOP





#include "i2c.h"
#include "clock.h"
#include "gpio.h"
#include "uart.h"   // only for debug prints 
#include "tim4.h"
#include <stdio.h>


volatile I2C_State i2c_state = I2C_STATE_IDLE;
volatile I2C_Transaction i2c_txn;
volatile uint8_t i2c_txn_active = 0;


// Optional: for timeouts, logging
volatile uint32_t i2c_last_activity_ms = 0;

static uint8_t i2c_addr = 0;
static const uint8_t *i2c_tx_buf = 0;
static uint8_t i2c_tx_len = 0;
static uint8_t i2c_tx_pos = 0;
static I2C_Error_t i2c_last_error = I2C_ERR_NONE;
static uint32_t i2c_deadline = 0;


void I2C_Init(uint32_t freq)
{

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

    uint16_t ccr = 16000000 / (freq * 2UL);
    if (ccr < 4) ccr = 4; // minimum
    I2C->CCRL = ccr & 0xFF;
    I2C->CCRH = (ccr >> 8) & 0x0F;

    I2C->TRISER = 17;

    // 6) Own address (not really used in master mode)
    I2C->OARL = 0x00;
    I2C->OARH = 0x40; // 7-bit address mode, own addr = 0

    // 7) Enable interrupts: error, event, buffer
    I2C->ITR = I2C_ITR_ITERREN | I2C_ITR_ITEVTEN | I2C_ITR_ITBUFEN;

    I2C->CR1 = I2C_CR1_PE;
    // I2C->CR2 |= I2C_CR2_ACK;    // <---- THIS IS THE MISSING STEP

    i2c_state = I2C_STATE_IDLE;

}

uint8_t I2C_IsIdle(void) {
    return (i2c_txn_active == 0 && i2c_state == I2C_STATE_IDLE);
}

I2C_State I2C_GetState(void)
{
    return i2c_state;
}



uint8_t I2C_BeginWrite(uint8_t addr, const uint8_t *data, uint8_t len)
{
    if(i2c_state != I2C_STATE_IDLE) {
        return 1; //busy
    }

    //transaction fill
    i2c_txn.address = addr;
    i2c_txn.data = data;
    i2c_txn.length = len;
    i2c_txn.index = 0;

    i2c_last_activity_ms = millis();

    if (data == NULL || len == 0)
        return -2;

    if ((addr & 0x80) != 0)
        return -3;     // address must be 7-bit

    
    // Mark transaction active
    i2c_txn_active = 1;

    //Generate Start Condition
    i2c_state = I2C_STATE_START;
    I2C->CR2 |= I2C_CR2_START;

    return 0;

    // i2c_last_error = I2C_ERR_NONE;

    // //Kick off a transaction
    // i2c_state = I2C_STATE_START;
    // i2c_deadline = millis() + I2C_TIMEOUT_MS;

    // return 1; //started
}

//Abort n Recover
void I2C_Abort(void) 
{
    //Generate Stop
    I2C->CR2 |= I2C_CR2_STOP;

    //Clear AF
    I2C->SR2 &= (uint8_t)~I2C_SR2_AF;

    // Optionally re-init bus lines with GPIO recovery
    GPIO_I2C_ReleaseBus(PB4, PB5);

    i2c_state = I2C_STATE_ERROR;

}




