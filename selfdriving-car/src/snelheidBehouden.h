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
#include "defines.h"

XStatus init_snelheidBehouden();
void snelheidBehouden(globalData* Data);

#endif /* SRC_SNELHEIDBEHOUDEN_H_ */
