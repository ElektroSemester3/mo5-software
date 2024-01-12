/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>

#include "defines.h"
#include "lijnherkenning.h"
#include "motorAansturing.h"
#include "obstakeldetectie.h"
#include "platform.h"
#include "snelheidBehouden.h"
#include "sturen.h"
#include "xgpio.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xscugic.h"

// --- TEMPORARY ---
#define SWITCH_DEVICE_ID XPAR_USER_INTERFACE_SWITCHES_GPIO_DEVICE_ID
#define SWITCH_CHANNEL 1
#define SWITCH_MASK 0x1

XGpio buttonGpio;

int initButton() {
    int status;
    // Initialize the GPIO instance
    status = XGpio_Initialize(&buttonGpio, SWITCH_DEVICE_ID);
    if (status != XST_SUCCESS) {
        printf("Error initializing button GPIO\r\n");
        return XST_FAILURE;
    }

    // Set the GPIO channel direction to input
    XGpio_SetDataDirection(&buttonGpio, SWITCH_CHANNEL, 0xFFFFFFFF);

    return XST_SUCCESS;
}
// --- END TEMPORARY ---

XStatus InitializeModules(){
    // Initialize the button module
	if (initButton() != XST_SUCCESS) {
		xil_printf("Init button failed\r\n");
		return XST_FAILURE;
	}

    // Initialize the obstakeldetectie module
    if (obstakeldetectieInit() != XST_SUCCESS) {
        xil_printf("Init obstakeldetectie failed\r\n");
        return XST_FAILURE;
    }

    // Initialize the snelheidBehouden module
    if (init_snelheidBehouden() != XST_SUCCESS) {
        xil_printf("Init snelheidBehouden failed\r\n");
        return XST_FAILURE;
    }

    // Initialize the motorAansturing module
    if (init_motorAansturing() != XST_SUCCESS) {
        xil_printf("Init motorAansturing failed\r\n");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int main() {
    init_platform();
    xil_printf("_Start_\r\n");

    // Initialize the modules
    if (InitializeModules() != XST_SUCCESS) {
        xil_printf("Failed to initialize modules\r\n");
        cleanup_platform();
        return 0;
    }
    xil_printf("Modules initialized, starting main loop\r\n");

    while (1) {
        globalData Data;

        // --- TEMPORARY ---
        uint8_t buttons = XGpio_DiscreteRead(&buttonGpio, SWITCH_CHANNEL);
        if (buttons & SWITCH_MASK) {
            Data.speedLeft = DEFAULT_SPEED;
            Data.speedRight = DEFAULT_SPEED;
            Data.speedBase = DEFAULT_SPEED;
            Data.turnValue = FULL_RIGHT_TURN_VALUE;
        } else {
            Data.speedLeft = 0;
            Data.speedRight = 0;
            Data.speedBase = 0;
            Data.turnValue = DEFAULT_TURN_VALUE;
        }
        // --- END TEMPORARY ---

        obstakeldetectie(&Data);

        Data.speedLeft = Data.speedBase;
        Data.speedRight = Data.speedBase;
    	lijnherkenning();
    	sturen();
        snelheidBehouden(&Data);   // Changes Speed.left and Speed.right
        motorAansturing(&Data);    // Sets the speed of the motors
    }

    cleanup_platform();
    return 0;
}

