#ifndef XPARAMETERS_H   /* prevent circular inclusions */
#define XPARAMETERS_H   /* by using protection macros */

/* Definition for CPU ID */
#define XPAR_CPU_ID 0U

/* Definitions for peripheral PS7_CORTEXA9_0 */
#define XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ 650000000


/******************************************************************/

/* Canonical definitions for peripheral PS7_CORTEXA9_0 */
#define XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ 650000000


/******************************************************************/

#include "xparameters_ps.h"

#define STDIN_BASEADDRESS 0xE0000000
#define STDOUT_BASEADDRESS 0xE0000000

/******************************************************************/

/* Platform specific definitions */
#define PLATFORM_ZYNQ
 
/* Definitions for sleep timer configuration */
#define XSLEEP_TIMER_IS_DEFAULT_TIMER
 
 
/******************************************************************/

/* Definitions for peripheral PS7_DDR_0 */
#define XPAR_PS7_DDR_0_S_AXI_BASEADDR 0x00100000
#define XPAR_PS7_DDR_0_S_AXI_HIGHADDR 0x1FFFFFFF


/******************************************************************/

/* Definitions for driver DEVCFG */
#define XPAR_XDCFG_NUM_INSTANCES 1U

/* Definitions for peripheral PS7_DEV_CFG_0 */
#define XPAR_PS7_DEV_CFG_0_DEVICE_ID 0U
#define XPAR_PS7_DEV_CFG_0_BASEADDR 0xF8007000U
#define XPAR_PS7_DEV_CFG_0_HIGHADDR 0xF80070FFU


/******************************************************************/

/* Canonical definitions for peripheral PS7_DEV_CFG_0 */
#define XPAR_XDCFG_0_DEVICE_ID XPAR_PS7_DEV_CFG_0_DEVICE_ID
#define XPAR_XDCFG_0_BASEADDR 0xF8007000U
#define XPAR_XDCFG_0_HIGHADDR 0xF80070FFU


/******************************************************************/

/* Definitions for driver DMAPS */
#define XPAR_XDMAPS_NUM_INSTANCES 2

/* Definitions for peripheral PS7_DMA_NS */
#define XPAR_PS7_DMA_NS_DEVICE_ID 0
#define XPAR_PS7_DMA_NS_BASEADDR 0xF8004000
#define XPAR_PS7_DMA_NS_HIGHADDR 0xF8004FFF


/* Definitions for peripheral PS7_DMA_S */
#define XPAR_PS7_DMA_S_DEVICE_ID 1
#define XPAR_PS7_DMA_S_BASEADDR 0xF8003000
#define XPAR_PS7_DMA_S_HIGHADDR 0xF8003FFF


/******************************************************************/

/* Canonical definitions for peripheral PS7_DMA_NS */
#define XPAR_XDMAPS_0_DEVICE_ID XPAR_PS7_DMA_NS_DEVICE_ID
#define XPAR_XDMAPS_0_BASEADDR 0xF8004000
#define XPAR_XDMAPS_0_HIGHADDR 0xF8004FFF

/* Canonical definitions for peripheral PS7_DMA_S */
#define XPAR_XDMAPS_1_DEVICE_ID XPAR_PS7_DMA_S_DEVICE_ID
#define XPAR_XDMAPS_1_BASEADDR 0xF8003000
#define XPAR_XDMAPS_1_HIGHADDR 0xF8003FFF


/******************************************************************/


/* Peripheral Definitions for peripheral PS7_AFI_0 */
#define XPAR_PS7_AFI_0_S_AXI_BASEADDR 0xF8008000
#define XPAR_PS7_AFI_0_S_AXI_HIGHADDR 0xF8008FFF


/* Peripheral Definitions for peripheral PS7_AFI_1 */
#define XPAR_PS7_AFI_1_S_AXI_BASEADDR 0xF8009000
#define XPAR_PS7_AFI_1_S_AXI_HIGHADDR 0xF8009FFF


