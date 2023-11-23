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

#define DEFAULT_SPEED	100

int main()
{
    init_platform();
    print("Hello World\n\r");

    while (1) {
    	static u8 speedL, speedR = DEFAULT_SPEED;
    	obstakeldetectie();
    	lijnherkenning();
    	sturen();
    	snelheidBehouden(&speedL, &speedR);
    	motorAansturing();
    	_test_motorAansturing(&speedL, &speedR); // --- temp ---
    }

    cleanup_platform();
    return 0;
}
