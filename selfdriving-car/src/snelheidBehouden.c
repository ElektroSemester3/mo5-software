/*
 * snelheidBehouden.c
 *
 *  Created on: 23 Nov 2023
 *  Author: Tommy de Wever
 */

#include "snelheidBehouden.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "xgpio.h"
#include "xtmrctr.h"
#include "xparameters.h"
#include <stdbool.h>
#include "xtime_l.h"
#include "PID.h"

// --- Interrupt controller constants ---
#define INTC_DEVICE_ID 			XPAR_PS7_SCUGIC_0_DEVICE_ID
#define ENCODER_DEVICE_ID		XPAR_SPEED_SENSORS_AXI_GPIO_0_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID 	XPAR_FABRIC_SPEED_SENSORS_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define ENCODER_INT 			XGPIO_IR_CH1_MASK

// --- PID values ---
#define MULTIPLICATION_FACTOR 0xFFFF
#define KP 1.135
#define KI 7.5285
#define KD 0
// --- PID constants ---
#define Kp_VALUE (uint32_t)(KP * MULTIPLICATION_FACTOR)
#define Ki_VALUE (uint32_t)(KI * MULTIPLICATION_FACTOR)
#define Kd_VALUE (uint32_t)(KD * MULTIPLICATION_FACTOR)

// --- Global variables ---
XGpio encoderInput; 	// The instance of the encoder input GPIO.
XScuGic INTinstance;	// The instance of the Interrupt Controller
uint16_t speed_storage[encoder_count] = {0};	// Speed in mm/s
pid_struct PID[encoder_count];	// The PID controller struct

// --- Function declarations ---
static void onInterrupt(void* baseaddr_p);
static XStatus InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
static XStatus IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr);
static void resetSpeed(globalData* Data, uint8_t index);

// --- Function definitions ---

/**
 * @brief Initializes the "snelheidBehouden" module.
 * 
 * This function is responsible for initializing the necessary resources and variables
 * required for the "snelheidBehouden" module to function properly.
 * 
 * @return XStatus Returns the status of the initialization process.
 *         - XST_SUCCESS if the initialization is successful.
 *         - XST_FAILURE if the initialization fails.
 */
XStatus init_snelheidBehouden() {
	XStatus status = XST_SUCCESS;

	// Initialize the encoder input
	status = XGpio_Initialize(&encoderInput, ENCODER_DEVICE_ID);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// set all interrupt direction to inputs
	XGpio_SetDataDirection(&encoderInput, 1, 0xFF);

	// Initialize interrupt controller
	status = IntcInitFunction(INTC_DEVICE_ID, &encoderInput);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Initialize the PID controler
	#ifdef DEBUG_MODE
		xil_printf("P: %u, I: %u, D: %u \r\n", Kp_VALUE, Ki_VALUE, Kd_VALUE);
	#endif
	for (uint8_t i = 0; i < encoder_count; i++){
		pid_init(&PID[i], Kp_VALUE, Ki_VALUE, Kd_VALUE, MAX_MAX_SPEED_VALUE, MIN_SPEED_VALUE, MULTIPLICATION_FACTOR);
	}

	// success
	return XST_SUCCESS;
}

/**
 * @brief Maintains the speed of the self-driving car.
 *
 * This function is responsible for maintaining the speed of the self-driving car.
 * It takes in the pointers to the left and right speed variables and updates them accordingly.
 *
 * @param Data The globalData struct containing the speed values for the left and right motor.
 */
void snelheidBehouden(globalData* Data) {	// Adjust the speed
	// old speed storage
	static uint16_t speedValue[encoder_count] = {0};
	
	// create a timer to adjust the speed in a certain time
	XTime time_now = 0;
	static XTime time_old = 0;
	XTime_GetTime(&time_now);
	if (time_now - time_old > NS_TO_TIME(SPEED_CALC_LOOP_TIME)){
		time_old = time_now;

		#ifdef DEBUG_MODE
			bool print = false;
		#endif

		for (uint8_t i = 0; i < encoder_count; i++){
			// reset the speed storage if the speed is 0
		 	resetSpeed(Data, i);

			// calculate the speed in %
			uint16_t encoderSpeed = (uint32_t)((speed_storage[i] * NORMAL_MAX_SPEED_VALUE) / MAX_SPEED); // mm/s to %

			// get the setpoint value
			uint16_t speed_setpoint = 0;
			switch (i)
			{
			case encoder_left:
				speed_setpoint = Data->speedLeft;
				break;

			case encoder_right:
				speed_setpoint = Data->speedRight;
				break;
			
			default:
				break;
			}

			// calculate the error value referd to the setpoint
			int16_t error = 0;
			
			// only calculate the error if the setpoint is not 0
			// error = NORMAL_MAX_SPEED_VALUE - encoderSpeed * NORMAL_MAX_SPEED_VALUE / speed_setpoint;
			error = speed_setpoint - encoderSpeed;
		
			// calculate the new speed
			speedValue[i] = pid_calculate(&PID[i], error, speed_setpoint);
			
			#ifdef DEBUG_MODE
				xil_printf("Encoder:  %d \t-- Error: %d \t-- Speed value: %d \t--\t Time: %d \t | \t",encoderSpeed, error, speedValue[i], (uint64_t)TIME_TO_NS(time_now));
				print = true;
			#endif
		}
		#ifdef DEBUG_MODE
			if (print) xil_printf("\r\n");
		#endif
	}

	// set the new speed based on the stored value
	Data->speedLeft = speedValue[encoder_left];
	Data->speedRight = speedValue[encoder_right];
}

/**
 * @brief Resets the speed storage if the speed is 0.
 * 
 * This function is responsible for resetting the speed storage if the speed is 0.
 * 
 * @param Data The globalData struct containing the speed values for the left and right motor.
 * @param index The index of the encoder.
 */
static void resetSpeed(globalData* Data, uint8_t index){
	// define reset value
	typeof(Data->speedLeft) speed_setpoint = MIN_SPEED_VALUE;

	// get the setpoint value
	switch (index)
	{
	case encoder_left:
		speed_setpoint = Data->speedLeft;
		break;

	case encoder_right:
		speed_setpoint = Data->speedRight;
		break;
	
	default:
		break;
	}

	// reset the speed storage if the speed setpoint is 0
	if (speed_setpoint == 0){
		speed_storage[index] = 0;
	}
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
	static XTime old_encoderTime[encoder_count] = {0};
	// Disable GPIO interrupts
	XGpio_InterruptDisable(&encoderInput, ENCODER_INT);

	// Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&encoderInput) & ENCODER_INT) != ENCODER_INT) {
		return;
	}

	// Get the gpio input value
	uint8_t encoderInputValue = XGpio_DiscreteRead(&encoderInput, ENCODER_INT);

	// Get the changed bit
	uint8_t changedBit = encoderInputValue ^ oldInput;

	// Detect edge
	uint8_t risingEdge = (changedBit & oldInput) & changedBit;

	// Do something when there is arising edge
	if (risingEdge != 0 && changedBit > 0) {
		// xil_printf("Rising edge detected: %d\r\n", changedBit);

		// Get the time
		XTime now_time;
		XTime_GetTime(&now_time);

		// Get the encoder index
		uint8_t encoderIndex = changedBit / 2;

		// Calculate the time between pulses
		XTime encoderPulseTime = TIME_TO_NS(now_time) - old_encoderTime[encoderIndex];
		old_encoderTime[encoderIndex] = TIME_TO_NS(now_time);

		// Calculate the speed
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

	// success
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

	// success
	return XST_SUCCESS;
}