/* Peripheral Definitions for peripheral PS7_AFI_2 */
#define XPAR_PS7_AFI_2_S_AXI_BASEADDR 0xF800A000
#define XPAR_PS7_AFI_2_S_AXI_HIGHADDR 0xF800AFFF


/* Peripheral Definitions for peripheral PS7_AFI_3 */
#define XPAR_PS7_AFI_3_S_AXI_BASEADDR 0xF800B000
#define XPAR_PS7_AFI_3_S_AXI_HIGHADDR 0xF800BFFF


/* Peripheral Definitions for peripheral PS7_DDRC_0 */
#define XPAR_PS7_DDRC_0_S_AXI_BASEADDR 0xF8006000
#define XPAR_PS7_DDRC_0_S_AXI_HIGHADDR 0xF8006FFF


/* Peripheral Definitions for peripheral PS7_GLOBALTIMER_0 */
#define XPAR_PS7_GLOBALTIMER_0_S_AXI_BASEADDR 0xF8F00200
#define XPAR_PS7_GLOBALTIMER_0_S_AXI_HIGHADDR 0xF8F002FF


/* Peripheral Definitions for peripheral PS7_GPV_0 */
#define XPAR_PS7_GPV_0_S_AXI_BASEADDR 0xF8900000
#define XPAR_PS7_GPV_0_S_AXI_HIGHADDR 0xF89FFFFF


/* Peripheral Definitions for peripheral PS7_INTC_DIST_0 */
#define XPAR_PS7_INTC_DIST_0_S_AXI_BASEADDR 0xF8F01000
#define XPAR_PS7_INTC_DIST_0_S_AXI_HIGHADDR 0xF8F01FFF


/* Peripheral Definitions for peripheral PS7_IOP_BUS_CONFIG_0 */
#define XPAR_PS7_IOP_BUS_CONFIG_0_S_AXI_BASEADDR 0xE0200000
#define XPAR_PS7_IOP_BUS_CONFIG_0_S_AXI_HIGHADDR 0xE0200FFF


/* Peripheral Definitions for peripheral PS7_L2CACHEC_0 */
#define XPAR_PS7_L2CACHEC_0_S_AXI_BASEADDR 0xF8F02000
#define XPAR_PS7_L2CACHEC_0_S_AXI_HIGHADDR 0xF8F02FFF


/* Peripheral Definitions for peripheral PS7_OCMC_0 */
#define XPAR_PS7_OCMC_0_S_AXI_BASEADDR 0xF800C000
#define XPAR_PS7_OCMC_0_S_AXI_HIGHADDR 0xF800CFFF


/* Peripheral Definitions for peripheral PS7_PL310_0 */
#define XPAR_PS7_PL310_0_S_AXI_BASEADDR 0xF8F02000
#define XPAR_PS7_PL310_0_S_AXI_HIGHADDR 0xF8F02FFF


/* Peripheral Definitions for peripheral PS7_PMU_0 */
#define XPAR_PS7_PMU_0_S_AXI_BASEADDR 0xF8891000
#define XPAR_PS7_PMU_0_S_AXI_HIGHADDR 0xF8891FFF
#define XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR 0xF8893000
#define XPAR_PS7_PMU_0_PMU1_S_AXI_HIGHADDR 0xF8893FFF


/* Peripheral Definitions for peripheral PS7_QSPI_LINEAR_0 */
#define XPAR_PS7_QSPI_LINEAR_0_S_AXI_BASEADDR 0xFC000000
#define XPAR_PS7_QSPI_LINEAR_0_S_AXI_HIGHADDR 0xFCFFFFFF


/* Peripheral Definitions for peripheral PS7_RAM_0 */
#define XPAR_PS7_RAM_0_S_AXI_BASEADDR 0x00000000
#define XPAR_PS7_RAM_0_S_AXI_HIGHADDR 0x0003FFFF


/* Peripheral Definitions for peripheral PS7_RAM_1 */
#define XPAR_PS7_RAM_1_S_AXI_BASEADDR 0xFFFC0000
#define XPAR_PS7_RAM_1_S_AXI_HIGHADDR 0xFFFFFFFF


