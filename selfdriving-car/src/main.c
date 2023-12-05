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

#include "obstakeldetectie.h"
#include "lijnherkenning.h"
#include "sturen.h"
#include "snelheidBehouden.h"
#include "motorAansturing.h"

const uint8_t DEFAULT_SPEED = 50;


int main()
{
    init_platform();
    print("_Start_\r\n");
    
    if (_test_init_motorAansturing() != XST_SUCCESS) return XST_FAILURE;
    if (init_snelheidBehouden() != XST_SUCCESS) return XST_FAILURE;

    while (1) {
        uint8_t speedL = DEFAULT_SPEED;
        uint8_t speedR = DEFAULT_SPEED;
        obstakeldetectie();
        lijnherkenning();
        sturen();
        snelheidBehouden(&speedL, &speedR);
        motorAansturing();

        // --- temp ---
        _test_motorAansturing(&speedL, &speedR);
    }

    cleanup_platform();
    return 0;
}

