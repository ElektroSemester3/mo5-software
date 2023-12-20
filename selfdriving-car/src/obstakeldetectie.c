/*
 * obstakeldetectie.c
 *
 *  Created on: 23 Nov 2023
 *      Author: Jochem
 */

#include "obstakeldetectie.h"
#include "xil_printf.h"
#include "xil_exception.h"
#include "xiicps.h"
#include "xstatus.h"
#include "VL53L0X.h"

#define IIC_DEVICE_ID	XPAR_PS7_I2C_1_DEVICE_ID

#define IIC_SLAVE_ADDR	0x70
#define IIC_CLOCK_SPEED	100000

XIicPs iic;

XStatus obstakeldetectieInit() {
	int status;
	XIicPs_Config *config;

	config = XIicPs_LookupConfig(IIC_DEVICE_ID);
	if (config == NULL) {
		xil_printf("Error: XIicPs_LookupConfig()\n\r");
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

	status = tuning_loadDefault();
	if (status != XST_SUCCESS) {
		xil_printf("Error: tuning_loadDefault()\n\r");
		return XST_FAILURE;
	}

	xil_printf("Loaded default tuning\n\r");

	status = calibration_start();
	if (status != XST_SUCCESS) {
		xil_printf("Error: calibration_start()\n\r");
		return XST_FAILURE;
	}

	xil_printf("Calibration completed\n\r");

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