/* Peripheral Definitions for peripheral PS7_SCUC_0 */
#define XPAR_PS7_SCUC_0_S_AXI_BASEADDR 0xF8F00000
#define XPAR_PS7_SCUC_0_S_AXI_HIGHADDR 0xF8F000FC


/* Peripheral Definitions for peripheral PS7_SLCR_0 */
#define XPAR_PS7_SLCR_0_S_AXI_BASEADDR 0xF8000000
#define XPAR_PS7_SLCR_0_S_AXI_HIGHADDR 0xF8000FFF


/******************************************************************/






































/******************************************************************/

/* Definitions for driver GPIO */
#define XPAR_XGPIO_NUM_INSTANCES 8

/* Definitions for peripheral LIJN_HERKENNING_AXI_GPIO_0 */
#define XPAR_LIJN_HERKENNING_AXI_GPIO_0_BASEADDR 0x41200000
#define XPAR_LIJN_HERKENNING_AXI_GPIO_0_HIGHADDR 0x4120FFFF
#define XPAR_LIJN_HERKENNING_AXI_GPIO_0_DEVICE_ID 0
#define XPAR_LIJN_HERKENNING_AXI_GPIO_0_INTERRUPT_PRESENT 0
#define XPAR_LIJN_HERKENNING_AXI_GPIO_0_IS_DUAL 0


/* Definitions for peripheral MOTOR_DRIVER_AXI_GPIO_0 */
#define XPAR_MOTOR_DRIVER_AXI_GPIO_0_BASEADDR 0x41220000
#define XPAR_MOTOR_DRIVER_AXI_GPIO_0_HIGHADDR 0x4122FFFF
#define XPAR_MOTOR_DRIVER_AXI_GPIO_0_DEVICE_ID 1
#define XPAR_MOTOR_DRIVER_AXI_GPIO_0_INTERRUPT_PRESENT 0
#define XPAR_MOTOR_DRIVER_AXI_GPIO_0_IS_DUAL 0


/* Definitions for peripheral OBSTAKELDETECTIE_AXI_GPIO_0 */
#define XPAR_OBSTAKELDETECTIE_AXI_GPIO_0_BASEADDR 0x41210000
#define XPAR_OBSTAKELDETECTIE_AXI_GPIO_0_HIGHADDR 0x4121FFFF
#define XPAR_OBSTAKELDETECTIE_AXI_GPIO_0_DEVICE_ID 2
#define XPAR_OBSTAKELDETECTIE_AXI_GPIO_0_INTERRUPT_PRESENT 1
#define XPAR_OBSTAKELDETECTIE_AXI_GPIO_0_IS_DUAL 0


/* Definitions for peripheral SPEED_SENSORS_AXI_GPIO_0 */
#define XPAR_SPEED_SENSORS_AXI_GPIO_0_BASEADDR 0x41230000
#define XPAR_SPEED_SENSORS_AXI_GPIO_0_HIGHADDR 0x4123FFFF
#define XPAR_SPEED_SENSORS_AXI_GPIO_0_DEVICE_ID 3
#define XPAR_SPEED_SENSORS_AXI_GPIO_0_INTERRUPT_PRESENT 1
#define XPAR_SPEED_SENSORS_AXI_GPIO_0_IS_DUAL 0


/* Definitions for peripheral USER_INTERFACE_BTNS_GPIO */
#define XPAR_USER_INTERFACE_BTNS_GPIO_BASEADDR 0x41240000
#define XPAR_USER_INTERFACE_BTNS_GPIO_HIGHADDR 0x4124FFFF
#define XPAR_USER_INTERFACE_BTNS_GPIO_DEVICE_ID 4
#define XPAR_USER_INTERFACE_BTNS_GPIO_INTERRUPT_PRESENT 1
#define XPAR_USER_INTERFACE_BTNS_GPIO_IS_DUAL 0


