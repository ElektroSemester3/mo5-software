/*
 * obstakeldetectie.c
 *
 *  Created on: 23 Nov 2023
 *      Author: Jochem
 */

#include "obstakeldetectie.h"

#include "VL53L0X.h"
#include "defines.h"
#include "sleep.h"
#include "stdbool.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xstatus.h"

#define SLOWDOWN_DISTANCE 500
#define STOP_DISTANCE 100

XStatus obstakeldetectieInit() {
    int status;

    setTimeout(500);
    status = init(true);
    if (status != true) {
        xil_printf("Error: init()\n\r");
        return XST_FAILURE;
    }

    startContinuous(0);

    return XST_SUCCESS;
}

void obstakeldetectie(globalData* Data) {
    uint16_t distance = 0;

    distance = readRangeContinuousMillimeters();

    if (distance == 0) {
        return;
    } else if (distance < STOP_DISTANCE) {
        Data->speedBase = 0;
    } else if (distance < SLOWDOWN_DISTANCE) {
        uint8_t maxSpeed = NORMAL_MAX_SPEED_VALUE * (distance - STOP_DISTANCE) / (SLOWDOWN_DISTANCE - STOP_DISTANCE);
        if (Data->speedBase > maxSpeed) {
            Data->speedBase = maxSpeed;
        }
    }
}
