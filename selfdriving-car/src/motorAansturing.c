/*
 * motorAansturing.c
 *
 *  Created on: 23 Nov 2023
 *      Author: Tjerk
 */
#include "motorAansturing.h"

#include "xtime_l.h"
#include "libs/timer_f.h"
#include "sleep.h"

const uint32_t PWM_FREQ = 500000;
const uint32_t WAIT_TIME = 10000000;

void motorAansturing() {
	//	Do something here
}

// --- temp ---
XTmrCtr timerLeft, timerRight;

int _test_init_motorAansturing(){
	int status_L = PwmInit(&timerLeft, TMR0_DEVICE_ID);
	if (status_L != XST_SUCCESS){
		return XST_FAILURE;
	}
	int status_R = PwmInit(&timerRight, TMR1_DEVICE_ID);
	if (status_R != XST_SUCCESS){
		return XST_FAILURE;
	}
	print("Successfully init Motor aansturing\r\n");
	return 0;
}

void _test_motorAansturing(uint8_t* speedLeft, uint8_t* speedRight) {
	static uint8_t old_speedLeft = 0;
	static uint8_t old_speedRight = 0;

	if (old_speedLeft != *speedLeft){
		old_speedLeft = *speedLeft;
		uint32_t speedTime = PWM_FREQ * *speedLeft / 255;
		PwmConfig(&timerLeft, PWM_FREQ, speedTime);
		usleep(10);
	}

	if (old_speedRight != *speedRight){
		old_speedRight = *speedRight;
		uint32_t speedTime = PWM_FREQ * *speedRight / 255;
		PwmConfig(&timerRight, PWM_FREQ, speedTime);
		usleep(10);
	}
}