/* Definitions for peripheral USER_INTERFACE_LEDS_GPIO */
#define XPAR_USER_INTERFACE_LEDS_GPIO_BASEADDR 0x41250000
#define XPAR_USER_INTERFACE_LEDS_GPIO_HIGHADDR 0x4125FFFF
#define XPAR_USER_INTERFACE_LEDS_GPIO_DEVICE_ID 5
#define XPAR_USER_INTERFACE_LEDS_GPIO_INTERRUPT_PRESENT 0
#define XPAR_USER_INTERFACE_LEDS_GPIO_IS_DUAL 0


/* Definitions for peripheral USER_INTERFACE_RGB_LEDS_GPIO */
#define XPAR_USER_INTERFACE_RGB_LEDS_GPIO_BASEADDR 0x41260000
#define XPAR_USER_INTERFACE_RGB_LEDS_GPIO_HIGHADDR 0x4126FFFF
#define XPAR_USER_INTERFACE_RGB_LEDS_GPIO_DEVICE_ID 6
#define XPAR_USER_INTERFACE_RGB_LEDS_GPIO_INTERRUPT_PRESENT 0
#define XPAR_USER_INTERFACE_RGB_LEDS_GPIO_IS_DUAL 0


/* Definitions for peripheral USER_INTERFACE_SWITCHES_GPIO */
#define XPAR_USER_INTERFACE_SWITCHES_GPIO_BASEADDR 0x41270000
#define XPAR_USER_INTERFACE_SWITCHES_GPIO_HIGHADDR 0x4127FFFF
#define XPAR_USER_INTERFACE_SWITCHES_GPIO_DEVICE_ID 7
#define XPAR_USER_INTERFACE_SWITCHES_GPIO_INTERRUPT_PRESENT 1
#define XPAR_USER_INTERFACE_SWITCHES_GPIO_IS_DUAL 0


/******************************************************************/

/* Canonical definitions for peripheral LIJN_HERKENNING_AXI_GPIO_0 */
#define XPAR_GPIO_0_BASEADDR 0x41200000
#define XPAR_GPIO_0_HIGHADDR 0x4120FFFF
#define XPAR_GPIO_0_DEVICE_ID XPAR_LIJN_HERKENNING_AXI_GPIO_0_DEVICE_ID
#define XPAR_GPIO_0_INTERRUPT_PRESENT 0
#define XPAR_GPIO_0_IS_DUAL 0

/* Canonical definitions for peripheral MOTOR_DRIVER_AXI_GPIO_0 */
#define XPAR_GPIO_1_BASEADDR 0x41220000
#define XPAR_GPIO_1_HIGHADDR 0x4122FFFF
#define XPAR_GPIO_1_DEVICE_ID XPAR_MOTOR_DRIVER_AXI_GPIO_0_DEVICE_ID
#define XPAR_GPIO_1_INTERRUPT_PRESENT 0
#define XPAR_GPIO_1_IS_DUAL 0

/* Canonical definitions for peripheral OBSTAKELDETECTIE_AXI_GPIO_0 */
#define XPAR_GPIO_2_BASEADDR 0x41210000
#define XPAR_GPIO_2_HIGHADDR 0x4121FFFF
#define XPAR_GPIO_2_DEVICE_ID XPAR_OBSTAKELDETECTIE_AXI_GPIO_0_DEVICE_ID
#define XPAR_GPIO_2_INTERRUPT_PRESENT 1
#define XPAR_GPIO_2_IS_DUAL 0

/* Canonical definitions for peripheral SPEED_SENSORS_AXI_GPIO_0 */
#define XPAR_GPIO_3_BASEADDR 0x41230000
#define XPAR_GPIO_3_HIGHADDR 0x4123FFFF
#define XPAR_GPIO_3_DEVICE_ID XPAR_SPEED_SENSORS_AXI_GPIO_0_DEVICE_ID
#define XPAR_GPIO_3_INTERRUPT_PRESENT 1
#define XPAR_GPIO_3_IS_DUAL 0

/* Canonical definitions for peripheral USER_INTERFACE_BTNS_GPIO */
#define XPAR_GPIO_4_BASEADDR 0x41240000
#define XPAR_GPIO_4_HIGHADDR 0x4124FFFF
#define XPAR_GPIO_4_DEVICE_ID XPAR_USER_INTERFACE_BTNS_GPIO_DEVICE_ID
#define XPAR_GPIO_4_INTERRUPT_PRESENT 1
#define XPAR_GPIO_4_IS_DUAL 0

