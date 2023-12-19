/*
 * obstakeldetectie.c
 *
 *  Created on: 23 Nov 2023
 *      Author: Jochem
 */

#include "obstakeldetectie.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "xil_exception.h"
#include "xiicps.h"
#include "xstatus.h"

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

	status = XIicPs_CfgInitialize(&iic, config, config->BaseAddress);
	if (status != XST_SUCCESS) {
		xil_printf("Error: XIicPs_CfgInitialize()\n\r");
		return XST_FAILURE;
	}

	status = XIicPs_SelfTest(&iic);
	if (status != XST_SUCCESS) {
		xil_printf("Error: XIicPs_SelfTest()\n\r");
		return XST_FAILURE;
	}

	XIicPs_SetSClk(&iic, IIC_CLOCK_SPEED);

	const int TEST_BUFFER_SIZE = 132;
	u8 SendBuffer[TEST_BUFFER_SIZE];
	u8 RecvBuffer[TEST_BUFFER_SIZE];

	for (int Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		SendBuffer[Index] = (Index % TEST_BUFFER_SIZE);
		RecvBuffer[Index] = 0;
	}

	status = XIicPs_MasterSendPolled(&iic, SendBuffer,
					 TEST_BUFFER_SIZE, IIC_SLAVE_ADDR);
	if (status != XST_SUCCESS) {
		xil_printf("Error: XIicPs_MasterSendPolled()\n\r");
		return XST_FAILURE;
	}

	while (XIicPs_BusIsBusy(&iic)) {
		/* NOP */
	}

	return XST_SUCCESS;
}

void obstakeldetectie() {
	//	Do something here
}
