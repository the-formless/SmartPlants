#ifndef SCHEDULER_H
#define SCHEDULER_H
  
#include <stdint.h>

  typedef void (*TaskFn)(void);

  typedef struct {
    TaskFn task;
    uint32_t period_ms;
    uint32_t next_run;
  } Task;

  void Scheduler_Init(void);
  void Scheduler_Run(void);

  //Tasks list
  extern Task task_list[];
  extern const uint8_t TASK_COUNT;

#endif