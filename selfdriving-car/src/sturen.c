/*
 * sturen.c
 *
 *  Created on: 23 Nov 2023
 *      Author: Lars
 */
#include <stdio.h>
#include "xil_printf.h"
#include "sturen.h"

int scale16to8(int num) {
    // Perform right shift operation on the 16 bits integer
    int result = num >> 8;
    return result;
}


void sturen(globalData *Data) {
// 	// Data->speedBase= NORMAL_MAX_SPEED_VALUE

// 	if (Data->turnValue == DEFAULT_TURN_VALUE){
// 		Data->speedLeft = Data->speedBase;
// 		Data->speedRight = Data->speedBase;
// 	}
// 	else if (Data->turnValue > DEFAULT_TURN_VALUE){
// 		Data->speedLeft = Data->speedBase;
// 		Data->speedRight = Data->speedBase - (float)Data->turnValue / 127 * 100;
// 	}
// printf("speedLeft: %d\n\r", Data->speedLeft);
// printf("speedRight: %d\n\r", Data->speedRight);



   	if (Data->turnValue == DEFAULT_TURN_VALUE){
    		Data->speedLeft = Data->speedBase;
    		Data->speedRight = Data->speedBase;
    	}
    	//naar rechts sturen
    	else if (Data->turnValue < DEFAULT_TURN_VALUE){

    		Data->speedLeft = Data->speedBase;
    		Data->speedRight = Data->speedBase-(Data->speedBase-Data->turnValue*Data->speedBase/DEFAULT_TURN_VALUE);
    		//printf("ja");
    	}
    	else if (Data->turnValue > DEFAULT_TURN_VALUE){
    		//naar links sturen
    		Data->speedLeft = Data->speedBase-(Data->turnValue-DEFAULT_TURN_VALUE)*Data->speedBase/DEFAULT_TURN_VALUE;
    		Data->speedRight = Data->speedBase;
    		//printf("ja");
    	}
    	else{
    		Data->speedLeft = 0;
    		Data->speedRight = 0;
    	}
}
