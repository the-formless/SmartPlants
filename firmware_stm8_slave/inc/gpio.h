#ifndef STM8_GPIO_H
#define STM8_GPIO_H

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

//HAL Enums
typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT = 1
} GPIO_Mode;

typedef enum {
    GPIO_NOPULL = 0,
    GPIO_PULLUP = 1
} GPIO_Pull;

typedef enum {
    GPIO_OUTPUT_PUSHPULL = 0,
    GPIO_OUTPUT_OPENDRAIN = 1
} GPIO_OutputType;

typedef enum {
    GPIO_SPEED_SLOW = 0,
    GPIO_SPEED_FAST = 1 
} GPIO_Speed;

//-----
//GPIO Initialization Config
//-----
typedef struct {
    // uint8_t pin;    //defining PINs separately for better readability
    GPIO_Mode mode;
    GPIO_Pull pull;
    GPIO_OutputType outputType; // Only relevant if mode is OUTPUT
    GPIO_Speed speed;           // Only relevant if mode is OUTPUT
} GPIO_InitTypeDef;


typedef struct {
    volatile GPIO_Port *port;
    uint8_t pin;
} GPIO_Pin;

//-----
//GPIO Pin defs
//-----
#define PA1 ((GPIO_Pin){GPIOA, 1})
#define PA2 ((GPIO_Pin){GPIOA, 2})
#define PA3 ((GPIO_Pin){GPIOA, 3})
#define PB4 ((GPIO_Pin){GPIOB, 4})
#define PB5 ((GPIO_Pin){GPIOB, 5})
#define PC3 ((GPIO_Pin){GPIOC, 3})
#define PC4 ((GPIO_Pin){GPIOC, 4})
#define PC5 ((GPIO_Pin){GPIOC, 5})
#define PC6 ((GPIO_Pin){GPIOC, 6})
#define PC7 ((GPIO_Pin){GPIOC, 7})
#define PD3 ((GPIO_Pin){GPIOD, 3})
#define PD2 ((GPIO_Pin){GPIOD, 2})
#define PD4 ((GPIO_Pin){GPIOD, 4})
#define PD5 ((GPIO_Pin){GPIOD, 5})
#define PD6 ((GPIO_Pin){GPIOD, 6})

//-----
//GPIO HAL Functions
//-----
void GPIO_InitPin(volatile GPIO_Pin pin, GPIO_InitTypeDef *config);
// void GPIO_WritePin(volatile GPIO_Port * port, uint8_t pin, uint8_t value);
// uint8_t GPIO_ReadPin(volatile GPIO_Port * port, uint8_t pin);
// void GPIO_TogglePin(volatile GPIO_Port * port, uint8_t pin);
void GPIO_WritePin(volatile GPIO_Pin pin, uint8_t value);
uint8_t GPIO_ReadPin(volatile GPIO_Pin pin);
void GPIO_TogglePin(volatile GPIO_Pin pin);

void GPIO_WritePort(volatile GPIO_Port * port, uint8_t value);
uint8_t GPIO_ReadPort(volatile GPIO_Port * port);   

#endif