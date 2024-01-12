/*
 * lijnHerkenning.c
 *
 *  Created on: 23 Nov 2023
 *      Author: Ellen
 */

#include "lijnherkenning.h"
#include "xstatus.h"
#include "xgpio.h"
#define INPUT_DEVICE_ID XPAR_LIJN_HERKENNING_AXI_GPIO_0_DEVICE_ID

XStatus init_lijnherkenning(){
	// run once on startup
	xil_printf("Lijnherkenning start\r\n");

	return XST_SUCCESS;
}

void lijnherkenning(globalData* Data) { // uint8_t Snelheid is de snelheid die ik binnen krijg.
	/// De pinnen van de Sensoren
		XGpio Inputs;
		XGpio_Initialize(&Inputs,INPUT_DEVICE_ID);
		//1 kanaal van blok in vivado, 0xFF is 8bits( ik gebruik er 6)
		XGpio_SetDataDirection(&Inputs, 1, 0xFF);
		// uint8_t is 8 bits
		//inputs uitlezen, 1 is kanaal 1
		uint8_t Waarde = XGpio_DiscreteRead(&Inputs, 1);

		/// welke 0 en 1 wat betekend.
		/// ik krijg een 6 bits signaal binnen.
		/// Dit signaal geeft aan welke sensor hoog is en welke laag is.

		// variabele Welke waarde bij welke sensor hoort.
		// V = Voor, A = Achter, L =Links, M = Midden, R = Rechts
		int VL, VM, VR, AL, AM, AR;

		// 1 staat op de plek die uitgelezen word de andere plekken zijn 0 en worden niet uitgelezen.
		// 0 staat voor hoeveel plekken die opschuift. (hij begint aan de rechterkant en naar de linkerkant.)
		VL = Waarde & (1 << 0);
		VM = Waarde & (1 << 1);
		VR = Waarde & (1 << 2);
		AL = Waarde & (1 << 3);
		AM = Waarde & (1 << 4);
		AR = Waarde & (1 << 5);

	// welke case
	int HuidigeStaat = 0;

	// bij welke sytustie welke HuidigeStaat hoort.
		if(VM == 1 && AL == 1){
			HuidigeStaat = 0;
		}
		else if(VM == 1 && AM == 1){
			HuidigeStaat = 1;
		}
		else if(VM == 1 && AR == 1){
			HuidigeStaat = 2;
		}
		else if(VM == 1){
			HuidigeStaat = 3;
		}
		else if(VL == 1 && AL == 1){
			HuidigeStaat = 4;
		}
		else if(VL == 1 && AM == 1){
			HuidigeStaat = 5;
		}
		else if(VL == 1 && AR == 1){
			HuidigeStaat = 6;
		}
		else if(VL == 1){
			HuidigeStaat = 7;
		}
		else if(VR == 1 && AL == 1){
			HuidigeStaat = 8;
		}
		else if(VR == 1 && AM == 1){
			HuidigeStaat = 9;
		}
		else if(VR == 1 && AR == 1){
			HuidigeStaat = 10;
		}
		else if(VR == 1){
			HuidigeStaat = 11;
		}
		else{
			HuidigeStaat = 12;
		}

		// Deze termen houden de pozitie van de auto in aan de hand hier van word de stuur richting bepaald.
		    // 126 –> 0 = Snelheid(heftigheid van sturen) naar links, 127 = Rechtdoor, 128	–> 255 = Snelheid(heftigheid van sturen)naar rechts

		int RechtM, RechtL, RechtR, BeScLinks, BeScRechts, VeScRechts, VeScLinks, VMx, VRx, VLx, Onbekend;

	    /// deze gegevens moeten door gestuur worden naar de volgens module sturen.
		/// Tommy en Jochem gaan iets maken zo dat de module kunnrmn gaan comuliseren.
		///uint8_t turnValue;
		VeScRechts = FULL_LEFT_TURN_VALUE; //0
		VLx = DEFAULT_TURN_VALUE/3; //42.666...
		RechtR = DEFAULT_TURN_VALUE/3; //42.666...
		BeScRechts = DEFAULT_TURN_VALUE/3*2; //85.333...
		RechtM = DEFAULT_TURN_VALUE;  //128
		VMx = DEFAULT_TURN_VALUE;  //128
		Onbekend = DEFAULT_TURN_VALUE;  //128
		BeScLinks = FULL_RIGHT_TURN_VALUE/6*4; //170
		VRx = FULL_RIGHT_TURN_VALUE/6*5; //212.5
		RechtL = FULL_RIGHT_TURN_VALUE/6*5; //212.5
		VeScLinks = FULL_RIGHT_TURN_VALUE; //255


		//SnelheidB(snelheidBegrensing)
		// SnelheidB begint op 100, dus zonder begrensing.
		int SnelheidB = 100;

			switch (HuidigeStaat) {
			case 0: //VMAL = BeScLinks
				BeScLinks;
				SnelheidB = 80;
				break;
			case 1: //VMAM = RechtM
				RechtM;
				SnelheidB = 100;
				break;
			case 2: //VMAR = BeScRechts
				BeScRechts;
				SnelheidB = 80;
				break;
			case 3: //VM = VMx
				VMx;
				SnelheidB = 60;
				break;
			case 4: //VLAL = RechtR
				RechtR;
				SnelheidB = 90;
				break;
			case 5: //VLAM = BeScRechts
				BeScRechts;
				SnelheidB = 80;
				break;
			case 6: //VLAR = VeScRechts
				VeScRechts;
				SnelheidB = 70;
				break;
			case 7: //VL = VLx
				VLx;
				SnelheidB = 60;
				break;
			case 8: //VRAL = VeScLinks
				VeScLinks;
				SnelheidB = 70;
				break;
			case 9: //VRAM = BeScRechts
				BeScRechts;
				SnelheidB = 80;
				break;
			case 10: //VRAR = RechtL
				RechtL;
				SnelheidB = 90;
				break;
			case 11: //VR = VRx
				VRx;
				SnelheidB = 60;
				break;
			case 12:
				Onbekend;
				SnelheidB = 50;
				break;
			}



			// SnelheidBegrenzing(snelheidB)definieren
			// SnelheidBegrensing maken
			// Input van de snelheid mag niet hoger worden dan SnelheidB

			if(Data->speedBase > SnelheidB){
				Data->speedBase = SnelheidB;
			}

}











