
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xadcps.h"
#include "sleep.h"
#define XADC_DEVICE_ID XPAR_XADCPS_0_DEVICE_ID
static XAdcPs XAdcInst; /* XADC driver instance */
int turnValue;
int speedLeft;
int speedRight;
int speedBase;
int DEFAULT_TURN_VALUE = 128;
int state;
int newTurnValue;


float XAdcPs_RawToVoltage_own(u8 AdcData)
{
    return (((float)(AdcData)) * 3.3f)/65536.0f;
}

int scale16to8(int num) {
    // Perform right shift operation on the 16 bits integer
    int result = num >> 8;
    return result;
}

int main()
{
    init_platform();
    int Status;
    XAdcPs_Config *ConfigPtr;
    XAdcPs *XAdcInstPtr = &XAdcInst;

    ConfigPtr = XAdcPs_LookupConfig(XADC_DEVICE_ID);

    if (ConfigPtr == NULL) {
        return XST_FAILURE;
    }
    
    XAdcPs_CfgInitialize(XAdcInstPtr, ConfigPtr, ConfigPtr->BaseAddress);
    Status = XAdcPs_SelfTest(XAdcInstPtr);

    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    //any additional settings changes come here
    XAdcPs_SetSequencerMode(XAdcInstPtr,
    XADCPS_SEQ_MODE_CONTINPASS);

    u16 VccPintRawData1; //get the raw value from the adc
    u16 VccPintRawData2;
    //float VccPintData; // keeps the translated value from adc (in volts)

    //Status = XAdcConfig(XADC_DEVICE_ID, XADCPS_SEQ_MODE_CONTINPASS);

    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    int adc_channels [6] = {1,9,6,15,5,13};

    while(1){
    	//rechtdoor blijven rijden
    	if (turnValue == DEFAULT_TURN_VALUE){
    		speedLeft = speedBase;
    		speedRight = speedBase;
    	}
    	//naar rechts sturen
    	else if (turnValue < DEFAULT_TURN_VALUE){

    		speedLeft = speedBase;
    		speedRight = speedBase-(speedBase-turnValue*speedBase/DEFAULT_TURN_VALUE);
    		//printf("ja");
    	}
    	else if (turnValue > DEFAULT_TURN_VALUE){
    		//naar links sturen
    		speedLeft = speedBase-(turnValue-DEFAULT_TURN_VALUE)*speedBase/DEFAULT_TURN_VALUE;
    		speedRight = speedBase;
    		//printf("ja");
    	}
    	else{
    		speedLeft = 0;
    		speedRight = 0;
    	}

    	printf("speedBase: %d\n\r", speedBase);
        printf("speedLeft: %d\n\r", speedLeft);
        printf("speedRight: %d\n\r", speedRight);
        printf("turnValue: %d\n\r", turnValue);
        printf("state: %d\n\r", state);
        printf("newTurnValue: %d\n\r", newTurnValue);

        XAdcPs_Reset(&XAdcInst); //erase the old used values

        printf("\n\r ----- New samples ----- \n\r");

        VccPintRawData1 = XAdcPs_GetAdcData(&XAdcInst,XADCPS_CH_AUX_MIN + adc_channels[0]);
        VccPintRawData2 = XAdcPs_GetAdcData(&XAdcInst,XADCPS_CH_AUX_MIN + adc_channels[1]);

        //VccPintData = XAdcPs_RawToVoltage_own(VccPintRawData);
        //printf("Channel %d: %d Volts.\n\r", i, scale16to8(VccPintRawData) );

        turnValue=scale16to8(VccPintRawData1);
        speedBase=scale16to8(VccPintRawData2);

        usleep(100000);
    }


//void sturen() {
//
//
//	if (turnValue == DEFAULT_TURN_VALUE){
//		speedLeft = speedBase;
//		speedRight = speedBase;
//	}
//	else if (turnValue > DEFAULT_TURN_VALUE){
//		speedLeft = speedBase;
//		speedRight = speedBase;
//	}


//}


    cleanup_platform();
    return 0;
}
