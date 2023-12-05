/*
 * snelheidBehouden.h
 *
 *  Created on: 23 Nov 2023
 *      Author: Tommy
 */

#ifndef SRC_SNELHEIDBEHOUDEN_H_
#define SRC_SNELHEIDBEHOUDEN_H_

#include <stdint.h>
#include "xstatus.h"


XStatus init_snelheidBehouden();
void snelheidBehouden(uint8_t* speedLeft, uint8_t* speedRight);

#endif /* SRC_SNELHEIDBEHOUDEN_H_ */
