#include "gpio.h"

void GPIO_InitPin(GPIO_Pin pin , GPIO_InitTypeDef * config) {
    
    volatile GPIO_Port * port = pin.port;
    uint8_t pin_mask = GPIO_PIN(pin.pin);

    //--    
    //set mode: input/output
    //--
    if (config->mode == GPIO_MODE_OUTPUT) {
        port->DDR |= pin_mask; //set as output
    } else {
        port->DDR &= ~pin_mask; //set as input
    }

    //----  
    //set pull-up for input
    //push-pull/Open-drain fot output
    //----
    // -------------------------
    // CR1 (Pull-up OR Push-pull)
    // -------------------------
    if (config->mode == GPIO_MODE_INPUT)
    {
        // Input mode
        if (config->pull == GPIO_PULLUP)
            port->CR1 |= pin_mask;
        else
            port->CR1 &= ~pin_mask;
    }
    else
    {
        // Output mode
        if (config->outputType == GPIO_OUTPUT_PUSHPULL)
            port->CR1 |= pin_mask;
        else
            port->CR1 &= ~pin_mask;
    }

    //----
    //Speed: Slow/Fast
    //----
    if(config -> speed == GPIO_SPEED_FAST) {
        port -> CR2 |= pin_mask; //Fast
    } else {
        port -> CR2 &= ~pin_mask; //Slow
    }
}

//---------
//Write Pin
//---------
void GPIO_WritePin(GPIO_Pin pin, uint8_t value) {

    if(value) {
        pin.port->ODR |= GPIO_PIN(pin.pin); //set pin high
    } else {
        pin.port->ODR &= ~GPIO_PIN(pin.pin); //set pin low
    }
}

//---------
//toggle pin
//---------
void GPIO_TogglePin(GPIO_Pin pin) {
    pin.port->ODR ^= GPIO_PIN(pin.pin);
}

//---------
//Read Pin
//---------
uint8_t GPIO_ReadPin(GPIO_Pin pin) {
    return (pin.port->IDR & GPIO_PIN(pin.pin)) ? 1 : 0;
}

//---------
//Port read/Write
//---------
void GPIO_WritePort(volatile GPIO_Port *port, uint8_t value) {
    port->ODR = value;
}

uint8_t GPIO_ReadPort(volatile GPIO_Port *port) {
    return port->IDR;
}

/** Notes & Correct Behavior

STM8 CR1 acts as Pull-up (for input) and Push-pull (for output)

CR2 is fast mode, only valid in output mode

DDR is direction

ODR is used for writes */

/**
 PB$ and PB5 are:
✔ True open-drain only
✔ No push-pull driver
✔ They cannot drive the pin HIGH
✔ They can only pull LOW
✔ To turn LED ON, you must pull the pin LOW
✔ must have an external pull-up or LED connected to VDD 
*/