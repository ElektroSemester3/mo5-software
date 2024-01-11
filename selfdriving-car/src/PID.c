/*
 * PID.c
 *
 *  Created on: 18 Dec 2023
 *  Author: Tommy de Wever
 */

#include "PID.h"

// --- limits ---
#define MAX_OF(type) \
    (type)(((type)(~0LLU) > (type)((1LLU<<((sizeof(type)<<3)-1))-1LLU)) ? (long long unsigned int)(type)(~0LLU) : (long long unsigned int)(type)((1LLU<<((sizeof(type)<<3)-1))-1LLU))
#define MIN_OF(type) \
    (type)(((type)(1LLU<<((sizeof(type)<<3)-1)) < (type)1) ? (long long int)((~0LLU)-((1LLU<<((sizeof(type)<<3)-1))-1LLU)) : 0LL)

// --- Function declarations ---
static int32_t applyLimits(int32_t value, int32_t min_value, int32_t max_value);

// --- Function definitions ---

/**
 * Initializes the PID controller with the specified parameters.
 *
 * @param pid The PID controller structure to initialize.
 * @param kp The proportional gain.
 * @param ki The integral gain.
 * @param kd The derivative gain.
 * @param max_value The maximum output value of the controller.
 * @param min_value The minimum output value of the controller.
 * @param multiplication_factor The multiplication factor wat is used on the PID values.
 */
void pid_init(pid_struct* pid, uint32_t kp, uint32_t ki, uint32_t kd, uint16_t max_value, uint16_t min_value, uint16_t multiplication_factor) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->max_value = max_value;
    pid->min_value = min_value;

    pid->multiplication_factor = multiplication_factor;

    pid->last_time = 0;
    pid->last_error = 0;
    pid->integral = 0;
    pid->derivative = 0;
}

/**
 * Calculates the output of a PID controller based on the given error and setpoint.
 *
 * @param pid The PID controller structure.
 * @param error The current error value.
 * @param setpoint The desired setpoint value.
 * @return The calculated output of the PID controller.
 */
uint16_t pid_calculate(pid_struct* pid, int16_t error, uint16_t setpoint) {
    // Calculate the time difference
    XTime time_now = 0;
    XTime_GetTime(&time_now);
    uint32_t time_diff = TIME_TO_NS(time_now) - pid->last_time;
    pid->last_time = TIME_TO_NS(time_now);

    // Reset the integral if the setpoint is 0
    if (setpoint == 0) {
        pid->integral = 0;
    }

    // Calculate the integral
    typeof(pid->integral) temp_integral = error * (int64_t)time_diff / (int64_t)1e3;
    // prevent overflow 
    if (temp_integral > MAX_OF(typeof(pid->integral))) {
        pid->integral = MAX_OF(typeof(pid->integral));
    } else if (temp_integral < MIN_OF(typeof(pid->integral))) {
        pid->integral = MIN_OF(typeof(pid->integral));
    } else {
        pid->integral += temp_integral;
    }

    // Calculate the derivative
    pid->derivative = (int64_t)(error - pid->last_error) * (int64_t)(1e9) / (int64_t)time_diff;

    // Update the last error
    pid->last_error = error;

    // Calculate the output
    int32_t output = ((int64_t)pid->kp * error / (int32_t)pid->multiplication_factor)
    + ((int64_t)pid->ki * pid->integral / ((int32_t)pid->multiplication_factor * (int32_t)1e3))
    + ((int64_t)pid->kd * pid->derivative / (int32_t)pid->multiplication_factor);

    // Apply limits to the output value
    output = applyLimits(output, pid->min_value, pid->max_value);

    // Return the output value
    return output;
}

/**
 * Applies limits to a given value.
 *
 * @param value The value to be limited.
 * @param min_value The minimum allowed value.
 * @param max_value The maximum allowed value.
 * @return The limited value.
 */
static int32_t applyLimits(int32_t value, int32_t min_value, int32_t max_value) {
    if (value > max_value) {
        return max_value;
    } else if (value < min_value) {
        return min_value;
    } else {
        return value;
    }
}