/**
 * User library for cvora
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 * David Cobas BE/CO/HT May 9th 2011
 */

#ifndef _LIBCVORA_H
#define _LIBCVORA_H

#ifdef __cplusplus
extern "C"
{
#endif

/** cvora modes of operation */
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
 * Hardware definitions for registers defined in cvora_hard.h
 * This file defines the CVORA register names and their offsets in the addressmap
 * Where a register is composed of bit fields the bit names are defined
 */
#include "cvora_hard.h"

/**
 * @brief Initialize cvora user library
 * @param lun logical unit number
 * @return file descriptor, or < 0 if error
 */

int cvora_init(int lun);

/**
 * @brief get version of the module
 * @param fd  file descriptor returned from cvora_init
 * @param version	module version
 * @return 0 if OK, < 0 if error
 */
int cvora_get_version(int fd, int *version);

/**
 * @brief get mode
 * @param fd  file descriptor returned from cvora_init
 * @param mode one of the CVORA modes of operation
 * @return 0 if OK, < 0 if error
 */
int cvora_get_mode(int fd, enum cvora_mode *mode);

/**
 * @brief set mode
 * @param fd  file descriptor returned from cvora_init
 * @param mode one of the CVORA modes of operation
 * @return 0 if OK, < 0 if error
 */
int cvora_set_mode(int fd, enum cvora_mode mode);

/**
 * @brief read status register (DEPRECATED)
 * @param fd  file descriptor returned from cvora_init
 * @param status a bitmask of status bits
 * @return 0 if OK, < 0 if error
 */
int cvora_get_hardware_status(int fd, unsigned int *status);

/**
 * @brief set pulse polarity
 * @param fd  file descriptor returned from cvora_init
 * @param polarity polarity
		(POSITIVE = 1 like in CTRx,
		 NEGATIVE = 0 * like in TG8)
 * @return 0 if OK, < 0 if error
 */
int cvora_set_pulse_polarity(int fd, int polarity);

/**
 * @brief get pulse polarity
 * @param fd  file descriptor returned from cvora_init
 * @param polarity returned polarity
		(POSITIVE = 1 like in CTRx,
		 NEGATIVE = 0 * like in TG8)
 * @return 0 if OK, < 0 if error
 */
int cvora_get_pulse_polarity(int fd, int *polarity);

/**
 * @brief set module enable bit
 * @param fd  file descriptor returned from cvora_init
 * @param enabled (1 to enable, 0 to disable module)
 * @return 0 if OK, < 0 if error
 */
int cvora_set_module_enabled(int fd, int enabled);

/**
 * @brief get module enable bit
 * @param fd  file descriptor returned from cvora_init
 * @param enabled module status (1 = enabled, 0 = disabled)
 * @return 0 if OK, < 0 if error
 */
int cvora_get_module_enabled(int fd, int *enabled);

/**
 * @brief set IRQ status bit
 * @param fd  file descriptor returned from cvora_init
 * @param enabled 1 to enable interrupts, 0 to disable
 * @return 0 if OK, < 0 if error
 */
int cvora_set_irq_enabled(int fd, int enabled);

/**
 * @brief get IRQ status bit
 * @param fd  file descriptor returned from cvora_init
 * @param enabled 1 = interrupts enabled, 0 = ints disabled
 * @return 0 if OK, < 0 if error
 */
int cvora_get_irq_enabled(int fd, int *enabled);

/**
 * @brief enable interrupts from the module
 * @param fd  file descriptor returned from cvora_init
 * @return 0 if OK, < 0 if error
 */
int cvora_enable_interrupts(int fd);

/**
 * @brief disable interrupts from the module
 * @param fd  file descriptor returned from cvora_init
 * @return 0 if OK, < 0 if error
 */
int cvora_disable_interrupts(int fd);

/**
 * @brief set irq vector (DEPRECATED)
 * @param fd  file descriptor returned from cvora_init
 * @param vector vector value
 * @return 0 if OK, < 0 if error
 */
int cvora_set_irq_vector(int fd, int vector);

/**
 * @brief get irq vector (DEPRECATED)
 * @param fd  file descriptor returned from cvora_init
 * @param vector IRQ vector value
 * @return 0 if OK, < 0 if error
 */
int cvora_get_irq_vector(int fd, int *vector);

/**
 * @brief wait for end of sample interrupt.
 * This function blocks until an interrupt occurs, which signals a stop
 * acquisition event
 * @param fd  file descriptor returned from cvora_init
 * @return 0 if OK, < 0 if error
 */
int cvora_wait(int fd);

/**
 * @brief read memory buffer samples size
 * @param fd  file descriptor returned from cvora_init
 * @param memsz available memory size in bytes
 * @return 0 if OK, < 0 if error
 */
int cvora_get_sample_size(int fd, int *memsz);

/**
 * @brief Read memory sample buffer
 * @param fd  file descriptor returned from cvora_init
 * @param maxsz max size to read
 * @param actsz actual size read
 * @param buf pointer to data area
 * @return 0 if OK, < 0 if error
 */
int cvora_read_samples(int fd, int maxsz, int *actsz, unsigned int *buf);

/**
 * @brief Issue a software start
 * @param fd  file descriptor returned from cvora_init
 * @return 0 if OK, < 0 if error
 */
int cvora_soft_start(int fd);

/**
 * @brief Issue a software stop
 * @param fd  file descriptor returned from cvora_init
 * @return 0 if OK, < 0 if error
 */
int cvora_soft_stop(int fd);

/**
 * @brief read DAC
 * @param fd  file descriptor returned from cvora_init
 * @param dacv dac value
 * @return 0 if OK, < 0 if error
 */
int cvora_get_dac(int fd, unsigned int *dacv);

/**
 * @brief read clock frequency
 * @param fd  file descriptor returned from cvora_init
 * @param freq frequency value
 * @return 0 if OK, < 0 if error
 */
int cvora_get_clock_frequency(int fd, unsigned int *freq);

/**
 * @brief set plot input
 * @param fd  file descriptor returned from cvora_init
 * @param plti plot input P2 channel routed to output DAC 1..32
 * @return 0 if OK, < 0 if error
 */
int cvora_set_plot_input(int fd, unsigned int plti);

/**
 * @brief read parallel channels mask
 * @param fd  file descriptor returned from cvora_init
 * @param chans value
 * @return 0 if OK, < 0 if error
 */
int cvora_get_channels_mask(int fd, unsigned int *chans);

/**
 * @brief set parallel channels mask
 * @param fd  file descriptor returned from cvora_init
 * @param chans value
 * @return 0 if OK, < 0 if error
 */
int cvora_set_channels_mask(int fd, unsigned int chans);


#ifdef __cplusplus
}
#endif
#endif	/* _LIBCVORA_H */

