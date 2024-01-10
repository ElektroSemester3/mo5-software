/*
 * obstakeldetectie.c
 *
 *  Created on: 23 Nov 2023
 *      Author: Jochem
 */

#include "obstakeldetectie.h"

#include "VL53L0X.h"
#include "sleep.h"
#include "stdbool.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xstatus.h"

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

void obstakeldetectie() {
    uint16_t distance = 0;

    distance = readRangeContinuousMillimeters();

    if (timeoutOccurred()) {
        xil_printf("Timeout\n\r");
    } else {
        xil_printf("Distance: %d\n\r", distance);
    }

    usleep(100000);
}