/* Canonical definitions for peripheral USER_INTERFACE_LEDS_GPIO */
#define XPAR_GPIO_5_BASEADDR 0x41250000
#define XPAR_GPIO_5_HIGHADDR 0x4125FFFF
#define XPAR_GPIO_5_DEVICE_ID XPAR_USER_INTERFACE_LEDS_GPIO_DEVICE_ID
#define XPAR_GPIO_5_INTERRUPT_PRESENT 0
#define XPAR_GPIO_5_IS_DUAL 0

/* Canonical definitions for peripheral USER_INTERFACE_RGB_LEDS_GPIO */
#define XPAR_GPIO_6_BASEADDR 0x41260000
#define XPAR_GPIO_6_HIGHADDR 0x4126FFFF
#define XPAR_GPIO_6_DEVICE_ID XPAR_USER_INTERFACE_RGB_LEDS_GPIO_DEVICE_ID
#define XPAR_GPIO_6_INTERRUPT_PRESENT 0
#define XPAR_GPIO_6_IS_DUAL 0

/* Canonical definitions for peripheral USER_INTERFACE_SWITCHES_GPIO */
#define XPAR_GPIO_7_BASEADDR 0x41270000
#define XPAR_GPIO_7_HIGHADDR 0x4127FFFF
#define XPAR_GPIO_7_DEVICE_ID XPAR_USER_INTERFACE_SWITCHES_GPIO_DEVICE_ID
#define XPAR_GPIO_7_INTERRUPT_PRESENT 1
#define XPAR_GPIO_7_IS_DUAL 0


/******************************************************************/

/* Definitions for driver GPIOPS */
#define XPAR_XGPIOPS_NUM_INSTANCES 1

/* Definitions for peripheral PS7_GPIO_0 */
#define XPAR_PS7_GPIO_0_DEVICE_ID 0
#define XPAR_PS7_GPIO_0_BASEADDR 0xE000A000
#define XPAR_PS7_GPIO_0_HIGHADDR 0xE000AFFF


/******************************************************************/

/* Canonical definitions for peripheral PS7_GPIO_0 */
#define XPAR_XGPIOPS_0_DEVICE_ID XPAR_PS7_GPIO_0_DEVICE_ID
#define XPAR_XGPIOPS_0_BASEADDR 0xE000A000
#define XPAR_XGPIOPS_0_HIGHADDR 0xE000AFFF


/******************************************************************/

/* Definitions for driver IIC */
#define XPAR_XIIC_NUM_INSTANCES 1

/* Definitions for peripheral OBSTAKELDETECTIE_AXI_IIC_0 */
#define XPAR_OBSTAKELDETECTIE_AXI_IIC_0_DEVICE_ID 0
#define XPAR_OBSTAKELDETECTIE_AXI_IIC_0_BASEADDR 0x41600000
#define XPAR_OBSTAKELDETECTIE_AXI_IIC_0_HIGHADDR 0x4160FFFF
#define XPAR_OBSTAKELDETECTIE_AXI_IIC_0_TEN_BIT_ADR 0
#define XPAR_OBSTAKELDETECTIE_AXI_IIC_0_GPO_WIDTH 1


/******************************************************************/

/* Canonical definitions for peripheral OBSTAKELDETECTIE_AXI_IIC_0 */
#define XPAR_IIC_0_DEVICE_ID XPAR_OBSTAKELDETECTIE_AXI_IIC_0_DEVICE_ID
#define XPAR_IIC_0_BASEADDR 0x41600000
#define XPAR_IIC_0_HIGHADDR 0x4160FFFF
#define XPAR_IIC_0_TEN_BIT_ADR 0
#define XPAR_IIC_0_GPO_WIDTH 1


/******************************************************************/

/* Definitions for driver IICPS */
#define XPAR_XIICPS_NUM_INSTANCES 2

