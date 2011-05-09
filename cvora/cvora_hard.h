/**
 * =============================================================================
 * cvora hardware registers description
 * Definitions file
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 */

#ifndef CVORA_HARD
#define CVORA_HARD

/** register offsets */
#define CVORA_CONTROL		0x0
#define CVORA_MEMORY_POINTER	0x4
#define CVORA_MODE		0x8
#define CVORA_CHANNEL		0xc
#define CVORA_FREQUENCY		0x10
#define	CVORA_DAC		0x14
#define	CVORA_MEMORY		0x20

/** positions and masks within control register */
#define CVORA_POLARITY_BIT	0
#define CVORA_POLARITY_MASK	1
#define CVORA_MODULE_ENABLE_BIT	1
#define CVORA_INT_ENABLE_BIT	2
#define CVORA_SOFT_START_BIT	3
#define CVORA_SOFT_STOP_BIT	4
#define CVORA_SOFT_REARM_BIT	5
#define CVORA_COUNTER_OVERFLOW	6
#define CVORA_RAM_OVERFLOW	7
#define CVORA_VECTOR_BIT	8
#define CVORA_VECTOR_MASK	(0xff << CVORA_VECTOR_BIT)
#define CVORA_VERSION_BIT	16
#define CVORA_VERSION_MASK	(0xffff << CVORA_VERSION_BIT)

/** positions and masks withing mode register */
#define CVORA_MODE_BIT		0
#define CVORA_MODE_MASK		0x7

/** memory boundaries */
#define CVORA_MEM_MIN  0x20
#define CVORA_MEM_MAX  0x7FFFC
#define CVORA_MEM_SIZE (CVORA_MEM_MAX-CVORA_MEM_MIN)

/** polarity bit */
#define	POSITIVE	1
#define	NEGATIVE	0

enum cvora_mode {
	cvora_reserved,		/** reserved but parallel input for the moment */
	cvora_optical_16,       /** one optical input 16 bits - Input 2 is ignored */
	cvora_copper_16,        /** one copper Input 16 bits */
	cvora_btrain_counter,   /** Btrain counters */
	cvora_parallel_input,   /** parallel input */
	cvora_optical_2_16,     /** two optical inputs 16 bits */
	cvora_copper_2_16,      /** two copper Inputs 16 bits */
	cvora_serial_32,        /** 32 Serial Inputs on rear panel (P2 connector). */
};

/**
 * Register offset addresses
 * The bytes offsets in the register map
 * and their IO direction.
 */

#define CVA_OFFSET_WO_CONTROL     (0x00)
#define CVA_OFFSET_RO_STATUS      (0x00)
#define CVA_OFFSET_RW_MEM_POINTER (0x04)
#define CVA_OFFSET_WO_MODE        (0x08)
#define CVA_OFFSET_RO_MODULE_ID   (0x08)
#define CVA_OFFSET_RO_CHANNELS    (0x0C)
#define CVA_OFFSET_RO_FREQ        (0x10)
#define CVA_OFFSET_WO_PLOT        (0x10)
#define CVA_OFFSET_RO_DAC         (0x14)
#define CVA_OFFSET_RW_MEMORY      (0x20)

/**
 * Register numbers
 * Register indexes into the address map
 * using 32 bit register width
 */

#define CVA_REG_WO_CONTROL     (0)
#define CVA_REG_RO_STATUS      (0)
#define CVA_REG_RW_MEM_POINTER (1)
#define CVA_REG_WO_MODE        (2)
#define CVA_REG_RO_MODULE_ID   (2)
#define CVA_REG_RO_CHANNELS    (3)
#define CVA_REG_RO_FREQ        (4)
#define CVA_REG_WO_PLOT        (4)
#define CVA_REG_RO_DAC         (5)
#define CVA_REG_RW_MEMORY      (8)

/**
 * Command and status register ond byte
 * Bits within the status/command register
 */

#define CVA_POLARITY_POS 1
#define CVA_POLARITY_NEG 0
#define CVA_ENABLE_MODULE 1
#define CVA_DISABLE_MODULE 0
#define CVA_ENABLE_INTERRUPT 1
#define CVA_DISABLE_INTERRUPT 0
#define CVA_VECTOR_SHIFT 8

#define CVA_CMD_POLARITY         (0x01)
#define CVA_CMD_ENABLE_MODULE    (0x02)
#define CVA_CMD_ENABLE_INTERRUPT (0x04)
#define CVA_CMD_SOFT_START       (0x08)
#define CVA_CMD_SOFT_STOP        (0x10)
#define CVA_CMD_RESET_MEM_PNTR   (0x20)

#define CVA_STS_BUSY             (0x20)
#define CVA_STS_BT_OVERFLOW      (0x40)
#define CVA_STS_RAM_OVERFLOW     (0x80)

/**
 * Mode register three bits
 * Define the seven possible CVORA modes
 */

#define CVA_MDES_FIRST (1)
#define CVA_MDES_LAST (7)

#define CVA_MDE_ONE_OPTICAL      (1)
#define CVA_MDE_ONE_COPPER       (2)
#define CVA_MDE_BTRAIN           (3)
#define CVA_MDE_PARALLEL         (4)
#define CVA_MDE_TWO_OPTICAL      (5)
#define CVA_MDE_TWO_COPPER       (6)
#define CVA_MDE_SERIAL_P2        (7)

/**
 * Memory offset of CVORA sample area
 * its read only
 */

#define CVA_MEM_MIN  (0x20)
#define CVA_MEM_MAX  (0x7FFFC)
#define CVA_MEM_SIZE (0x7FFFC-0x20)

#endif
