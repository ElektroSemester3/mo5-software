/*
 * motorAansturing.c
 *
 *  Created on: 23 Nov 2023
 *  Author: Tommy de Wever
 */
#include "motorAansturing.h"
#include "xil_exception.h"
#include "xtmrctr.h"
#include "sleep.h"

// ##################################################
// # TODO: remove delay								#
// # TODO: try to remove disable/enable PWM			#
// ##################################################

// --- PWM timers constants ---
#define PWM_TIMER_LEFT	XPAR_MOTOR_DRIVER_AXI_TIMER_0_DEVICE_ID
#define PWM_TIMER_RIGHT XPAR_MOTOR_DRIVER_AXI_TIMER_1_DEVICE_ID

// --- Global variables ---
XTmrCtr timerLeft, timerRight;

// --- Function declarations ---
static XStatus PwmInit(XTmrCtr *TmrCtrInstancePtr, uint8_t TmrInstanceNr);
static void PwmConfig(XTmrCtr *TmrCtrInstancePtr, uint32_t Period, uint32_t HighTime);
static uint8_t applyPWMLimits(uint8_t value);

/**
 * Initializes the motor control.
 *
 * @return XStatus - The status of the initialization process.
 */
XStatus init_motorAansturing(){
	// Initialize PWM for the left motor
	int status_L = PwmInit(&timerLeft, PWM_TIMER_LEFT);
	if (status_L != XST_SUCCESS){
		return XST_FAILURE;
	}

	// Initialize PWM for the right motor
	int status_R = PwmInit(&timerRight, PWM_TIMER_RIGHT);
	if (status_R != XST_SUCCESS){
		return XST_FAILURE;
	}

	// success
	print("Successfully init Motor aansturing\r\n");
	return XST_SUCCESS;
}

/**
 * @brief Function to control the motor.
 * 
 * This function is responsible for controlling the motor of the self-driving car.
 * It handles the logic for accelerating, decelerating, and changing direction of the motor.
 * 
 * @param Data Pointer to the global data structure.
 * @return None
 */
void motorAansturing(globalData* Data) {
	// Get speed
	uint8_t percentage_left = applyPWMLimits(Data->speedLeft);
	uint8_t percentage_right = applyPWMLimits(Data->speedRight);

	// store old values
	static uint8_t old_percentage_Left = 0;
	static uint8_t old_percentage_Right = 0;

	// Change left speed if needed
	if (old_percentage_Left != percentage_left){
		old_percentage_Left = percentage_left;
		uint32_t speedTime = PWM_FREQ * percentage_left / MAX_MAX_SPEED_VALUE;
		PwmConfig(&timerLeft, PWM_FREQ, speedTime);
		usleep(10);
		// xil_printf("Left: %d, time: %d\r\n", percentage_left, speedTime);
	}

	// Change right speed if needed
	if (old_percentage_Right != percentage_right){
		old_percentage_Right = percentage_right;
		uint32_t speedTime = PWM_FREQ * percentage_right / MAX_MAX_SPEED_VALUE;
		PwmConfig(&timerRight, PWM_FREQ, speedTime);
		usleep(10);
		// xil_printf("Right: %d, time: %d\r\n", percentage_right, speedTime);
	}
}

/**
 * @brief Initializes the PWM module for motor control.
 *
 * This function initializes the PWM module using the specified timer instance.
 *
 * @param TmrCtrInstancePtr Pointer to the timer controller instance.
 * @param TmrInstanceNr The number of the timer instance to be used.
 * @return XStatus Returns the status of the PWM initialization.
 */
static XStatus PwmInit(XTmrCtr *TmrCtrInstancePtr, uint8_t TmrInstanceNr){
	XStatus Status = XST_SUCCESS;

	// Initialize the timer
	Status = XTmrCtr_Initialize(TmrCtrInstancePtr, TmrInstanceNr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Perform a self-test to ensure that the hardware was built correctly.
	Status = XTmrCtr_SelfTest(TmrCtrInstancePtr, TmrInstanceNr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// PWM timer configuration success
	return Status;
}

/**
 * Configures the PWM for motor control.
 *
 * @param TmrCtrInstancePtr Pointer to the instance of the timer controller.
 * @param Period The period of the PWM signal.
 * @param HighTime The high time of the PWM signal.
 */
static void PwmConfig(XTmrCtr *TmrCtrInstancePtr, uint32_t Period, uint32_t HighTime){
	// Disable PWM
	XTmrCtr_PwmDisable(TmrCtrInstancePtr);

	// Configure PWM
	XTmrCtr_PwmConfigure(TmrCtrInstancePtr, Period, HighTime);

	// Enable PWM
	XTmrCtr_PwmEnable(TmrCtrInstancePtr);
}

/**
 * Applies PWM limits to the given value.
 *
 * @param value The value to apply PWM limits to.
 * @return The value after applying PWM limits.
 */
static uint8_t applyPWMLimits(uint8_t value){
	// Apply PWM limits
	if (value < MIN_SPEED_VALUE + 1){ // +1 because 0 is not allowed
		return MIN_SPEED_VALUE + 1;
	} else if (value > MAX_MAX_SPEED_VALUE - 1){	// -1 because MAX is not allowed
		return MAX_MAX_SPEED_VALUE - 1;
	} else {
		return value; // value is between MIN and MAX
	}
}
