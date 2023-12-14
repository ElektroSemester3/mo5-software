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
#include "obstakeldetectie.h"
#include "lijnherkenning.h"
#include "sturen.h"
#include "snelheidBehouden.h"
#include "motorAansturing.h"

#include "xgpio.h"
#include "xparameters.h"

int main()
{
    init_platform();
    print("Hello World\n\r");

    XGpio btns;
    XGpio_Initialize(&btns, XPAR_USER_INTERFACE_BTNS_GPIO_DEVICE_ID);
    XGpio_SetDataDirection(&btns, 1, 0xF);

    while (XGpio_DiscreteRead(&btns, 1) == 0) {
    	// Wait
    }

    print("Starting...\n\r");

    obstakeldetectieInit();

    while (1) {
    	obstakeldetectie();
    	lijnherkenning();
    	sturen();
    	snelheidBehouden();
    	motorAansturing();
    }

    cleanup_platform();
    return 0;
}
