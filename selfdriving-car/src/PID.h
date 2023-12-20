#ifndef _PID_H_
#define _PID_H_

#include <stdint.h>
#include "defines.h"
#include "xtime_l.h"


typedef struct PIDStruct {
    uint32_t kp;
    uint32_t ki;
    uint32_t kd;

    int32_t max_value;
    int32_t min_value;

    uint16_t multiplication_factor;

    XTime last_time;
    int32_t last_error;
    int32_t integral;
    int32_t derivative;
} pid_struct;

void pid_init(pid_struct* pid, uint32_t kp, uint32_t ki, uint32_t kd, uint16_t max_value, uint16_t min_value, uint16_t multiplication_factor);
uint16_t pid_calculate(pid_struct* pid, int16_t error, uint16_t setpoint);

#endif
