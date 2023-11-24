/*
 * snelheidBehouden.c
 *
 *  Created on: 23 Nov 2023
 *      Author: Tommy
 */

#include "snelheidBehouden.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "xgpio.h"
#include "xtmrctr.h"
#include "xparameters.h"
#include <stdbool.h>
#include "xtime_l.h"

#define TIME_TO_MS_DIVIDER  500000

#define MASK_ENCODER_INPUT 0xF

#define ENCODER_COUNT 4
enum encoder {
	ENCODER_1 = 0x1,
	ENCODER_2 = 0x2,
	ENCODER_3 = 0x4,
	ENCODER_4 = 0x8,
};


void onInterrupt() {
	xil_printf("Interrupt received\r\n");
}

/*
// 	int Status;
// 	XGpio GPIO;
// 	XScuGic_Config *IntcConfig;
// 	XScuGic IntcInstance;

// 	IntcConfig = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);

// 	Status = XScuGic_CfgInitialize(&IntcInstance, IntcConfig, IntcConfig->CpuBaseAddress);
// 	if (Status != XST_SUCCESS) {
// 		xil_printf("Error initializing interrupt controller\r\n");
// 	}

// 	// // Connect to GPIO
// 	// Status = XGpio_Initialize(&GPIO, XPAR_GPIO_2_DEVICE_ID);
// 	// if (Status != XST_SUCCESS) {
// 	// 	xil_printf("Error initializing GPIO\r\n");
// 	// }
	
//     // XGpio_SetDataDirection(&GPIO, 1, 0x1);

// 	// loopup config for gpio
// 	XGpio_Config *gpio_config;
// 	gpio_config = XGpio_LookupConfig(XPAR_GPIO_0_DEVICE_ID);

// 	// initialize gpio
// 	Status = XGpio_CfgInitialize(&GPIO, gpio_config, gpio_config->BaseAddress);

// 	// set direction of gpio
// 	XGpio_SetDataDirection(&GPIO, 1, 0xF);

// 	// connect gpio interrupt to handler
// 	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
// 			(Xil_ExceptionHandler)XScuGic_InterruptHandler,
// 			&IntcInstance);
// //	if (Status != XST_SUCCESS) {
// //		xil_printf("Error connecting interrupt to handler\r\n");
// //	}

// 	// connect interrupt to gpio
// 	Status = XScuGic_Connect(&IntcInstance, XPAR_GPIO_0_INTERRUPT_PRESENT,
// 			(Xil_ExceptionHandler)onInterrupt, (void *)&GPIO);
// 	if (Status != XST_SUCCESS) {
// 		xil_printf("Error connecting interrupt to gpio\r\n");
// 	}


// 	// enable interrupt
// 	XScuGic_Enable(&IntcInstance, XPAR_FABRIC_ARDUINO_ARDUINO_INTR_EN_PINS_2_3_IP2INTC_IRPT_INTR);

// 	// enable interrupt on gpio
// 	XGpio_InterruptEnable(&GPIO, 0xF);
// 	XGpio_InterruptGlobalEnable(&GPIO);

// 	// enable interrupt on interrupt controller
// 	Xil_ExceptionEnable();
// 	xil_printf("Successfully init snelheidBehouden\r\n");
*/

XGpio encoderInput, ledOutput;

uint32_t encoderPulseTime[4] = {0};
uint32_t old_encoderTime[4] = {0};
 
void processEncoder(uint8_t encoderInputValue, uint8_t encoderMask, uint8_t encoderIndex) {
	// Check if the encoder has changed
	static uint8_t oldInput = 0;
	if ((encoderInputValue & encoderMask) != (oldInput & encoderMask)) {
		// Rising edge
		if ((encoderInputValue & encoderMask) && !(oldInput & encoderMask)) {
			// Get the time
			XTime now_time;
			XTime_GetTime(&now_time);

			// Calculate the time between pulses
			encoderPulseTime[encoderIndex] = now_time / TIME_TO_MS_DIVIDER - old_encoderTime[encoderIndex];
			old_encoderTime[encoderIndex] = now_time / TIME_TO_MS_DIVIDER;

			// Print the time of the encoder
			xil_printf("Encoder %d: %llu\r\n", encoderIndex, encoderPulseTime[encoderIndex]);
		}

		// Update old output
		oldInput = encoderInputValue; 
	}
}

void init_snelheidBehouden() {
	// init gpio
	XGpio_Initialize(&encoderInput, XPAR_ARDUINO_ARDUINO_NO_INTR_PINS_DEVICE_ID);
	XGpio_Initialize(&ledOutput, XPAR_USER_LEDS_GPIO_DEVICE_ID);
	
	// set direction of gpio
	XGpio_SetDataDirection(&encoderInput, 1, 0xF);
	XGpio_SetDataDirection(&ledOutput, 1, 0x0);
}
		

void snelheidBehouden(uint8_t* speedLeft, uint8_t* speedRight) {
	// Read the encoder input
	uint8_t encoderInputValue = XGpio_DiscreteRead(&encoderInput, 1);
	
	// Check if the encoder has changed for each encoder and calculate the time between pulses
	for (uint8_t i = 0; i < ENCODER_COUNT; i++) {
		processEncoder(encoderInputValue, 1 << i, i);
	}

	// uint8_t test = 0x0;
	// test |= ((encoderInputValue & ENCODER_2) << 1);
	XGpio_DiscreteWrite(&ledOutput, 1, encoderInputValue);


// --- temp ---
	// if ((XGpio_DiscreteRead(&encoderInput, 0x1) & ENCODER_INPUT_MASK) >= 0x1  ) {
		
	// 	*speedLeft = 10;
	// 	*speedRight = 10;
	// }
	// else {
	// 	XGpio_DiscreteWrite(&ledOutput, 1, 0x0);
	// 	*speedLeft = 100;
	// 	*speedRight = 100;
	// }
}