/* Definitions for peripheral PS7_I2C_0 */
#define XPAR_PS7_I2C_0_DEVICE_ID 0
#define XPAR_PS7_I2C_0_BASEADDR 0xE0004000
#define XPAR_PS7_I2C_0_HIGHADDR 0xE0004FFF
#define XPAR_PS7_I2C_0_I2C_CLK_FREQ_HZ 108333336


/* Definitions for peripheral PS7_I2C_1 */
#define XPAR_PS7_I2C_1_DEVICE_ID 1
#define XPAR_PS7_I2C_1_BASEADDR 0xE0005000
#define XPAR_PS7_I2C_1_HIGHADDR 0xE0005FFF
#define XPAR_PS7_I2C_1_I2C_CLK_FREQ_HZ 108333336


/******************************************************************/

/* Canonical definitions for peripheral PS7_I2C_0 */
#define XPAR_XIICPS_0_DEVICE_ID XPAR_PS7_I2C_0_DEVICE_ID
#define XPAR_XIICPS_0_BASEADDR 0xE0004000
#define XPAR_XIICPS_0_HIGHADDR 0xE0004FFF
#define XPAR_XIICPS_0_I2C_CLK_FREQ_HZ 108333336

/* Canonical definitions for peripheral PS7_I2C_1 */
#define XPAR_XIICPS_1_DEVICE_ID XPAR_PS7_I2C_1_DEVICE_ID
#define XPAR_XIICPS_1_BASEADDR 0xE0005000
#define XPAR_XIICPS_1_HIGHADDR 0xE0005FFF
#define XPAR_XIICPS_1_I2C_CLK_FREQ_HZ 108333336


/******************************************************************/

/* Definition for input Clock */
/* Definition for input Clock */
/* Definitions for driver QSPIPS */
#define XPAR_XQSPIPS_NUM_INSTANCES 1

/* Definitions for peripheral PS7_QSPI_0 */
#define XPAR_PS7_QSPI_0_DEVICE_ID 0
#define XPAR_PS7_QSPI_0_BASEADDR 0xE000D000
#define XPAR_PS7_QSPI_0_HIGHADDR 0xE000DFFF
#define XPAR_PS7_QSPI_0_QSPI_CLK_FREQ_HZ 200000000
#define XPAR_PS7_QSPI_0_QSPI_MODE 0
#define XPAR_PS7_QSPI_0_QSPI_BUS_WIDTH 2


/******************************************************************/

/* Canonical definitions for peripheral PS7_QSPI_0 */
#define XPAR_XQSPIPS_0_DEVICE_ID XPAR_PS7_QSPI_0_DEVICE_ID
#define XPAR_XQSPIPS_0_BASEADDR 0xE000D000
#define XPAR_XQSPIPS_0_HIGHADDR 0xE000DFFF
#define XPAR_XQSPIPS_0_QSPI_CLK_FREQ_HZ 200000000
#define XPAR_XQSPIPS_0_QSPI_MODE 0
#define XPAR_XQSPIPS_0_QSPI_BUS_WIDTH 2


/******************************************************************/

/* Definitions for Fabric interrupts connected to ps7_scugic_0 */
#define XPAR_FABRIC_OBSTAKELDETECTIE_AXI_IIC_0_IIC2INTC_IRPT_INTR 62U
#define XPAR_FABRIC_OBSTAKELDETECTIE_AXI_GPIO_0_IP2INTC_IRPT_INTR 63U
#define XPAR_FABRIC_SPEED_SENSORS_AXI_GPIO_0_IP2INTC_IRPT_INTR 64U
#define XPAR_FABRIC_USER_INTERFACE_BTNS_GPIO_IP2INTC_IRPT_INTR 65U
#define XPAR_FABRIC_USER_INTERFACE_SWITCHES_GPIO_IP2INTC_IRPT_INTR 66U

/******************************************************************/

