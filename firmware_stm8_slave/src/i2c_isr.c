#include "i2c.h"
#include "tim4.h"


void I2C_IRQHandler(void) __interrupt(19) 
{
    if (!i2c_txn_active)
        return; 

    uint8_t sr1 =  I2C->SR1;
    uint8_t sr2 =  I2C->SR2;
    uint8_t sr3 =  I2C->SR3;    // reading SR3 is needed to clear ADDR when combined with SR1

    (void)sr3; // avoid unused warning if not always used

    i2c_last_activity_ms = millis();

    //error check
    if (sr2 & I2C_SR2_AF) {
        // NACK received
        I2C->SR2 &= (uint8_t)~I2C_SR2_AF;
        I2C->CR2 |= I2C_CR2_STOP;   // send STOP
        i2c_state = I2C_STATE_ERROR;
        return;
    }

    switch (i2c_state){

        case I2C_STATE_START:
            //Wait for SB
            if(sr1 & I2C_SR1_SB)
            {
                // Writing DR sends address
                // LSB = R/W bit (0 = write)
                I2C->DR = (uint8_t)(i2c_txn.address << 1); // write mode
                i2c_state = I2C_STATE_ADDR;
            }
            break;

        case I2C_STATE_ADDR:
            // ADDR set when address phase done & ACK received
            if (sr1 & I2C_SR1_ADDR)
            {
                // Clear ADDR by reading SR1 then SR3 (already read above)
                (void)I2C->SR1;
                (void)I2C->SR3;

                if (i2c_txn.length > 0) {
                    // Write first data byte
                    I2C->DR = i2c_txn.data[i2c_txn.index++];
                    i2c_state = I2C_STATE_TX;
                } else {
                    // Nothing to send, just stop
                    I2C->CR2 |= I2C_CR2_STOP;
                    i2c_state = I2C_STATE_STOP;
                }
            }
            break;

        case I2C_STATE_TX:
            // TXE = data register empty → ready for next byte or STOP
            if (sr1 & I2C_SR1_TXE)
            {
                if (i2c_txn.index < i2c_txn.length) {
                    I2C->DR = i2c_txn.data[i2c_txn.index++];
                } else {
                    // All bytes sent → STOP
                    I2C->CR2 |= I2C_CR2_STOP;
                    i2c_state = I2C_STATE_STOP;
                }
            }
            break;

        case I2C_STATE_STOP:
            // We don’t get a specific STOP flag here.
            // Once we requested STOP, we consider transaction done.
            i2c_state = I2C_STATE_IDLE;
            i2c_txn_active = 0;   // <-- NOW we mark as inactive
            break;

        case I2C_STATE_IDLE:
        case I2C_STATE_ERROR:
        default:
            // Nothing to do, or error handled elsewhere
            break;
    }

}
