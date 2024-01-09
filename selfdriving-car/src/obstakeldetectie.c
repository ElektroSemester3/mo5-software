/*
 * obstakeldetectie.c
 *
 *  Created on: 23 Nov 2023
 *      Author: Jochem
 */

#include "obstakeldetectie.h"
#include "xil_printf.h"
#include "xil_exception.h"
#include "xstatus.h"
#include "VL53L0X.h"


XStatus obstakeldetectieInit() {
	int status;

	status = iic_init();
	if (status != XST_SUCCESS) {
		xil_printf("Error: iic_init()\n\r");
		return XST_FAILURE;
	}

	xil_printf("Initialized I2C\n\r");

	status = connectionCheck();
	if (status != XST_SUCCESS) {
		xil_printf("Error: connectionCheck()\n\r");
		return XST_FAILURE;
	}

	xil_printf("Ran connection check\n\r");

	status = config_init();
	if (status != XST_SUCCESS) {
		xil_printf("Error: config_init()\n\r");
		return XST_FAILURE;
	}

	xil_printf("Initialized sensor config\n\r");

	return XST_SUCCESS;
}

void obstakeldetectie() {
	uint16_t distance = 0;

	int status = measurement_start(&distance);
	if (status != XST_SUCCESS) {
		xil_printf("Error: measurement_start()\n\r");
	}

	xil_printf("Distance: %d\n\r", distance);
}
