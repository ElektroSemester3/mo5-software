/*
 * VL53L0X.c
 *
 *  Created on: 15 Dec 2023
 *      Author: Jochem
 */

#include "VL53L0X.h"

#include "xiicps.h"
#include "xil_printf.h"

#define IIC_DEVICE_ID XPAR_PS7_I2C_1_DEVICE_ID
#define IIC_CLOCK_SPEED 100000
#define IIC_SLAVE_ADDR 0x29

#define REG_MODEL_ID 0xC0
#define EXPECTED_MODEL_ID 0xEE

#define VL53L0X_REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV 0x89

#define REG_IIC_MODE 0x88
#define IIC_MODE_STANDARD 0x00

#define REG_SYSTEM_INTERRUPT_GPIO_CONFIG 0x0A
#define SYSTEM_INTERRUPT_NEW_SAMPLE_READY 0x04

#define REG_GPIO_HV_MUX_ACTIVE_HIGH 0x84
#define MASK_INTERRUPT_ACTIVE_LOW ~0x10

#define REG_SYSTEM_INTERRUPT_CLEAR 0x0B
#define SYSTEM_INTERRUPT_CLEAR 0x01

#define REG_SYSTEM_SEQUENCE_CONFIG 0x01
#define RANGE_SEQUENCE_STEP_TCC 0x10
#define RANGE_SEQUENCE_STEP_MSRC 0x04
#define RANGE_SEQUENCE_STEP_DSS 0x28
#define RANGE_SEQUENCE_STEP_PRE_RANGE 0x40
#define RANGE_SEQUENCE_STEP_FINAL_RANGE 0x80

#define REG_SYSRANGE_START 0x00
#define SYSRANGE_ARMED 0x01

#define REG_RESULT_INTERRUPT_STATUS 0x13

#define REG_RESULT_RANGE_STATUS 0x14

#define REG_MSRC_CONFIG_CONTROL 0x60

#define GLOBAL_CONFIG_REF_EN_START_SELECT 0xB6
#define DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD 0x4E
#define DYNAMIC_SPAD_REF_EN_START_OFFSET 0x4F
#define GLOBAL_CONFIG_SPAD_ENABLES_REF_0 0xB0

XIicPs iic;  // I2C instance

static uint8_t stopVariable = 0;
static uint32_t measurementTimingBudget;

// Setup I2C hardware and run a self test
int iic_init() {
    int status;
    XIicPs_Config *config;

    config = XIicPs_LookupConfig(IIC_DEVICE_ID);
    if (config == NULL) {
        xil_printf("Error: iic_init: IIC LookupConfig\n\r");
        return XST_FAILURE;
    }

    status = XIicPs_CfgInitialize(&iic, config, config->BaseAddress);
    if (status != XST_SUCCESS) {
        xil_printf("Error: iic_init: IIC CfgInitialize\n\r");
        return XST_FAILURE;
    }

    status = XIicPs_SelfTest(&iic);
    if (status != XST_SUCCESS) {
        xil_printf("Error: iic_init: IIC SelfTest\n\r");
        return XST_FAILURE;
    }

    XIicPs_SetSClk(&iic, IIC_CLOCK_SPEED);
    XIicPs_SetOptions(&iic, XIICPS_7_BIT_ADDR_OPTION);

    return XST_SUCCESS;
}

// Read an 8 bit value from an 8 bit register
int iic_readReg(uint8_t reg, uint8_t *result) {
    int status;
    uint8_t buffer[1];

    buffer[0] = reg;

    status = XIicPs_MasterSendPolled(&iic, buffer, 1, IIC_SLAVE_ADDR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: iic_readReg: IIC Send\n\r");
        return XST_FAILURE;
    }

    status = XIicPs_MasterRecvPolled(&iic, result, 1, IIC_SLAVE_ADDR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: iic_readReg: IIC Receive\n\r");
        return XST_FAILURE;
    }

    //    xil_printf("READ registry: %02x = %02x\n\r", reg, *result);

    return XST_SUCCESS;
}

