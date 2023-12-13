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
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xscugic.h"
#include "xgpio.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xgpio.h"

#include "defines.h"
#include "obstakeldetectie.h"
#include "lijnherkenning.h"
#include "sturen.h"
#include "snelheidBehouden.h"
#include "motorAansturing.h"

// --- TEMPORARY ---
#define BUTTON_DEVICE_ID XPAR_USER_INTERFACE_BTNS_GPIO_DEVICE_ID
#define BUTTON_CHANNEL 1
#define BUTTON_MASK 0x1

XGpio buttonGpio;

int initButton() {
    int status;
    // Initialize the GPIO instance
    status = XGpio_Initialize(&buttonGpio, BUTTON_DEVICE_ID);
    if (status != XST_SUCCESS) {
        printf("Error initializing button GPIO\r\n");
        return XST_FAILURE;
    }

    // Set the GPIO channel direction to input
    XGpio_SetDataDirection(&buttonGpio, BUTTON_CHANNEL, 0xFFFFFFFF);

    return XST_SUCCESS;
}
// --- END TEMPORARY ---


int main()
{
    init_platform();
    print("_Start_\r\n");
    
    // Initialize the modules
    if (init_motorAansturing() != XST_SUCCESS) return XST_FAILURE;
    if (init_snelheidBehouden() != XST_SUCCESS) return XST_FAILURE;
    if (initButton() != XST_SUCCESS) return XST_FAILURE;

   

    while (1) {
        speed_struct Speed;

        // --- TEMPORARY ---
        uint8_t buttons = XGpio_DiscreteRead(&buttonGpio, BUTTON_CHANNEL);
        if (buttons & BUTTON_MASK) {
            Speed.left = DEFAULT_SPEED;
            Speed.right = DEFAULT_SPEED;
        }
        else
        {
            Speed.left = 0;
            Speed.right = 0;
        }
        // --- END TEMPORARY ---

        obstakeldetectie();
        lijnherkenning();
        sturen();                   // convert Speed.speed to Speed.left and Speed.right
        snelheidBehouden(&Speed);   // Changes Speed.left and Speed.right
        motorAansturing(&Speed);    // Sets the speed of the motors
    }

    cleanup_platform();
    return 0;
}

