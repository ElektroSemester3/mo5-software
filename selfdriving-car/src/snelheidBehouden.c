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

// ##################################################
// # Constants										#
// ##################################################
// --- Interrupt controller constants ---
#define INTC_DEVICE_ID 			XPAR_PS7_SCUGIC_0_DEVICE_ID
#define ENCODER_DEVICE_ID		XPAR_ARDUINO_ARDUINO_INTR_EN_PINS_2_3_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID 	XPAR_FABRIC_ARDUINO_ARDUINO_INTR_EN_PINS_2_3_IP2INTC_IRPT_INTR
#define ENCODER_INT 			XGPIO_IR_CH1_MASK

// --- Time constants ---
#define TIME_TO_NS_DIVIDER 325 //XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2000000
#define TIME_TO_NS(i) (i / TIME_TO_NS_DIVIDER)
#define NS_TO_TIME(i) (i * TIME_TO_NS_DIVIDER)

// --- Encoder constants ---
#define MASK_ENCODER_INPUT 0xF

// --- Speed limits ---
const uint8_t MAX_MAX_SPEED_VALUE = 200;
const uint8_t NORMAL_MAX_SPEED_VALUE = 100;
const uint8_t MIN_SPEED_VALUE = 0;

// --- Encoder calculations ---
// time between pulses on 6v supply = 15.48ms
// pulses per revolution = 20
// revolutions times 6v supply = 309.6ms
#define TIRE_LENGTH_MM  	206
#define ENCODER_DISK_SLOTS	20
#define MIN_PULSE_TIME		15480 // ns

const uint16_t DISTANCE_PER_PULSE = TIRE_LENGTH_MM * 10 / ENCODER_DISK_SLOTS;  	// DISTANCE_PER_PULSE / 10 = mm per pulse
const uint16_t MAX_SPEED = TIRE_LENGTH_MM * 1000000 / ENCODER_DISK_SLOTS / MIN_PULSE_TIME; // 665.37 mm/s = 2.395 km/h

// --- Encoder masks ---
#define ENCODER_COUNT 2	// The number of encoders
// The masks for each encoder
enum encoder {
	ENCODER_1 = 0x1,
	ENCODER_2 = 0x2,
	ENCODER_3 = 0x4,
	ENCODER_4 = 0x8,
};

// --- Loop time ---
#define LOOP_TIME 1000 // ns

// ##################################################
// # Global variables								#
// ##################################################
XGpio encoderInput; 	// The instance of the encoder input GPIO.
XScuGic INTinstance;	// The instance of the Interrupt Controller
XGpio ledOutput;		// The instance of the LED output GPIO.	
uint16_t speed_storage[ENCODER_COUNT] = {0};	// Speed in mm/s

// ##################################################
// # Function declarations							#
// ##################################################

static void adjustSpeed(speed_struct* speed , uint16_t s_speed[]);
static int16_t applyLimits(int16_t value, int16_t min_value, int16_t max_value);
static void onInterrupt(void* baseaddr_p);
static XStatus InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
static XStatus IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr);


// ##################################################
// # Function definitions							#
// ##################################################
/**
 * @brief 
 * 
 */