/* Canonical definitions for Fabric interrupts connected to ps7_scugic_0 */
#define XPAR_FABRIC_IIC_0_VEC_ID XPAR_FABRIC_OBSTAKELDETECTIE_AXI_IIC_0_IIC2INTC_IRPT_INTR
#define XPAR_FABRIC_GPIO_2_VEC_ID XPAR_FABRIC_OBSTAKELDETECTIE_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define XPAR_FABRIC_GPIO_3_VEC_ID XPAR_FABRIC_SPEED_SENSORS_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define XPAR_FABRIC_GPIO_4_VEC_ID XPAR_FABRIC_USER_INTERFACE_BTNS_GPIO_IP2INTC_IRPT_INTR
#define XPAR_FABRIC_GPIO_7_VEC_ID XPAR_FABRIC_USER_INTERFACE_SWITCHES_GPIO_IP2INTC_IRPT_INTR

/******************************************************************/

/* Definitions for driver SCUGIC */
#define XPAR_XSCUGIC_NUM_INSTANCES 1U

/* Definitions for peripheral PS7_SCUGIC_0 */
#define XPAR_PS7_SCUGIC_0_DEVICE_ID 0U
#define XPAR_PS7_SCUGIC_0_BASEADDR 0xF8F00100U
#define XPAR_PS7_SCUGIC_0_HIGHADDR 0xF8F001FFU
#define XPAR_PS7_SCUGIC_0_DIST_BASEADDR 0xF8F01000U


/******************************************************************/

/* Canonical definitions for peripheral PS7_SCUGIC_0 */
#define XPAR_SCUGIC_0_DEVICE_ID 0U
#define XPAR_SCUGIC_0_CPU_BASEADDR 0xF8F00100U
#define XPAR_SCUGIC_0_CPU_HIGHADDR 0xF8F001FFU
#define XPAR_SCUGIC_0_DIST_BASEADDR 0xF8F01000U


/******************************************************************/

/* Definitions for driver SCUTIMER */
#define XPAR_XSCUTIMER_NUM_INSTANCES 1

/* Definitions for peripheral PS7_SCUTIMER_0 */
#define XPAR_PS7_SCUTIMER_0_DEVICE_ID 0
#define XPAR_PS7_SCUTIMER_0_BASEADDR 0xF8F00600
#define XPAR_PS7_SCUTIMER_0_HIGHADDR 0xF8F0061F


/******************************************************************/

/* Canonical definitions for peripheral PS7_SCUTIMER_0 */
#define XPAR_XSCUTIMER_0_DEVICE_ID XPAR_PS7_SCUTIMER_0_DEVICE_ID
#define XPAR_XSCUTIMER_0_BASEADDR 0xF8F00600
#define XPAR_XSCUTIMER_0_HIGHADDR 0xF8F0061F


/******************************************************************/

/* Definitions for driver SCUWDT */
#define XPAR_XSCUWDT_NUM_INSTANCES 1

/* Definitions for peripheral PS7_SCUWDT_0 */
#define XPAR_PS7_SCUWDT_0_DEVICE_ID 0
#define XPAR_PS7_SCUWDT_0_BASEADDR 0xF8F00620
#define XPAR_PS7_SCUWDT_0_HIGHADDR 0xF8F006FF


/******************************************************************/

/* Canonical definitions for peripheral PS7_SCUWDT_0 */
#define XPAR_SCUWDT_0_DEVICE_ID XPAR_PS7_SCUWDT_0_DEVICE_ID
#define XPAR_SCUWDT_0_BASEADDR 0xF8F00620
#define XPAR_SCUWDT_0_HIGHADDR 0xF8F006FF


/******************************************************************/

/* Definitions for driver TMRCTR */
#define XPAR_XTMRCTR_NUM_INSTANCES 2U

/* Definitions for peripheral MOTOR_DRIVER_AXI_TIMER_0 */
#define XPAR_MOTOR_DRIVER_AXI_TIMER_0_DEVICE_ID 0U
#define XPAR_MOTOR_DRIVER_AXI_TIMER_0_BASEADDR 0x42800000U
#define XPAR_MOTOR_DRIVER_AXI_TIMER_0_HIGHADDR 0x4280FFFFU
#define XPAR_MOTOR_DRIVER_AXI_TIMER_0_CLOCK_FREQ_HZ 100000000U


