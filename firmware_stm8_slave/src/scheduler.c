#include "scheduler.h"
#include "tim4.h" 

// ---------------
// DEFINE TASKS IN THIS FILE
// ---------------

// Forward declare tasks
void LCD_InitTask(void);
void LCD_Task(void);
void UART_Task(void);
void CommandTask(void);
void SensorTask(void);
void PumpTask(void);
void SafetyTask(void);
void HeartbeatTask(void);
void I2C_TimeoutTask(void);

// Define task table (tune periods later)
Task task_list[] = {
    // { LCD_InitTask,      1, 0 },   // run every 1ms
    // { LCD_Task,          1, 0 },
    { UART_Task,         1, 0 },
    // { CommandTask,       1, 0 },

    // { SensorTask,      100, 0 },
    // { PumpTask,         10, 0 },
    // { SafetyTask,      100, 0 },

    { HeartbeatTask,   500, 0 },   // LED blink / debug
    // { I2C_TimeoutTask,  10, 0 },
};

const uint8_t TASK_COUNT = sizeof(task_list) / sizeof(Task);

// ----------------
// IMPLEMENTATION
// ----------------

void Scheduler_Init(void)
{
    uint32_t now = millis();

    for (uint8_t i = 0; i < TASK_COUNT; i++) {
        task_list[i].next_run = now + task_list[i].period_ms;
    }
}

void Scheduler_Run(void)
{
    uint32_t now = millis();

    for (uint8_t i = 0; i < TASK_COUNT; i++)
    {
        if ((int32_t)(now - task_list[i].next_run) >= 0)
        {
            task_list[i].next_run = now + task_list[i].period_ms;

            // cooperative — never block here!
            task_list[i].task();
        }
    }
}