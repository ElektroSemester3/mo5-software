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
static void onInterrupt(void *baseaddr_p);


#define INTC_DEVICE_ID 		XPAR_PS7_SCUGIC_0_DEVICE_ID
#define BTNS_DEVICE_ID		XPAR_ARDUINO_ARDUINO_INTR_EN_PINS_2_3_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID XPAR_FABRIC_ARDUINO_ARDUINO_INTR_EN_PINS_2_3_IP2INTC_IRPT_INTR
#define BTN_INT 			XGPIO_IR_CH1_MASK

XGpio gpio;
XScuGic intc;
static int btn_value;

static void onInterrupt(void* baseaddr_p);
static int InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
static int IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr);


int main()
{
    init_platform();
    print("_Start_\r\n");

    int state = _test_init_motorAansturing();
    if (state != 0)
    {
        return state;
    }

    init_snelheidBehouden();




    int status = XGpio_Initialize(&gpio, BTNS_DEVICE_ID);
    if (status != XST_SUCCESS) {
    	return XST_FAILURE;
    }

    // Set all buttons direction to inputs
    XGpio_SetDataDirection(&gpio, 1, 0xFF);

    // Initialize interrupt controller
    status = IntcInitFunction(INTC_DEVICE_ID, &gpio);
    if (status != XST_SUCCESS) {
    	return XST_FAILURE;
    }



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



static void onInterrupt(void* baseaddr_p) {    // Interrupt handler
    xil_printf("Interrupt received\r\n");


	// Disable GPIO interrupts
	XGpio_InterruptDisable(&gpio, BTN_INT);
    // Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&gpio) & BTN_INT) !=
			BTN_INT) {
			return;
		}
	btn_value = XGpio_DiscreteRead(&gpio, 1);
	// Reset if centre button pressed
    (void)XGpio_InterruptClear(&gpio, BTN_INT);
    // Enable GPIO interrupts
    XGpio_InterruptEnable(&gpio, BTN_INT);
}



//----------------------------------------------------
// INITIAL SETUP FUNCTIONS
//----------------------------------------------------

int InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
{
	// Enable interrupt
	XGpio_InterruptEnable(&gpio, BTN_INT);
	XGpio_InterruptGlobalEnable(&gpio);

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 	 	 	 	 	 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
			 	 	 	 	 	 XScuGicInstancePtr);
	Xil_ExceptionEnable();


	return XST_SUCCESS;

}

int IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr)
{
	XScuGic_Config *IntcConfig;
	int status;

	// Interrupt controller initialisation
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&intc, IntcConfig, IntcConfig->CpuBaseAddress);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Call to interrupt setup
	status = InterruptSystemSetup(&intc);
	if(status != XST_SUCCESS) return XST_FAILURE;
	
	// Connect GPIO interrupt to handler
	status = XScuGic_Connect(&intc,
					  	  	 INTC_GPIO_INTERRUPT_ID,
					  	  	 (Xil_ExceptionHandler)onInterrupt,
					  	  	 (void *)GpioInstancePtr);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Enable GPIO interrupts interrupt
	XGpio_InterruptEnable(GpioInstancePtr, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	// Enable GPIO and timer interrupts in the controller
	XScuGic_Enable(&intc, INTC_GPIO_INTERRUPT_ID);
	
	return XST_SUCCESS;
}

