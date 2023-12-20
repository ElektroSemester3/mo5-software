/*
 * VL53L0X.h
 *
 *  Created on: 15 Dec 2023
 *      Author: Jochem
 */

#ifndef SRC_VL53L0X_H_
#define SRC_VL53L0X_H_

#include <stdint.h>

int iic_init();
int connectionCheck();
int config_init();
int tuning_loadDefault();
int calibration_start();

int measurement_start(uint16_t* distance);

#endif /* SRC_VL53L0X_H_ */
