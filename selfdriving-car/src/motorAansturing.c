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

void motorAansturing() {
	//	Do something here
}

// --- temp ---
XTmrCtr timerLeft, timerRight;

#define MAX_SPEED 686 // 686 mm/s = 2.47 km/h

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

void _test_motorAansturing(uint8_t* speedLeft, uint8_t* speedRight) {
	uint8_t DC_left = (uint16_t)(255 * *speedLeft / MAX_SPEED);
	uint8_t DC_right = (uint16_t)(255 * *speedRight / MAX_SPEED);

	static uint8_t old_DC_Left = 0;
	static uint8_t old_DC_Right = 0;

	if (old_DC_Left != DC_left){
		old_DC_Left = DC_left;
		uint32_t speedTime = PWM_FREQ * DC_left / 255;
		PwmConfig(&timerLeft, PWM_FREQ, speedTime);
		usleep(10);
	}

	if (old_DC_Right != DC_right){
		old_DC_Right = DC_right;
		uint32_t speedTime = PWM_FREQ * DC_right / 255;
		PwmConfig(&timerRight, PWM_FREQ, speedTime);
		usleep(10);
	}
}