XStatus init_snelheidBehouden() {
	XStatus status = XST_SUCCESS;

	// Initialize the encoder input
	status = XGpio_Initialize(&encoderInput, ENCODER_DEVICE_ID);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Initialize the LED output
	status = XGpio_Initialize(&ledOutput, XPAR_USER_LEDS_GPIO_DEVICE_ID);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// set all interrupt direction to inputs
	XGpio_SetDataDirection(&encoderInput, 1, 0xFF);

	// Set all LEDs direction to outputs
	XGpio_SetDataDirection(&ledOutput, 1, 0x0);

	// Initialize interrupt controller
	status = IntcInitFunction(INTC_DEVICE_ID, &encoderInput);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
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
void snelheidBehouden(speed_struct* speed) {
	// Read the encoder input
	uint8_t encoderInputValue = XGpio_DiscreteRead(&encoderInput, 1);
	
	// XTime time_now = 0;
	// static XTime time_old = 0;
	// XTime_GetTime(&time_now);
	// if (time_now - time_old > NS_TO_TIME(LOOP_TIME)){
	// 	time_old = time_now;
	// }
	adjustSpeed(speed, speed_storage);

	XGpio_DiscreteWrite(&ledOutput, 1, encoderInputValue);
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


void adjustSpeed(speed_struct* speed , uint16_t s_speed[]) {

	static uint16_t speedLeftNew = 0;
	static uint16_t speedRightNew = 0;
	
	XTime time_now = 0;
	static XTime time_old = 0;
	XTime_GetTime(&time_now);
	if (time_now - time_old > NS_TO_TIME(LOOP_TIME)){
		time_old = time_now;
		// calculate the speed in %
		uint16_t encoderSpeedLeft = (uint32_t)((s_speed[0] * NORMAL_MAX_SPEED_VALUE) / MAX_SPEED); // mm/s to %
		uint16_t encoderSpeedRight = (uint32_t)((s_speed[1] * NORMAL_MAX_SPEED_VALUE) / MAX_SPEED); // mm/s to %

		// calculate the error value referd to the setpoint
		int16_t errorLeft = 0; 
		int16_t errorRight = 0; 
		if (speed->left != 0) errorLeft = NORMAL_MAX_SPEED_VALUE - encoderSpeedLeft * NORMAL_MAX_SPEED_VALUE/ speed->left;
		if (speed->right != 0) errorRight = NORMAL_MAX_SPEED_VALUE - encoderSpeedRight * NORMAL_MAX_SPEED_VALUE/ speed->right;
		// calculate the new speed 
		speedLeftNew = applyLimits(speed->left != 0 ? speed->left + errorLeft : 0, MIN_SPEED_VALUE, MAX_MAX_SPEED_VALUE);
		speedRightNew = applyLimits(speed->right + errorRight, MIN_SPEED_VALUE, MAX_MAX_SPEED_VALUE);

		// if (*setpointRight != 0 || encoderSpeedLeft != 0 || encoderSpeedRight != 0)xil_printf("Encoder:  %d | %d \t--\t Error: %d | %d \t--\t Left: %d | Right: %d\t--\t Time: %d %ld\r\n",encoderSpeedLeft, encoderSpeedRight, errorLeft, errorRight, speedLeftNew, speedRightNew, (uint64_t)TIME_TO_NS(time_now));
	}

	speed->left = speedLeftNew;
	speed->right = speedRightNew;

}


	
/**
 * @brief Callback function for interrupt handling.
 *
 * This function is called when an interrupt occurs. It takes a base address as a parameter.
 * 
 * @param baseaddr_p The base address of the interrupt.
 */
static void onInterrupt(void* baseaddr_p) {
	static uint8_t oldInput = 0;
	static XTime old_encoderTime[ENCODER_COUNT] = {0};
	// Disable GPIO interrupts
	XGpio_InterruptDisable(&encoderInput, ENCODER_INT);

	// Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&encoderInput) & ENCODER_INT) != ENCODER_INT) {
		return;
	}

	// Get the gpio input value
	uint8_t encoderInputValue = XGpio_DiscreteRead(&encoderInput, ENCODER_INT);
	// Check if the encoder has changed
	uint8_t changedBit = encoderInputValue ^ oldInput;

	// Get the changed bit
	uint8_t risingEdge = (changedBit & oldInput) & changedBit;

	// Check if the changed bit has a rising edge
	if (risingEdge != 0 && changedBit != 0) {
		// Do something when there is a rising edge
		// xil_printf("Rising edge detected: %d\r\n", changedBit);
		// Get the time
		XTime now_time;
		XTime_GetTime(&now_time);

		uint8_t encoderIndex = changedBit / 2;

		// Calculate the time between pulses
		XTime encoderPulseTime = TIME_TO_NS(now_time) - old_encoderTime[encoderIndex];
		old_encoderTime[encoderIndex] = TIME_TO_NS(now_time);

		speed_storage[encoderIndex] = (uint32_t)(DISTANCE_PER_PULSE * 100000) / encoderPulseTime;
	}

	// Update old output
	oldInput = encoderInputValue; 


	// Clear the interrupt such that it is no longer pending in the GPIO
	(void)XGpio_InterruptClear(&encoderInput, ENCODER_INT);

	// Enable GPIO interrupts
	XGpio_InterruptEnable(&encoderInput, ENCODER_INT);
}


/**
 * @brief Sets up the interrupt system for the self-driving car.
 *
 * This function initializes the interrupt system using the provided XScuGic instance pointer.
 *
 * @param XScuGicInstancePtr Pointer to the XScuGic instance.
 * @return XStatus Returns XST_SUCCESS if the interrupt system setup is successful, else an error code.
 */
static XStatus InterruptSystemSetup(XScuGic *XScuGicInstancePtr) {
	// Enable interrupt
	XGpio_InterruptEnable(&encoderInput, ENCODER_INT);
	XGpio_InterruptGlobalEnable(&encoderInput);

	// Register the interrupt handler
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler,
			XScuGicInstancePtr);

	// Connect the interrupt controller interrupt handler to the hardware interrupt handling logic in the processor.
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

/**
 * @brief Initializes the interrupt controller and GPIO instance.
 *
 * This function initializes the interrupt controller and GPIO instance
 * for further use in the program.
 *
 * @param DeviceId The device ID of the interrupt controller.
 * @param GpioInstancePtr Pointer to the GPIO instance.
 * @return XStatus Returns the status of the initialization process.
 */
static XStatus IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr) {
	XScuGic_Config *IntcConfig;
	int status;

	// Interrupt controller initialisation
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&INTinstance, IntcConfig, IntcConfig->CpuBaseAddress);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Call to interrupt setup
	status = InterruptSystemSetup(&INTinstance);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Connect GPIO interrupt to handler
	status = XScuGic_Connect(&INTinstance,
			INTC_GPIO_INTERRUPT_ID,
			(Xil_ExceptionHandler)onInterrupt,
			(void *)GpioInstancePtr);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Enable GPIO interrupts interrupt
	XGpio_InterruptEnable(GpioInstancePtr, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	// Enable GPIO and timer interrupts in the controller
	XScuGic_Enable(&INTinstance, INTC_GPIO_INTERRUPT_ID);

	return XST_SUCCESS;
}