int iic_readReg16(uint8_t reg, uint16_t *result) {
    int status;
    uint8_t sendBuffer[1];
    uint8_t receiveBuffer[2];

    sendBuffer[0] = reg;

    status = XIicPs_MasterSendPolled(&iic, sendBuffer, 1, IIC_SLAVE_ADDR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: iic_readReg16: IIC Send\n\r");
        return XST_FAILURE;
    }

    status = XIicPs_MasterRecvPolled(&iic, receiveBuffer, 1, IIC_SLAVE_ADDR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: iic_readReg16: IIC Receive\n\r");
        return XST_FAILURE;
    }

    xil_printf("receiveBuffer[0]: %d\n\r", receiveBuffer[0]);
    xil_printf("receiveBuffer[1]: %d\n\r", receiveBuffer[1]);

    *result = (receiveBuffer[0] << 8) | receiveBuffer[1];

    xil_printf("READ registry: %02x = %04x\n\r", reg, *result);

    return XST_SUCCESS;
}

int iic_readRegArray(uint8_t reg, uint8_t *data, int size) {
    int status;
    uint8_t sendBuffer[1];

    sendBuffer[0] = reg;

    status = XIicPs_MasterSendPolled(&iic, sendBuffer, 1, IIC_SLAVE_ADDR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: iic_readRegArray: IIC Send\n\r");
        return XST_FAILURE;
    }

    status = XIicPs_MasterRecvPolled(&iic, data, size, IIC_SLAVE_ADDR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: iic_readRegArray: IIC Receive\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

// Write an 8 bit value to an 8 bit register
int iic_writeReg(uint8_t reg, uint8_t data) {
    int status;
    uint8_t buffer[2];

    buffer[0] = reg;
    buffer[1] = data;
    status = XIicPs_MasterSendPolled(&iic, buffer, 2, IIC_SLAVE_ADDR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: iic_writeReg: IIC Send\n\r");
        return XST_FAILURE;
    }

    //    xil_printf("WRITE registry: %02x = %02x\n\r", reg, data);

    return XST_SUCCESS;
}

int iic_writeReg16(uint8_t reg, uint16_t data) {
    int status;
    uint8_t buffer[3];

    buffer[0] = reg;
    buffer[1] = (data >> 8) & 0xFF;
    buffer[2] = data & 0xFF;
    status = XIicPs_MasterSendPolled(&iic, buffer, 3, IIC_SLAVE_ADDR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: iic_writeReg16: IIC Send\n\r");
        return XST_FAILURE;
    }

    //    xil_printf("WRITE registry: %02x = %04x\n\r", reg, data);

    return XST_SUCCESS;
}

int iic_writeRegArray(uint8_t reg, uint8_t *data, int size) {
    int status;
    uint8_t buffer[size + 1];

    buffer[0] = reg;
    for (int i = 0; i < size; i++) {
        buffer[i + 1] = data[i];
    }

    status = XIicPs_MasterSendPolled(&iic, buffer, size + 1, IIC_SLAVE_ADDR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: iic_writeRegArray: IIC Send\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

// Check if the connection with the sensor is working by reading the device
// model ID register
int connectionCheck() {
    int status;
    uint8_t deviceModelID;

    status = iic_readReg(REG_MODEL_ID, &deviceModelID);
    if (status != XST_SUCCESS) {
        xil_printf("Error: connectionCheck: IIC Read\n\r");
        return XST_FAILURE;
    }

    if (deviceModelID != EXPECTED_MODEL_ID) {
        xil_printf("Error: deviceModelID != 0xEE\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

// Set the IO to 2V8 mode
int config_set2v8() {
    int status;
    uint8_t currentSetting;

    status = iic_readReg(VL53L0X_REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV,
                         &currentSetting);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_set2v8: IIC Read\n\r");
        return XST_FAILURE;
    }

    status = iic_writeReg(VL53L0X_REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV,
                          currentSetting | 0x01);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_set2v8: IIC Write\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

// Set the I2C to standard mode
int config_setIicStandard() {
    int status;

    status = iic_writeReg(REG_IIC_MODE, IIC_MODE_STANDARD);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_setIicStandard: IIC Write\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

// Get the stop variable from the sensor
int config_getStopVariable() {
    int status;

    status = iic_writeReg(0x80, 0x01);
    status &= iic_writeReg(0xFF, 0x01);
    status &= iic_writeReg(0x00, 0x00);
    status &= iic_readReg(0x91, &stopVariable);
    status &= iic_writeReg(0x00, 0x01);
    status &= iic_writeReg(0xFF, 0x00);
    status &= iic_writeReg(0x80, 0x00);

    if (status != XST_SUCCESS) {
        xil_printf("Error: config_getStopVariable\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

// Setup the interrupt output as new sample ready and active low
int config_interrupts() {
    int status;

    status = iic_writeReg(REG_SYSTEM_INTERRUPT_GPIO_CONFIG,
                          SYSTEM_INTERRUPT_NEW_SAMPLE_READY);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_interrupt: IIC Write\n\r");
        return XST_FAILURE;
    }

    uint8_t currentSetting;
    status = iic_readReg(REG_GPIO_HV_MUX_ACTIVE_HIGH, &currentSetting);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_interrupt: IIC Read\n\r");
        return XST_FAILURE;
    }

    status = iic_writeReg(REG_GPIO_HV_MUX_ACTIVE_HIGH,
                          currentSetting & MASK_INTERRUPT_ACTIVE_LOW);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_interrupt: IIC Write\n\r");
        return XST_FAILURE;
    }

    status = iic_writeReg(REG_SYSTEM_INTERRUPT_CLEAR, SYSTEM_INTERRUPT_CLEAR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_interrupt: IIC Write\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

// Set sequence steps
int config_setSequenceSteps(uint8_t setting) {
    int status;

    status = iic_writeReg(REG_SYSTEM_SEQUENCE_CONFIG, setting);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_sequenceSteps: IIC Write\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int config_disableRateChecks() {
    int status;
    uint8_t currentSetting;

    status = iic_readReg(REG_MSRC_CONFIG_CONTROL, &currentSetting);
    if (status != XST_SUCCESS) {
        xil_printf("Error: congfig_disableRateChecks: IIC Read\n\r");
        return XST_FAILURE;
    }

    status = iic_writeReg(REG_MSRC_CONFIG_CONTROL, currentSetting | 0x12);
    if (status != XST_SUCCESS) {
        xil_printf("Error: congfig_disableRateChecks: IIC Write\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int config_setSignalRateLimit(float limit_Mcps) {
    if (limit_Mcps < 0 || limit_Mcps > 511.99) {
        xil_printf(
            "Error: config_setSignalRateLimit: limit_Mcps out of range\n\r");
        return XST_FAILURE;
    }

    int status;
    uint16_t limit = limit_Mcps * (1 << 7);
    status = iic_writeReg16(0x44, limit);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_setSignalRateLimit: IIC Write\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

// Load the default tuning settings (from the API)
int config_loadDefaultTuning() {
    int status;

    status = iic_writeReg(0xFF, 0x01);
    status &= iic_writeReg(0x00, 0x00);
    status &= iic_writeReg(0xFF, 0x00);
    status &= iic_writeReg(0x09, 0x00);
    status &= iic_writeReg(0x10, 0x00);
    status &= iic_writeReg(0x11, 0x00);
    status &= iic_writeReg(0x24, 0x01);
    status &= iic_writeReg(0x25, 0xFF);
    status &= iic_writeReg(0x75, 0x00);
    status &= iic_writeReg(0xFF, 0x01);
    status &= iic_writeReg(0x4E, 0x2C);
    status &= iic_writeReg(0x48, 0x00);
    status &= iic_writeReg(0x30, 0x20);
    status &= iic_writeReg(0xFF, 0x00);
    status &= iic_writeReg(0x30, 0x09);
    status &= iic_writeReg(0x54, 0x00);
    status &= iic_writeReg(0x31, 0x04);
    status &= iic_writeReg(0x32, 0x03);
    status &= iic_writeReg(0x40, 0x83);
    status &= iic_writeReg(0x46, 0x25);
    status &= iic_writeReg(0x60, 0x00);
    status &= iic_writeReg(0x27, 0x00);
    status &= iic_writeReg(0x50, 0x06);
    status &= iic_writeReg(0x51, 0x00);
    status &= iic_writeReg(0x52, 0x96);
    status &= iic_writeReg(0x56, 0x08);
    status &= iic_writeReg(0x57, 0x30);
    status &= iic_writeReg(0x61, 0x00);
    status &= iic_writeReg(0x62, 0x00);
    status &= iic_writeReg(0x64, 0x00);
    status &= iic_writeReg(0x65, 0x00);
    status &= iic_writeReg(0x66, 0xA0);
    status &= iic_writeReg(0xFF, 0x01);
    status &= iic_writeReg(0x22, 0x32);
    status &= iic_writeReg(0x47, 0x14);
    status &= iic_writeReg(0x49, 0xFF);
    status &= iic_writeReg(0x4A, 0x00);
    status &= iic_writeReg(0xFF, 0x00);
    status &= iic_writeReg(0x7A, 0x0A);
    status &= iic_writeReg(0x7B, 0x00);
    status &= iic_writeReg(0x78, 0x21);
    status &= iic_writeReg(0xFF, 0x01);
    status &= iic_writeReg(0x23, 0x34);
    status &= iic_writeReg(0x42, 0x00);
    status &= iic_writeReg(0x44, 0xFF);
    status &= iic_writeReg(0x45, 0x26);
    status &= iic_writeReg(0x46, 0x05);
    status &= iic_writeReg(0x40, 0x40);
    status &= iic_writeReg(0x0E, 0x06);
    status &= iic_writeReg(0x20, 0x1A);
    status &= iic_writeReg(0x43, 0x40);
    status &= iic_writeReg(0xFF, 0x00);
    status &= iic_writeReg(0x34, 0x03);
    status &= iic_writeReg(0x35, 0x44);
    status &= iic_writeReg(0xFF, 0x01);
    status &= iic_writeReg(0x31, 0x04);
    status &= iic_writeReg(0x4B, 0x09);
    status &= iic_writeReg(0x4C, 0x05);
    status &= iic_writeReg(0x4D, 0x04);
    status &= iic_writeReg(0xFF, 0x00);
    status &= iic_writeReg(0x44, 0x00);
    status &= iic_writeReg(0x45, 0x20);
    status &= iic_writeReg(0x47, 0x08);
    status &= iic_writeReg(0x48, 0x28);
    status &= iic_writeReg(0x67, 0x00);
    status &= iic_writeReg(0x70, 0x04);
    status &= iic_writeReg(0x71, 0x01);
    status &= iic_writeReg(0x72, 0xFE);
    status &= iic_writeReg(0x76, 0x00);
    status &= iic_writeReg(0x77, 0x00);
    status &= iic_writeReg(0xFF, 0x01);
    status &= iic_writeReg(0x0D, 0x01);
    status &= iic_writeReg(0xFF, 0x00);
    status &= iic_writeReg(0x80, 0x01);
    status &= iic_writeReg(0x01, 0xF8);
    status &= iic_writeReg(0xFF, 0x01);
    status &= iic_writeReg(0x8E, 0x01);
    status &= iic_writeReg(0x00, 0x01);
    status &= iic_writeReg(0xFF, 0x00);
    status &= iic_writeReg(0x80, 0x00);

    if (status != XST_SUCCESS) {
        xil_printf("Error: config_loadDefaultTuning\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int config_getSpadInfo(uint8_t *count, int *type_is_aperture) {
    uint8_t tmp;

    iic_writeReg(0x80, 0x01);
    iic_writeReg(0xFF, 0x01);
    iic_writeReg(0x00, 0x00);

    iic_writeReg(0xFF, 0x06);
    iic_readReg(0x83, &tmp);
    iic_writeReg(0x83, tmp | 0x04);
    iic_writeReg(0xFF, 0x07);
    iic_writeReg(0x81, 0x01);

    iic_writeReg(0x80, 0x01);

    iic_writeReg(0x94, 0x6b);
    iic_writeReg(0x83, 0x00);

    int finished = 0;
    while (finished == 0) {
        iic_readReg(0x83, &tmp);
        finished = tmp != 0x00;
    }
    iic_writeReg(0x83, 0x01);
    iic_readReg(0x92, &tmp);

    *count = tmp & 0x7f;
    *type_is_aperture = (tmp >> 7) & 0x01;

    iic_writeReg(0x81, 0x00);
    iic_writeReg(0xFF, 0x06);
    iic_readReg(0x83, &tmp);
    iic_writeReg(0x83, tmp & ~0x04);
    iic_writeReg(0xFF, 0x01);
    iic_writeReg(0x00, 0x01);

    iic_writeReg(0xFF, 0x00);
    iic_writeReg(0x80, 0x00);

    return XST_SUCCESS;
}

int config_setupSpads() {
    int status;
    uint8_t spadCount;
    int spadIsGood;

    status = config_getSpadInfo(&spadCount, &spadIsGood);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_setupSpads: config_getSpadInfo\n\r");
        return XST_FAILURE;
    }

    uint8_t spadMap[6];
    status = iic_readRegArray(0xB0, spadMap, 6);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_setupSpads: IIC Read\n\r");
        return XST_FAILURE;
    }

    iic_writeReg(0xFF, 0x01);
    iic_writeReg(DYNAMIC_SPAD_REF_EN_START_OFFSET, 0x00);
    iic_writeReg(DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD, 0x2C);
    iic_writeReg(0xFF, 0x00);
    iic_writeReg(GLOBAL_CONFIG_REF_EN_START_SELECT, 0xB4);

    uint8_t first_spad_to_enable =
        spadIsGood ? 12 : 0;  // 12 is the first aperture spad
    uint8_t spads_enabled = 0;

    for (uint8_t i = 0; i < 48; i++) {
        if (i < first_spad_to_enable || spads_enabled == spadCount) {
            spadMap[i / 8] &= ~(1 << (i % 8));
        } else if ((spadMap[i / 8] >> (i % 8)) & 0x1) {
            spads_enabled++;
        }
    }

    iic_writeRegArray(0xB0, spadMap, 6);
}

int config_setTimingBudget() {
    int status;
    uint32_t currentTimingBudget;

    return XST_SUCCESS;
}

// Setup the sensor config
int config_init() {
    int status;

    status = config_set2v8();
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: config_set2v8\n\r");
        return XST_FAILURE;
    }

    status = config_setIicStandard();
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: config_setIicStandard\n\r");
        return XST_FAILURE;
    }

    status = config_getStopVariable();
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: config_getStopVariable\n\r");
        return XST_FAILURE;
    }

    status = config_disableRateChecks();
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: congfig_disableRateChecks\n\r");
        return XST_FAILURE;
    }

    status = config_setSignalRateLimit(0.25);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: config_setSignalRateLimit\n\r");
        return XST_FAILURE;
    }

    status = config_setSequenceSteps(0xFF);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: config_setSequenceSteps\n\r");
        return XST_FAILURE;
    }

    status = config_setupSpads();
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: config_setupSpads\n\r");
        return XST_FAILURE;
    }

    status = config_loadDefaultTuning();
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: config_loadDefaultTuning\n\r");
        return XST_FAILURE;
    }

    status = config_interrupts();
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: config_interrupt\n\r");
        return XST_FAILURE;
    }

    status = config_setSequenceSteps(0x01);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: config_setSequenceSteps\n\r");
        return XST_FAILURE;
    }

    status = config_setTimingBudget();
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: config_setTimingBudget\n\r");
        return XST_FAILURE;
    }

    status = config_setSequenceSteps(0xE8);
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: config_setSequenceSteps\n\r");
        return XST_FAILURE;
    }

    status = calibration_start();
    if (status != XST_SUCCESS) {
        xil_printf("Error: config_init: calibration_start\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int calibration_vhv() {
    int status;

    uint8_t sysrange_start = 0x01 | 0x40;
    uint8_t sequence_config = 0x01;

    status = iic_writeReg(REG_SYSTEM_SEQUENCE_CONFIG, sequence_config);
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_vhv: IIC Write\n\r");
        return XST_FAILURE;
    }

    status = iic_writeReg(REG_SYSRANGE_START, sysrange_start);
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_vhv: IIC Write\n\r");
        return XST_FAILURE;
    }

    uint8_t interruptStatus = 0;
    do {
        status = iic_readReg(REG_RESULT_RANGE_STATUS, &interruptStatus);
    } while (status && ((interruptStatus & 0x07) == 0));
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_vhv: IIC Read\n\r");
    }

    status = iic_writeReg(REG_SYSTEM_INTERRUPT_CLEAR, SYSTEM_INTERRUPT_CLEAR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_vhv: IIC Write\n\r");
        return XST_FAILURE;
    }

    status = iic_writeReg(REG_SYSRANGE_START, 0x00);
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_vhv: IIC Write\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int calibration_phase() {
    int status;

    uint8_t sysrange_start = 0x01 | 0x00;
    uint8_t sequence_config = 0x02;

    status = iic_writeReg(REG_SYSTEM_SEQUENCE_CONFIG, sequence_config);
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_vhv: IIC Write\n\r");
        return XST_FAILURE;
    }

    status = iic_writeReg(REG_SYSRANGE_START, sysrange_start);
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_vhv: IIC Write\n\r");
        return XST_FAILURE;
    }

    uint8_t interruptStatus = 0;
    do {
        status = iic_readReg(REG_RESULT_RANGE_STATUS, &interruptStatus);
    } while (status && ((interruptStatus & 0x07) == 0));
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_vhv: IIC Read\n\r");
    }

    status = iic_writeReg(REG_SYSTEM_INTERRUPT_CLEAR, SYSTEM_INTERRUPT_CLEAR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_vhv: IIC Write\n\r");
        return XST_FAILURE;
    }

    status = iic_writeReg(REG_SYSRANGE_START, 0x00);
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_vhv: IIC Write\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int calibration_start() {
    int status;

    status = calibration_vhv();
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_start: calibration_vhv\n\r");
        return XST_FAILURE;
    }

    status = calibration_phase();
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_start: calibration_phase\n\r");
        return XST_FAILURE;
    }

    status = config_setSequenceSteps(0xE8);
    if (status != XST_SUCCESS) {
        xil_printf("Error: calibration_start: config_setSequenceSteps\n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int measurement_start(uint16_t *range) {
    int status;

    status = iic_writeReg(0x80, 0x01);
    status &= iic_writeReg(0xFF, 0x01);
    status &= iic_writeReg(0x00, 0x00);
    status &= iic_writeReg(0x91, stopVariable);
    status &= iic_writeReg(0x00, 0x01);
    status &= iic_writeReg(0xFF, 0x00);
    status &= iic_writeReg(0x80, 0x00);

    if (status != XST_SUCCESS) {
        xil_printf("Error: measurement_start\n\r");
        return XST_FAILURE;
    }

    status = iic_writeReg(REG_SYSRANGE_START, SYSRANGE_ARMED);
    if (status != XST_SUCCESS) {
        xil_printf("Error: measurement_start: IIC Write\n\r");
        return XST_FAILURE;
    }

    xil_printf("Starting measurement\n\r");

    uint8_t measurementStarted = 0;
    do {
        status = iic_readReg(REG_SYSRANGE_START, &measurementStarted);
    } while (status && (measurementStarted & 0x01));
    if (status != XST_SUCCESS) {
        xil_printf("Error: measurement_start: IIC Read\n\r");
        return XST_FAILURE;
    }

    xil_printf("Measurement started\n\r");

    uint8_t interruptStatus = 0;
    do {
        status = iic_readReg(REG_RESULT_INTERRUPT_STATUS, &interruptStatus);
    } while (((interruptStatus & 0x07) == 0));
    if (status != XST_SUCCESS) {
        xil_printf("Error: measurement_start: IIC Read\n\r");
        return XST_FAILURE;
    }

    status = iic_readReg16(REG_RESULT_RANGE_STATUS + 10, range);
    if (status != XST_SUCCESS) {
        xil_printf("Error: measurement_start: IIC Read\n\r");
        return XST_FAILURE;
    }

    status = iic_writeReg(REG_SYSTEM_INTERRUPT_CLEAR, SYSTEM_INTERRUPT_CLEAR);
    if (status != XST_SUCCESS) {
        xil_printf("Error: measurement_start: IIC Write\n\r");
        return XST_FAILURE;
    }

    if (*range == 8190 || *range == 8191) {
        *range = 0;
    }

    return XST_SUCCESS;
}
