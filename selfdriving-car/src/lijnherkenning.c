/*
 * lijnHerkenning.c
 *
 *  Created on: 23 Nov 2023
 *      Author: Ellen
 */

#include "lijnherkenning.h"
#include "xstatus.h"
#include "xgpio.h"
#include "sleep.h"
#define INPUT_DEVICE_ID XPAR_LIJN_HERKENNING_AXI_GPIO_0_DEVICE_ID

XGpio Inputs;

XStatus init_lijnherkenning() {
	// run once on startup
	xil_printf("Lijnherkenning start\r\n");

	if (XGpio_Initialize(&Inputs, INPUT_DEVICE_ID) != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//1 kanaal van blok in vivado, 0xFF is 8bits( ik gebruik er 6)
	XGpio_SetDataDirection(&Inputs, 1, 0xFF);

	return XST_SUCCESS;
}

void lijnherkenning(globalData* Data) { // uint8_t Snelheid is de snelheid die ik binnen krijg.
	// uint8_t is 8 bits
	//inputs uitlezen, 1 is kanaal 1
	uint8_t Waarde = XGpio_DiscreteRead(&Inputs, 1);
	xil_printf("Waarde: %d\n\r", Waarde);

	/// welke 0 en 1 wat betekend.
	/// ik krijg een 6 bits signaal binnen.
	/// Dit signaal geeft aan welke sensor hoog is en welke laag is.

	// opgeslagen waarde van de pozietie voor de huidige pozitie
	static int oudePositie = 0;

	// variabele Welke waarde bij welke sensor hoort.
	// V = Voor, A = Achter, L =Links, M = Midden, R = Rechts

	// 1 staat op de plek die uitgelezen word de andere plekken zijn 0 en worden niet uitgelezen.
	// 0 staat voor hoeveel plekken die opschuift. (hij begint aan de rechterkant en naar de linkerkant.)
	int VL = Waarde & (1 << 0);
	int VM = Waarde & (1 << 1);
	int VR = Waarde & (1 << 2);
	int AL = Waarde & (1 << 3);
	int AM = Waarde & (1 << 4);
	int AR = Waarde & (1 << 5);

	xil_printf("VL: %d\n\r", VL);
	xil_printf("VM: %d\n\r", VM);
	xil_printf("VR: %d\n\r", VR);
	xil_printf("AL: %d\n\r", AL);
	xil_printf("AM: %d\n\r", AM);
	xil_printf("AR: %d\n\r", AR);

	// welke case
	int HuidigeStaat = 0;

	// bij welke sytuatie welke HuidigeStaat hoort.
	if (VM == 2 && AL == 8) {
		HuidigeStaat = 0;
	} else if (VM == 2 && AM == 16) {
		HuidigeStaat = 1;
		oudePositie = 1; //Pozitie is RechtM
	} else if (VM == 2 && AR == 1) {
		HuidigeStaat = 2;
	} else if (VM == 2) {
		HuidigeStaat = 3;
	} else if (VL == 1 && AL == 8) {
		HuidigeStaat = 4;
	} else if (VL == 1 && AM == 16) {
		HuidigeStaat = 5;
	} else if (VL == 1 && AR == 1) {
		HuidigeStaat = 6;
	} else if (VL == 1) {
		HuidigeStaat = 7;
	} else if (VR == 4 && AL == 8) {
		HuidigeStaat = 8;
	} else if (VR == 4 && AM == 16) {
		HuidigeStaat = 9;
	} else if (VR == 4 && AR == 1) {
		HuidigeStaat = 10;
	} else if (VR == 4) {
		HuidigeStaat = 11;
	}
	//als er geen sensoren gezien worden, maar de vorige state was recht dan mag je sneler dan 50% van de max snelheid
	else if (VL == 62 && VM == 61 && VR == 59 && AL == 55 && AM == 47 && AR == 31
			&& oudePositie == 1) {
		HuidigeStaat = 12;
	} else {
		HuidigeStaat = 13;
	}

	// Deze termen houden de pozitie van de auto in aan de hand hier van word de stuur richting bepaald.
	// 126 –> 0 = Snelheid(heftigheid van sturen) naar links, 127 = Rechtdoor, 128	–> 255 = Snelheid(heftigheid van sturen)naar rechts

	/// deze gegevens moeten door gestuurd worden naar de volgende module.
	/// ik moet de zelfde namen gebruiken als in defines.h
	///uint8_t turnValue;

	int VeScRechts = FULL_LEFT_TURN_VALUE; //0
	int VLx = DEFAULT_TURN_VALUE / 3; //42.666...
	int RechtR = DEFAULT_TURN_VALUE / 3; //42.666...
	int BeScRechts = DEFAULT_TURN_VALUE / 3 * 2; //85.333...
	int RechtM = DEFAULT_TURN_VALUE;  //128
	int VMx = DEFAULT_TURN_VALUE;  //128
	int Onbekend = DEFAULT_TURN_VALUE;  //128
	int BeScLinks = FULL_RIGHT_TURN_VALUE / 6 * 4; //170
	int VRx = FULL_RIGHT_TURN_VALUE / 6 * 5; //212.5
	int RechtL = FULL_RIGHT_TURN_VALUE / 6 * 5; //212.5
	int VeScLinks = FULL_RIGHT_TURN_VALUE; //255

	//SnelheidB(snelheidBegrensing)
	// SnelheidB begint op 100, dus zonder begrensing.
	int SnelheidB = 100;

	switch (HuidigeStaat) {
	case 0: //VMAL = BeScLinks
		Data->turnValue = BeScLinks;
		SnelheidB = NORMAL_MAX_SPEED_VALUE * 0.8;
		break;
	case 1: //VMAM = RechtM
		Data->turnValue = RechtM;
		SnelheidB = NORMAL_MAX_SPEED_VALUE;
		break;
	case 2: //VMAR = BeScRechts
		Data->turnValue = BeScRechts;
		SnelheidB = NORMAL_MAX_SPEED_VALUE * 0.8;
		break;
	case 3: //VM = VMx
		Data->turnValue = VMx;
		SnelheidB = NORMAL_MAX_SPEED_VALUE * 0.6;
		break;
	case 4: //VLAL = RechtR
		Data->turnValue = RechtR;
		SnelheidB = NORMAL_MAX_SPEED_VALUE * 0.9;
		break;
	case 5: //VLAM = BeScRechts
		Data->turnValue = BeScRechts;
		SnelheidB = NORMAL_MAX_SPEED_VALUE * 0.8;
		break;
	case 6: //VLAR = VeScRechts
		Data->turnValue = VeScRechts;
		SnelheidB = NORMAL_MAX_SPEED_VALUE * 0.70;
		break;
	case 7: //VL = VLx
		Data->turnValue = VLx;
		SnelheidB = NORMAL_MAX_SPEED_VALUE * 0.60;
		break;
	case 8: //VRAL = VeScLinks
		Data->turnValue = VeScLinks;
		SnelheidB = NORMAL_MAX_SPEED_VALUE * 0.70;
		break;
	case 9: //VRAM = BeScRechts
		Data->turnValue = BeScLinks;
		SnelheidB = NORMAL_MAX_SPEED_VALUE * 0.80;
		break;
	case 10: //VRAR = RechtL
		Data->turnValue = RechtL;
		SnelheidB = NORMAL_MAX_SPEED_VALUE * 0.90;
		break;
	case 11: //VR = VRx
		Data->turnValue = VRx;
		SnelheidB = NORMAL_MAX_SPEED_VALUE * 0.60;
		break;
	case 12: //onbekend en oudepozitie is 1 dus RechtM
		Data->turnValue = Onbekend;
		SnelheidB = NORMAL_MAX_SPEED_VALUE;
		break;
	case 13:
		Data->turnValue = Onbekend;
		SnelheidB = NORMAL_MAX_SPEED_VALUE / 2;
		break;
	}
	xil_printf("HuidigeStaat: %d\r\n",HuidigeStaat);
	xil_printf("Data->turnValue: %d\r\n",Data->turnValue);
	xil_printf("SnelheidB: %d\r\n",SnelheidB);


	// SnelheidBegrenzing(snelheidB)definieren
	// SnelheidBegrensing maken
	// Input van de snelheid mag niet hoger worden dan SnelheidB

	if (Data->speedBase > SnelheidB) {
		Data->speedBase = SnelheidB;
	}
}

