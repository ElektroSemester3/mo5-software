/*
 * motorAansturing.h
 *
 *  Created on: 23 Nov 2023
 *      Author: Tjerk
 */

#ifndef SRC_MOTORAANSTURING_H_
#define SRC_MOTORAANSTURING_H_

#include <stdint.h>
#include "xstatus.h"
#include "defines.h"

XStatus init_motorAansturing();
void motorAansturing(globalData* Data);

#endif /* SRC_MOTORAANSTURING_H_ */
