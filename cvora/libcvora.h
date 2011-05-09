/**
 * Implement library for cvora
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 */

#ifndef _LIBCVORA_H
#define _LIBCVORA_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct cvora_handle {
	int	file;
} cvora_t;

/**
 * Hardware definitions for registers defined in cvora_hard.h
 * This file defines the CVORA register names and their offsets in the addressmap
 * Where a register is composed of bit fields the bit names are defined
 */
#include "cvora_hard.h"

/**
 * @brief Initialize cvora user library
 * @param lun logical unit number
 * @return pointer to handle or null on error, see errno
 */

cvora_t *cvora_init(int lun);

/**
 * @brief get version of the module
 * @param h handle returned from open
 * @param version	module version
 * @return 0 if OK, < 0 if error
 */
int cvora_get_version(cvora_t *h, int *version);

/**
 * @brief get mode
 * @param h handle returned from open
 * @param mode one of the CVORA modes of operation
 * @return 0 if OK, < 0 if error
 */
int cvora_get_mode(cvora_t *h, enum cvora_mode *mode);

/**
 * @brief set mode
 * @param h handle returned from open
 * @param mode one of the CVORA modes of operation
 * @return 0 if OK, < 0 if error
 */
int cvora_set_mode(cvora_t *h, enum cvora_mode mode);

/**
 * @brief read status register (DEPRECATED)
 * @param h handle returned from open
 * @param status a bitmask of status bits
 * @return 0 if OK, < 0 if error
 */
int cvora_get_hardware_status(cvora_t *h, unsigned int *status);

/**
 * @brief set pulse polarity
 * @param h handle returned from open
 * @param polarity polarity
		(POSITIVE = 1 like in CTRx,
		 NEGATIVE = 0 * like in TG8)
 * @return 0 if OK, < 0 if error
 */
int cvora_set_pulse_polarity(cvora_t *h, int polarity);

/**
 * @brief get pulse polarity
 * @param h handle returned from open
 * @param polarity returned polarity
		(POSITIVE = 1 like in CTRx,
		 NEGATIVE = 0 * like in TG8)
 * @return 0 if OK, < 0 if error
 */
int cvora_get_pulse_polarity(cvora_t *h, int *polarity);

/**
 * @brief set module enable bit
 * @param h handle returned from open
 * @param enabled (1 to enable, 0 to disable module)
 * @return 0 if OK, < 0 if error
 */
int cvora_set_module_enabled(cvora_t *h, int enabled);

/**
 * @brief get module enable bit
 * @param h handle returned from open
 * @param enabled module status (1 = enabled, 0 = disabled)
 * @return 0 if OK, < 0 if error
 */
int cvora_get_module_enabled(cvora_t *h, int *enabled);

/**
 * @brief set IRQ status bit
 * @param h handle returned from open
 * @param enabled 1 to enable interrupts, 0 to disable
 * @return 0 if OK, < 0 if error
 */
int cvora_set_irq_enabled(cvora_t *h, int enabled);

/**
 * @brief get IRQ status bit
 * @param h handle returned from open
 * @param enabled 1 = interrupts enabled, 0 = ints disabled
 * @return 0 if OK, < 0 if error
 */
int cvora_get_irq_enabled(cvora_t *h, int *enabled);

/**
 * @brief set irq vector (DEPRECATED)
 * @param h handle returned from open
 * @param vector vector value
 * @return 0 if OK, < 0 if error
 */
int cvora_set_irq_vector(cvora_t *h, int vector);

/**
 * @brief get irq vector (DEPRECATED)
 * @param h handle returned from open
 * @param vector IRQ vector value
 * @return 0 if OK, < 0 if error
 */
int cvora_get_irq_vector(cvora_t *h, int *vector);

/**
 * @brief wait for end of sample interrupt.
 * This function blocks until an interrupt occurs, which signals a stop
 * acquisition event
 * @param h handle returned from open
 * @return 0 if OK, < 0 if error
 */
int cvora_wait(cvora_t *h);

/**
 * @brief read memory buffer samples size
 * @param h handle returned from open
 * @param memsz available memory size in bytes
 * @return 0 if OK, < 0 if error
 */
int cvora_get_sample_size(cvora_t *h, int *memsz);

/**
 * @brief Read memory sample buffer
 * @param h handle returned from open
 * @param maxsz max size to read
 * @param actsz actual size read
 * @param buf pointer to data area
 * @return 0 if OK, < 0 if error
 */
int cvora_read_samples(cvora_t *h, int maxsz, int *actsz, unsigned int *buf);

/**
 * @brief Issue a software start
 * @param h handle returned from open
 * @return 0 if OK, < 0 if error
 */
int cvora_soft_start(cvora_t *h);

/**
 * @brief Issue a software stop
 * @param h handle returned from open
 * @return 0 if OK, < 0 if error
 */
int cvora_soft_stop(cvora_t *h);

/**
 * @brief read DAC
 * @param h handle returned from open
 * @param dacv dac value
 * @return 0 if OK, < 0 if error
 */
int cvora_get_dac(cvora_t *h, unsigned int *dacv);

/**
 * @brief read clock frequency
 * @param h handle returned from open
 * @param freq frequency value
 * @return 0 if OK, < 0 if error
 */
int cvora_get_clock_frequency(cvora_t *h, unsigned int *freq);

/**
 * @brief set plot input
 * @param h handle returned from open
 * @param plti plot input P2 channel routed to output DAC 1..32
 * @return 0 if OK, < 0 if error
 */
int cvora_set_plot_input(cvora_t *h, unsigned int plti);

/**
 * @brief read parallel channels mask
 * @param h handle returned from open
 * @param chans value
 * @return 0 if OK, < 0 if error
 */
int cvora_get_channels_mask(cvora_t *h, unsigned int *chans);

/**
 * @brief set parallel channels mask
 * @param h handle returned from open
 * @param chans value
 * @return 0 if OK, < 0 if error
 */
int cvora_set_channels_mask(cvora_t *h, unsigned int chans);


#ifdef __cplusplus
}
#endif

#endif	/* _LIBCVORA_H */

