/**
 * =============================================================================
 * Implement library for cvora
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 */

#ifndef CVORA_USER
#define CVORA_USER


#include "vmeio_support.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**
 *! \class User Library
 *  \brief CVORA user library API
 *  \author Julian Lewis
 *  \version 1.0
 *  \date Mon 8th November 2010
 *  \par About the support library
 * Include vmeio_support.h. Here is the list of all the
 * predefined cvora support library routines on which this library
 * can make calls. See vmeio_support.h if DRV_NAME is cvora then
 * the following interface is implemented by support...
 *
 *  \par void *cvora_open(int lun);
 *  \par void  cvora_close(struct cvora_handle *handle);
 *  \par int   cvora_get_window(struct cvora_handle *handle, struct vmeio_get_window_s *win);
 *  \par int   cvora_raw(struct cvora_handle *handle, struct vmeio_riob_s *buf, int flag);
 *  \par int   cvora_dma(struct cvora_handle *handle, struct vmeio_riob_s *buf, int flag);
 *  \par int   cvora_wait(struct cvora_handle *handle, struct vmeio_read_buf_s *event);
 *  \par int   cvora_set_params(struct cvora_handle *handle, int winnum, int dmaflag);
 *  \par int   cvora_read_reg(struct cvora_handle *handle, int reg_num, int *reg_val);
 *  \par int   cvora_write_reg(struct cvora_handle *handle, int reg_num, int *reg_val);
 *  \par int   cvora_set_debug(struct cvora_handle *handle, int *level);
 *  \par int   cvora_get_debug(struct cvora_handle *handle, int *level);
 *  \par int   cvora_set_timeout(struct cvora_handle *handle, int *milliseconds);
 *  \par int   cvora_get_timeout(struct cvora_handle *handle, int *milliseconds);
 *  \par int   cvora_do_interrupt(struct cvora_handle *handle, int *mask);
 *  \par int   cvora_get_version(struct cvora_handle *handle, struct vmeio_version_s *ver);
 *  \par
 * These routines can also be called through the DRV_SYMB macro. In this
 * case the names systemattically become the capitalized string with the
 * leading cvora_ removed.
 * Ex cvora_set_timeout -> SET_TIMEOUT and cvora_write_reg -> WRITE_REG etc
 * Using the macro call syntax permits the same code to work with more than
 * one module.
 */

#include "cvora_hard.h"

/**
 * Hardware definitions for registers defined in cvora_hard.h
 * This file defines the CVORA register names and their offsets in the addressmap
 * Where a register is composed of bit fields the bit names are defined
 */

/**
 * ===================
 * @brief Initialize cvora user library
 * @param lun logical unit number
 * @return pointer to handle or null on error, see errno
 */

struct cvora_handle *cvora_init(int lun);

/**
 * ===================
 * @brief get module ID
 * @param handle returned from open
 * @param modid module ID
 * @return 1=OK 0=Error
 */

int cvora_get_module_id(struct cvora_handle *handle, int *modid);

/**
 * ===================
 * @brief get mode
 * @param handle returned from open
 * @param mode
 * @return 1=OK 0=Error
 */

int cvora_get_mode(struct cvora_handle *handle, int *mode);

/**
 * ===================
 * @brief set mode
 * @param handle returned from open
 * @param mode (See CVA_MODE)
 * @return 1=OK 0=Error
 */

int cvora_set_mode(struct cvora_handle *handle, int *mode);

/**
 * ===================
 * @brief read hardware status
 * @param handle returned from open
 * @param status (See CVA_STATUS)
 * @return 1=OK 0=Error
 */

int cvora_get_hardware_status(struct cvora_handle *handle, int *status);

/**
 * ===================
 * @brief wait for end of sample interrupt
 * @param handle returned from open
 * @return 1=OK 0=Error
 */

int cvora_wait2(struct cvora_handle *handle);

/**
 * ===================
 * @brief read memory buffer samples size
 * @param handle returned from open
 * @param memsz memory size in bytes
 * @return 1=OK 0=Error
 */

int cvora_get_sample_size(struct cvora_handle *handle, int *memsz);

/**
 * ===================
 * @brief Read memory sample buffer
 * @param handle returned from open
 * @param maxsz max size to read
 * @param actsz actual size read
 * @param buf pointer to data area
 * @return 1=OK 0=Error
 */

int cvora_read_samples(struct cvora_handle *handle, int maxsz, int *actsz, int *buf);

/**
 * ===================
 * @brief Issue a software start
 * @param handle returned from open
 * @return 1=OK 0=Error
 */

int cvora_soft_start(struct cvora_handle *handle);

/**
 * ===================
 * @brief Issue a software stop
 * @param handle returned from open
 * @return 1=OK 0=Error
 */

int cvora_soft_stop(struct cvora_handle *handle);

/**
 * ===================
 * @brief read DAC
 * @param handle returned from open
 * @param dacv dac value
 * @return 1=OK 0=Error
 */

int cvora_get_dac(struct cvora_handle *handle, int *dacv);

/**
 * ===================
 * @brief read clock frequency
 * @param handle returned from open
 * @param freq frequency value
 * @return 1=OK 0=Error
 */

int cvora_get_clock_frequency(struct cvora_handle *handle, int *freq);

/**
 * ===================
 * @brief set plot input
 * @param handle returned from open
 * @param plti plot input P2 channel routed to output DAC 1..32
 * @return 1=OK 0=Error
 */

int cvora_set_plot_input(struct cvora_handle *handle, int *plti);

/**
 * ===================
 * @brief read parralell channels mask
 * @param handle returned from open
 * @param chans value
 * @return 1=OK 0=Error
 */

int cvora_get_channels_mask(struct cvora_handle *handle, int *chans);

/**
 * ===================
 * @brief set parralell channels mask
 * @param handle returned from open
 * @param chans value
 * @return 1=OK 0=Error
 */

int cvora_set_channels_mask(struct cvora_handle *handle, int *chans);

#ifdef __cplusplus
}
#endif

#endif
