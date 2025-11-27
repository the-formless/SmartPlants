#ifndef GPIO_H
#define GPIO_H

#include <stdint.h> 

//GPIO Register STruct

typedef struct
{
  volatile uint8_t ODR;   // Output Data Register
  volatile uint8_t IDR;   // Input Data Register
  volatile uint8_t DDR;   // Data Direction Register
  volatile uint8_t CR1;   // Control Register 1
  volatile uint8_t CR2;   // Control Register 2
} GPIO_Port;

//--
//Base Addresses
//--
#define GPIOA ((volatile GPIO_Port*) 0x5000)
#define GPIOB ((volatile GPIO_Port*) 0x5005)
#define GPIOC ((volatile GPIO_Port*) 0x500A)
#define GPIOD ((volatile GPIO_Port*) 0x500F)
#define GPIOE ((volatile GPIO_Port*) 0x5014)
#define GPIOF ((volatile GPIO_Port*) 0x5019) 

//helper macros
#define GPIO_PIN(n) (1 << (n))

#endif