/* Definitions for peripheral MOTOR_DRIVER_AXI_TIMER_1 */
#define XPAR_MOTOR_DRIVER_AXI_TIMER_1_DEVICE_ID 1U
#define XPAR_MOTOR_DRIVER_AXI_TIMER_1_BASEADDR 0x42810000U
#define XPAR_MOTOR_DRIVER_AXI_TIMER_1_HIGHADDR 0x4281FFFFU
#define XPAR_MOTOR_DRIVER_AXI_TIMER_1_CLOCK_FREQ_HZ 100000000U


/******************************************************************/

/* Canonical definitions for peripheral MOTOR_DRIVER_AXI_TIMER_0 */
#define XPAR_TMRCTR_0_DEVICE_ID 0U
#define XPAR_TMRCTR_0_BASEADDR 0x42800000U
#define XPAR_TMRCTR_0_HIGHADDR 0x4280FFFFU
#define XPAR_TMRCTR_0_CLOCK_FREQ_HZ XPAR_MOTOR_DRIVER_AXI_TIMER_0_CLOCK_FREQ_HZ
/* Canonical definitions for peripheral MOTOR_DRIVER_AXI_TIMER_1 */
#define XPAR_TMRCTR_1_DEVICE_ID 1U
#define XPAR_TMRCTR_1_BASEADDR 0x42810000U
#define XPAR_TMRCTR_1_HIGHADDR 0x4281FFFFU
#define XPAR_TMRCTR_1_CLOCK_FREQ_HZ XPAR_MOTOR_DRIVER_AXI_TIMER_1_CLOCK_FREQ_HZ

/******************************************************************/

/* Definitions for driver UARTPS */
#define XPAR_XUARTPS_NUM_INSTANCES 1

/* Definitions for peripheral PS7_UART_0 */
#define XPAR_PS7_UART_0_DEVICE_ID 0
#define XPAR_PS7_UART_0_BASEADDR 0xE0000000
#define XPAR_PS7_UART_0_HIGHADDR 0xE0000FFF
#define XPAR_PS7_UART_0_UART_CLK_FREQ_HZ 100000000
#define XPAR_PS7_UART_0_HAS_MODEM 0


/******************************************************************/

/* Canonical definitions for peripheral PS7_UART_0 */
#define XPAR_XUARTPS_0_DEVICE_ID XPAR_PS7_UART_0_DEVICE_ID
#define XPAR_XUARTPS_0_BASEADDR 0xE0000000
#define XPAR_XUARTPS_0_HIGHADDR 0xE0000FFF
#define XPAR_XUARTPS_0_UART_CLK_FREQ_HZ 100000000
#define XPAR_XUARTPS_0_HAS_MODEM 0


/******************************************************************/

/* Definition for input Clock */
/* Definitions for driver XADCPS */
#define XPAR_XADCPS_NUM_INSTANCES 1

/* Definitions for peripheral PS7_XADC_0 */
#define XPAR_PS7_XADC_0_DEVICE_ID 0
#define XPAR_PS7_XADC_0_BASEADDR 0xF8007100
#define XPAR_PS7_XADC_0_HIGHADDR 0xF8007120


/******************************************************************/

/* Canonical definitions for peripheral PS7_XADC_0 */
#define XPAR_XADCPS_0_DEVICE_ID XPAR_PS7_XADC_0_DEVICE_ID
#define XPAR_XADCPS_0_BASEADDR 0xF8007100
#define XPAR_XADCPS_0_HIGHADDR 0xF8007120


/******************************************************************/

/* Xilinx FAT File System Library (XilFFs) User Settings */
#define FILE_SYSTEM_USE_MKFS
#define FILE_SYSTEM_NUM_LOGIC_VOL 2
#define FILE_SYSTEM_USE_STRFUNC 0
#define FILE_SYSTEM_SET_FS_RPATH 0
#define FILE_SYSTEM_WORD_ACCESS
#endif  /* end of protection macro */
