#include "i2c.h"
#include "clock.h"
#include "gpio.h"
#include "uart.h"   // only for debug prints 
#include "tim4.h"


typedef enum {
    I2C_STATE_IDLE = 0,
    I2C_STATE_START,
    I2C_STATE_WAIT_SB,
    I2C_STATE_SEND_ADDR,
    I2C_STATE_WAIT_ADDR,
    I2C_STATE_SEND_BYTE,
    I2C_STATE_WAIT_TXE,
    I2C_STATE_STOP,
    I2C_STATE_DONE,
    I2C_STATE_ERROR
} I2C_State_t;

static volatile I2C_State_t i2c_state = I2C_STATE_IDLE;

static uint8_t i2c_addr = 0;
static const uint8_t *i2c_tx_buf = 0;
static uint8_t i2c_tx_len = 0;
static uint8_t i2c_tx_pos = 0;
static I2C_Error_t i2c_last_error = I2C_ERR_NONE;
static uint32_t i2c_deadline = 0;


void I2C_Start(void)
{
    UART1_WriteString("start function\r\n");
    I2C->CR2 |= I2C_CR2_START;
    UART1_WriteString("start function exit\r\n");
}

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

    uint16_t ccr = 16000000 / (freq * 2);
    I2C->CCRL = ccr & 0xFF;
    I2C->CCRH = (ccr >> 8) & 0x0F;

    I2C->TRISER = 17;

    I2C->CR1 = I2C_CR1_PE;
    I2C->CR2 |= I2C_CR2_ACK;    // <---- THIS IS THE MISSING STEP

}

uint8_t I2C_BeginWrite(uint8_t addr, const uint8_t *data, uint8_t len){
    if(i2c_state != I2C_STATE_IDLE) {
        return 0; //busy
    }

    i2c_addr = addr;
    i2c_tx_buf = data;
    i2c_tx_len = len;
    i2c_tx_pos = 0;
    i2c_last_error = I2C_ERR_NONE;

    //Kick off a transaction
    i2c_state = I2C_STATE_START;
    i2c_deadline = millis() + I2C_TIMEOUT_MS;

    return 1; //started
}

uint8_t I2C_IsBusy(void) {
    return (i2c_state != I2C_STATE_IDLE);
}

I2C_Error_t I2c_GetLastError(void) {
    return i2c_last_error;
}

static uint8_t i2c_timed_out(void) {
    uint32_t now = millis();
    // signed comparison to handle wrap (not super critical at 10ms)
    return (int32_t)(now - i2c_deadline) > 0;
}



void I2C_Stop(void)
{
    I2C->CR2 |= I2C_CR2_STOP;
}


void I2C_Task(void) {

    switch (i2c_state){
        case I2C_STATE_IDLE:
        UART1_WriteString("idle state\r\n");
            //nothing to do
            break;

        case I2C_STATE_START:
        UART1_WriteString("start state\r\n");
            //Generate Start
            I2C_Start();
            i2c_deadline = millis() + I2C_TIMEOUT_MS;
            i2c_state = I2C_STATE_WAIT_SB;
            break;

        case I2C_STATE_WAIT_SB:
        UART1_WriteString("wait sb state\r\n");
            if(I2C->SR1 & I2C_SR1_SB) {
                i2c_state = I2C_STATE_SEND_ADDR;
            } else if(i2c_timed_out()) {
                i2c_last_error = I2C_ERR_TIMEOUT;
                i2c_state = I2C_STATE_ERROR;
            }
            break;

        case  I2C_STATE_SEND_ADDR:
        UART1_WriteString("send addr state\r\n");
            I2C->DR = (i2c_addr << 1); //write mode
            i2c_deadline = millis() + I2C_TIMEOUT_MS;
            i2c_state = I2C_STATE_WAIT_ADDR;
            break;

        case I2C_STATE_WAIT_ADDR:
        UART1_WriteString("wait addr state\r\n");
            if(I2C->SR1 & I2C_SR1_ADDR) {
                volatile uint8_t dummy;
                dummy = I2C -> SR1;
                dummy = I2C -> SR3;  // reading to trigger
                (void)dummy;

                if(i2c_tx_len > 0) {
                    i2c_state = I2C_STATE_SEND_BYTE;
                } else {
                    i2c_state = I2C_STATE_STOP;
                }
            } else if(I2C->SR2 & I2C_SR2_AF) {      //NACK
                I2C->SR2 &= ~I2C_SR2_AF;
                i2c_last_error = I2C_ERR_NACK;
                i2c_state = I2C_STATE_ERROR;
            } else if(i2c_timed_out()) {
                i2c_last_error = I2C_ERR_TIMEOUT;
                i2c_state = I2C_STATE_ERROR;
            }
            break;

        case I2C_STATE_SEND_BYTE:
        UART1_WriteString("send byte state\r\n");
            I2C->DR = i2c_tx_buf[i2c_tx_pos++];
            i2c_deadline = millis() + I2C_TIMEOUT_MS;
            i2c_state = I2C_STATE_WAIT_TXE;
            break;

        case I2C_STATE_WAIT_TXE:
        UART1_WriteString("wait txe state\r\n");
            if (I2C->SR1 & I2C_SR1_TXE) {
                if (i2c_tx_pos < i2c_tx_len) {
                    i2c_state = I2C_STATE_SEND_BYTE;
                } else {
                    i2c_state = I2C_STATE_STOP;
                }
            } else if (I2C->SR2 & I2C_SR2_AF) {
                I2C->SR2 &= ~I2C_SR2_AF;
                i2c_last_error = I2C_ERR_NACK;
                i2c_state = I2C_STATE_ERROR;
            } else if (i2c_timed_out()) {
                i2c_last_error = I2C_ERR_TIMEOUT;
                i2c_state = I2C_STATE_ERROR;
            }
            break;

        case I2C_STATE_STOP:
        UART1_WriteString("stop state\r\n");
            I2C->CR2 |= I2C_CR2_STOP;
            i2c_state = I2C_STATE_DONE;
            break;

        case I2C_STATE_DONE:
        UART1_WriteString("done state\r\n");
            // transaction completed successfully
            i2c_state = I2C_STATE_IDLE;
            break;

        case I2C_STATE_ERROR:
        UART1_WriteString("error state\r\n");
            // try stopping bus
            I2C->CR2 |= I2C_CR2_STOP;
            i2c_state = I2C_STATE_IDLE;
            break;
    }
}
