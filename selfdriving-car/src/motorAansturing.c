/*
 * motorAansturing.c
 *
 *  Created on: 23 Nov 2023
 *      Author: Tjerk
 */
#include "motorAansturing.h"
#include "xil_exception.h"
#include "xtmrctr.h"
#include "sleep.h"

const uint32_t PWM_FREQ = 500000;
const uint32_t WAIT_TIME = 10000000;

// --- Speed limits ---
const uint8_t MAX_SPEED_VALUE_ = 200;
const uint8_t MIN_SPEED_VALUE_ = 0;

void motorAansturing() {
	//	Do something here
}

// --- temp ---
XTmrCtr timerLeft, timerRight;

int PwmInit(XTmrCtr *TmrCtrInstancePtr, uint8_t TmrInstanceNr){

	int Status = XST_SUCCESS;

	Status = XTmrCtr_Initialize(TmrCtrInstancePtr, TmrInstanceNr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built
	 * correctly.
	 */
	Status = XTmrCtr_SelfTest(TmrCtrInstancePtr, TmrInstanceNr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	return Status;
}

void PwmConfig(XTmrCtr *TmrCtrInstancePtr, u32 Period, u32 HighTime){
	XTmrCtr_PwmDisable(TmrCtrInstancePtr);

	XTmrCtr_PwmConfigure(TmrCtrInstancePtr, Period, HighTime);

	XTmrCtr_PwmEnable(TmrCtrInstancePtr);
}

int _test_init_motorAansturing(){
	int status_L = PwmInit(&timerLeft, XPAR_TMRCTR_0_DEVICE_ID);
	if (status_L != XST_SUCCESS){
		return XST_FAILURE;
	}
	int status_R = PwmInit(&timerRight, XPAR_TMRCTR_1_DEVICE_ID);
	if (status_R != XST_SUCCESS){
		return XST_FAILURE;
	}
	print("Successfully init Motor aansturing\r\n");
	return 0;
}

static uint8_t applyPWMLimits(uint8_t value){
	if (value < MIN_SPEED_VALUE_ + 1){
		return 1;
	} else if (value > MAX_SPEED_VALUE_ - 1){
		return MAX_SPEED_VALUE_ - 1;
	} else {
		return value;
	}
}

void _test_motorAansturing(speed_struct* speed) {
	uint8_t percentage_left = applyPWMLimits(speed->left);
	uint8_t percentage_right = applyPWMLimits(speed->right);

	static uint8_t old_percentage_Left = 0;
	static uint8_t old_percentage_Right = 0;

	if (old_percentage_Left != percentage_left){
		old_percentage_Left = percentage_left;
		uint32_t speedTime = PWM_FREQ * percentage_left / MAX_SPEED_VALUE_;
		PwmConfig(&timerLeft, PWM_FREQ, speedTime);
		usleep(10);
		// xil_printf("Left: %d, time: %d\r\n", percentage_left, speedTime);
	}

	if (old_percentage_Right != percentage_right){
		old_percentage_Right = percentage_right;
		uint32_t speedTime = PWM_FREQ * percentage_right / MAX_SPEED_VALUE_;
		PwmConfig(&timerRight, PWM_FREQ, speedTime);
		usleep(10);
		// xil_printf("Right: %d, time: %d\r\n", percentage_right, speedTime);
	}
}

