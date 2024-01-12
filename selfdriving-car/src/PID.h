#ifndef _PID_H_
#define _PID_H_

#include <stdint.h>
#include "defines.h"
#include "xtime_l.h"


typedef struct PIDStruct {
    uint32_t kp;            // proportional gain
    uint32_t ki;            // integral gain
    uint32_t kd;            // derivative gain

    int32_t max_value;      // maximum output value
    int32_t min_value;      // minimum output value

    uint16_t multiplication_factor; // multiplication factor for the PID calculations

    XTime last_time;        // last time the PID was calculated
    int32_t last_error;     // last error value
    int32_t integral;       // integral value
    int32_t derivative;     // derivative value
} pid_struct;   // PID struct

void pid_init(pid_struct* pid, uint32_t kp, uint32_t ki, uint32_t kd, uint16_t max_value, uint16_t min_value, uint16_t multiplication_factor);
uint16_t pid_calculate(pid_struct* pid, int16_t error, uint16_t setpoint);

#endif