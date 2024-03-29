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

// --- Status led ---
#define STATUS_LED_DEVICE_ID XPAR_USER_INTERFACE_RGB_LEDS_GPIO_DEVICE_ID

XGpio statusGpio;

XStatus init_statusLed() {
    int status;
    // Initialize the GPIO instance
    status = XGpio_Initialize(&statusGpio, STATUS_LED_DEVICE_ID);
    if (status != XST_SUCCESS) {
        printf("Error initializing status led GPIO\r\n");
        return XST_FAILURE;
    }

    // Set the GPIO channel direction to output
    XGpio_SetDataDirection(&statusGpio, 1, 0x0);

    // Set the status led to orange
    XGpio_DiscreteWrite(&statusGpio, 1, 0x5);

    return XST_SUCCESS;
}


XStatus InitializeModules(){
    // Initialize the button module
	if (initButton() != XST_SUCCESS) {
		xil_printf("Init button failed\r\n");
        // Set the status led to red
        XGpio_DiscreteWrite(&statusGpio, 1, 0x5);
		return XST_FAILURE;
	}

    // Initialize the obstakeldetectie module
    if (obstakeldetectieInit() != XST_SUCCESS) {
        xil_printf("Init obstakeldetectie failed\r\n");
        // Set the status led to red
        XGpio_DiscreteWrite(&statusGpio, 1, 0x1);
    }

    // Initialize the lijnherkenning module
    if (init_lijnherkenning() != XST_SUCCESS) {
        xil_printf("Init lijnherkenning failed\r\n");
        // Set the status led to red
        XGpio_DiscreteWrite(&statusGpio, 1, 0x5);
        return XST_FAILURE;
    }

    // Initialize the snelheidBehouden module
    if (init_snelheidBehouden() != XST_SUCCESS) {
        xil_printf("Init snelheidBehouden failed\r\n");
        // Set the status led to red
        XGpio_DiscreteWrite(&statusGpio, 1, 0x1);
        return XST_FAILURE;
    }

    // Initialize the motorAansturing module
    if (init_motorAansturing() != XST_SUCCESS) {
        xil_printf("Init motorAansturing failed\r\n");
        // Set the status led to red
        XGpio_DiscreteWrite(&statusGpio, 1, 0x1);
        return XST_FAILURE;
    }

    // Set the status led to green
    XGpio_DiscreteWrite(&statusGpio, 1, 0x4);
    return XST_SUCCESS;
}

int main() {
    init_platform();
    xil_printf("_Start_\r\n");

    // Initialize the status led
    if (init_statusLed() != XST_SUCCESS) {
        xil_printf("Init status led failed\r\n");
        cleanup_platform();
        return 0;
    }

    // Initialize the modules
    if (InitializeModules() != XST_SUCCESS) {
        xil_printf("Failed to initialize modules\r\n");
        cleanup_platform();
        return 0;
    }

    // Initialize done
    xil_printf("Modules initialized, starting main loop\r\n");

    while (1) {
        globalData Data;

        // --- TEMPORARY ---
        // When the switch is on, the car will drive at DEFAULT_SPEED
        uint8_t buttons = XGpio_DiscreteRead(&buttonGpio, SWITCH_CHANNEL);
        if (buttons & SWITCH_MASK) {
            Data.speedBase = DEFAULT_SPEED;
        } else {
            Data.speedBase = 0;
        }
        // --- END TEMPORARY ---

        obstakeldetectie(&Data);    // Calculates the speed correction
    	lijnherkenning(&Data);      // Calculates the steering correction
    	sturen(&Data);              // Applies the steering correction 
        snelheidBehouden(&Data);    // Preforms a constant speed
        motorAansturing(&Data);     // Sets the speed of the motors
    }

    cleanup_platform();
    return 0;
}

