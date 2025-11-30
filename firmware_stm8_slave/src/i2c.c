#include "i2c.h"
#include "clock.h"
#include "gpio.h"
#include "uart.h"   // only for debug prints 
#include "tim4.h"

#define I2C_TIMEOUT_MS 10

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


void I2C_Start(void)
{
    I2C->CR2 |= I2C_CR2_START;
    // while(!(I2C->SR1 & I2C_SR1_SB));  // Wait START
    UART1_WriteString("CR2 register set for start");
}

void I2C_Stop(void)
{
    I2C->CR2 |= I2C_CR2_STOP;
}

void I2C_Write(uint8_t data)
{
    I2C->DR = data;
    
    uint32_t timeout = 50000;
    while (timeout--)
    {
        if (I2C->SR1 & I2C_SR1_TXE)
        return;
        
        if (I2C->SR2 & I2C_SR2_AF)
        {
            I2C->SR2 &= ~I2C_SR2_AF; // clear AF
            return;
        }
    }
    
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

void I2C_Task(void) {
    UART1_WriteString("STATE = ");
    UART1_WriteHex8(i2c_state);
    UART1_WriteString("\r\n");
    tim4_delay(10);

    switch (i2c_state){
        case I2C_STATE_IDLE:
            //nothing to do
            break;

        case I2C_STATE_START:
            //Generate Start
            I2C_Start();
            i2c_deadline = millis() + I2C_TIMEOUT_MS;
            i2c_state = I2C_STATE_WAIT_SB;
            break;

        case I2C_STATE_WAIT_SB:
            if(I2C->SR1 & I2C_SR1_SB) {
                i2c_state = I2C_STATE_SEND_ADDR;
            } else if(i2c_timed_out()) {
                i2c_last_error = I2C_ERR_TIMEOUT;
                i2c_state = I2C_STATE_ERROR;
            }
            break;

        case  I2C_STATE_SEND_ADDR:
            I2C->DR = (i2c_addr << 1); //write mode
            i2c_deadline = millis() + I2C_TIMEOUT_MS;
            i2c_state = I2C_STATE_WAIT_ADDR;
            break;

        case I2C_STATE_WAIT_ADDR:
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
            I2C->DR = i2c_tx_buf[i2c_tx_pos++];
            i2c_deadline = millis() + I2C_TIMEOUT_MS;
            i2c_state = I2C_STATE_WAIT_TXE;
            break;

        case I2C_STATE_WAIT_TXE:
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
            I2C->CR2 |= I2C_CR2_STOP;
            i2c_state = I2C_STATE_DONE;
            break;

        case I2C_STATE_DONE:
            // transaction completed successfully
            i2c_state = I2C_STATE_IDLE;
            break;

        case I2C_STATE_ERROR:
            // try stopping bus
            I2C->CR2 |= I2C_CR2_STOP;
            i2c_state = I2C_STATE_IDLE;
            break;
    }
}
