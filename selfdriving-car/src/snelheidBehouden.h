/*
 * snelheidBehouden.h
 *
 *  Created on: 23 Nov 2023
 *      Author: Tommy
 */

#ifndef SRC_SNELHEIDBEHOUDEN_H_
#define SRC_SNELHEIDBEHOUDEN_H_

#include <stdint.h>
#define INTC_GPIO_INTERRUPT_ID      XPAR_FABRIC_USER_USER_INTC_IRQ_INTR
#define INTC_ARD_GPIO_INTERRUPT_ID  XPAR_FABRIC_ARDUINO_ARDUINO_INTR_EN_PINS_2_3_IP2INTC_IRPT_INTR


void init_snelheidBehouden();
void snelheidBehouden(uint8_t* speedLeft, uint8_t* speedRight);

#endif /* SRC_SNELHEIDBEHOUDEN_H_ */
