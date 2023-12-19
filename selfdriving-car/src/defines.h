#include <stdint.h>
#ifndef _DEFINES_H_
#define _DEFINES_H_

typedef struct GlobalData
{
    uint8_t speedLeft;
    uint8_t speedRight;
    uint8_t speedBase;
    uint8_t turnValue;
} globalData;

// --- Time constants ---
#define TIME_TO_NS_DIVIDER 325 //XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2000000
#define TIME_TO_NS(i) (i / TIME_TO_NS_DIVIDER)
#define NS_TO_TIME(i) (i * TIME_TO_NS_DIVIDER)

// --- Encoder calculations ---
// time between pulses on 6v supply = 15.48ms
// pulses per revolution = 20
// revolutions times 6v supply = 309.6ms
#define TIRE_LENGTH_MM          206   // mm
#define ENCODER_DISK_SLOTS      20    // slots
#define MIN_PULSE_TIME          15480 // ns
#define ENCODER_COUNT			2

// --- Speed limits ---
#define MAX_MAX_SPEED_VALUE      200  // maximum allowed speed value
#define NORMAL_MAX_SPEED_VALUE   100  // 100% speed value
#define MIN_SPEED_VALUE          0
#define DEFAULT_SPEED            50   // value between MIN_SPEED_VALUE and NORMAL_MAX_SPEED_VALUE
// -- Speed calculations --
#define DISTANCE_PER_PULSE  TIRE_LENGTH_MM * 10 / ENCODER_DISK_SLOTS  	// DISTANCE_PER_PULSE / 10 = mm per pulse
#define MAX_SPEED  TIRE_LENGTH_MM * 1000000 / ENCODER_DISK_SLOTS / MIN_PULSE_TIME // 665.37 mm/s = 2.395 km/h

// --- Turn value ---
#define DEFAULT_TURN_VALUE      128
#define FULL_RIGHT_TURN_VALUE   255
#define FULL_LEFT_TURN_VALUE    0

// --- Loop times ---
#define SPEED_CALC_LOOP_TIME  1000  // ns

// --- PWM frequency ---
#define PWM_FREQ    500000   // ns 
#define WAIT_TIME   10000000 // ns

#endif // _DEFINES_H_
