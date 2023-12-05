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

#define TIME_TO_NS_DIVIDER 325 //XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2000000
#define TIME_TO_NS(i) (i / TIME_TO_NS_DIVIDER)
#define NS_TO_TIME(i) (i * TIME_TO_NS_DIVIDER)

#define MASK_ENCODER_INPUT 0xF

// time between pulses on 6v supply = 15.48ms
// pulses per revolution = 20
// revolutions times 6v supply = 309.6ms
#define TIRE_LENGTH_MM  	206
#define ENCODER_DISK_SLOTS	20
#define MIN_PULSE_TIME		15480 // ns

const uint16_t DISTANCE_PER_PULSE = TIRE_LENGTH_MM * 10 / ENCODER_DISK_SLOTS;  	// DISTANCE_PER_PULSE / 10 = mm per pulse
const uint16_t MAX_SPEED = TIRE_LENGTH_MM * 1000000 / ENCODER_DISK_SLOTS / MIN_PULSE_TIME; // 665.37 mm/s = 2.395 km/h


#define ENCODER_COUNT 4
enum encoder {
	ENCODER_1 = 0x1,
	ENCODER_2 = 0x2,
	ENCODER_3 = 0x4,
	ENCODER_4 = 0x8,
};

#define LOOP_TIME 100000 // ns

const uint8_t MAX_SPEED_VALUE = 200;
const uint8_t MIN_SPEED_VALUE = 0;

// void onInterrupt() {
// 	xil_printf("Interrupt received\r\n");
// }

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

/**
 * @brief Process the encoder input value.
 * 
 * This function takes the encoder input value, encoder mask, and encoder index as parameters
 * and performs the necessary processing on the input value.
 * 
 * @param encoderInputValue The input value from the encoder.
 * @param encoderMask The mask used to extract relevant bits from the input value.
 * @param encoderIndex The index of the encoder.
 */
void processEncoder(uint8_t encoderInputValue, uint8_t encoderMask, uint8_t encoderIndex, uint16_t speed[]) {
	static XTime encoderPulseTime[ENCODER_COUNT] = {0};
	static XTime old_encoderTime[ENCODER_COUNT] = {0};
	static uint8_t oldInput = 0;
	// Check if the encoder has changed
	if ((encoderInputValue & encoderMask) != (oldInput & encoderMask)) {
		// Rising edge
		if ((encoderInputValue & encoderMask) && !(oldInput & encoderMask)) {
			// Get the time
			XTime now_time;
			XTime_GetTime(&now_time);

			// Calculate the time between pulses
			encoderPulseTime[encoderIndex] = TIME_TO_NS(now_time) - old_encoderTime[encoderIndex];
			old_encoderTime[encoderIndex] = TIME_TO_NS(now_time);

			speed[encoderIndex] = (uint32_t)(DISTANCE_PER_PULSE * 100000) / encoderPulseTime[encoderIndex];

			// // Calculate the speed in mm/s
			// uint16_t _speed = (uint32_t)(DISTANCE_PER_PULSE * 100000) / encoderPulseTime[encoderIndex];

			// int16_t error = 100 - (uint32_t)((_speed * 100) / MAX_SPEED);

			// if (error < -100) {
			// 	error = -100;
			// } else if (error > 100) {
			// 	error = 100;
			// }

			// uint16_t newSpeed = 100 + error;

			// // Print the time of the encoder
			// xil_printf("Encoder %d: Current speed: %d | Error: %d | New speed: %d\r\n", encoderIndex, _speed, error, newSpeed);

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

/**
 * Applies limits to a given value.
 *
 * This function takes a value and applies limits to it, ensuring that it does not exceed
 * the specified minimum and maximum values.
 *
 * @param value The value to be limited.
 * @param min_value The minimum allowed value.
 * @param max_value The maximum allowed value.
 * @return The limited value.
 */
int16_t applyLimits(int16_t value, int16_t min_value, int16_t max_value){
	if (value < min_value){
		return min_value;
	} else if (value > max_value){
		return max_value;
	} else {
		return value;
	}
}


void adjustSpeed(uint8_t* setpointLeft, uint8_t* setpointRight, uint16_t speed[]) {
	// calculate the speed over the 2 sensors
	// uint16_t averageSpeedLeft = (uint32_t)(speed[0] + speed[1]) / 2;
	// uint16_t averageSpeedRight = (uint32_t)(speed[2] + speed[3]) / 2;
	uint16_t averageSpeedLeft = speed[0];
	uint16_t averageSpeedRight = speed[2];

	uint8_t oldSetPointLeft = *setpointLeft;
	uint8_t oldSetPointRight = *setpointRight;

	// calculate the error value referd to the setpoint
	int16_t errorLeft = (uint32_t)((averageSpeedLeft * 100) / MAX_SPEED);
	int16_t errorRight = (uint32_t)((averageSpeedRight * 100) / MAX_SPEED);

	// calculate the new speed 
	uint16_t speedLeftNew = *setpointLeft - applyLimits(errorLeft, MIN_SPEED_VALUE, MAX_SPEED_VALUE - *setpointLeft);
	uint16_t speedRightNew = *setpointRight - applyLimits(errorRight, MIN_SPEED_VALUE, MAX_SPEED_VALUE - *setpointRight);

	*setpointLeft = speedLeftNew;
	*setpointRight = speedRightNew;

	// xil_printf("SP: %d | %d \t--\t AVG:  %d | %d \t--\t Error: %d | %d \t--\t Left: %d | Right: %d\r\n", oldSetPointLeft, oldSetPointRight, averageSpeedLeft, averageSpeedRight, errorLeft, errorRight, speedLeftNew, speedRightNew);
}
		

/**
 * @brief Maintains the speed of the self-driving car.
 *
 * This function is responsible for maintaining the speed of the self-driving car.
 * It takes in the pointers to the left and right speed variables and updates them accordingly.
 *
 * @param speedLeft Pointer to the left speed variable.
 * @param speedRight Pointer to the right speed variable.
 */
void snelheidBehouden(uint8_t* speedLeft, uint8_t* speedRight) {
	// Read the encoder input
	uint8_t encoderInputValue = XGpio_DiscreteRead(&encoderInput, 1);
	static uint16_t speed[ENCODER_COUNT] = {0};
	
	// Check if the encoder has changed for each encoder and calculate the time between pulses
	for (uint8_t i = 0; i < ENCODER_COUNT; i++) {
		processEncoder(encoderInputValue, 1 << i, i, speed);
	}

	XTime time_now = 0;
	static XTime time_old = 0;
	XTime_GetTime(&time_now);
	if (time_now - time_old > NS_TO_TIME(LOOP_TIME)){
		time_old = time_now;
		adjustSpeed(speedLeft, speedRight, speed);
	}

	XGpio_DiscreteWrite(&ledOutput, 1, encoderInputValue);
}

