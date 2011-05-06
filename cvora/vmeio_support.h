/**
 * =============================================================================
 * Implement library for driver to do basic VMEIO
 * This is the support layer on which hardware specific routines are implemented
 * Definitions file
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 *
 */

#ifndef VMEIOSUPPORT
#define VMEIOSUPPORT

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * This file defines the driver instance DRV_NAME and DRV_SYMB macros
 */
#include "vmeio.h"

struct vmeio_handle {
   int                       file;      /** File number */
   int                       winum;     /** Window 1..2 */
   int                       dmaflag;   /** Use DMA flag 0..1 */
   int                       dmaswap;   /** Swap after DMA flag 0..1 */
   int                       offset;    /** Block offset added to all addresses */
   struct vmeio_get_window_s window;
 };

/*
 * ============================================
 * Basic routines calling driver
 */

/**
 * @brief open a handle for a given lun
 * @param lun logical unit number
 * @return handle pointer or null if error
 */

struct vmeio_handle *cvora_open(int lun);

/**
 * @brief open a handle for a given lun
 * @param lun logical unit number
 * @param name of the node in /dev (used in test programs)
 * @return handle pointer or null if error
 */

struct vmeio_handle *cvora_open_name(int lun, char *name);

/**
 * @brief close a handle
 * @param handle returned from open
 */

void cvora_close(struct vmeio_handle *h);

/**
 * ============================================
 * @brief Get driver and library UTC compilation dates
 * @param handle returned from open
 * @param ver versions of library and driver
 * @return 1 = OK 0 = FAIL
 */

int cvora_get_version(struct vmeio_handle *h, struct vmeio_version_s *ver);

/**
 * ============================================
 * @brief Set driver timeout in milliseconds
 * @param handle returned from open
 * @param timeout in milliseconds
 * @return 1 = OK 0 = FAIL
 */

int cvora_set_timeout(struct vmeio_handle *h, int *timeout);

/**
 * @brief Set driver debug level
 * @param handle returned from open
 * @param level of debug
 * @return 1 = OK 0 = FAIL
 */

int cvora_set_debug(struct vmeio_handle *h, int *level);

/**
 * @brief Get driver timeout in milliseconds
 * @param handle returned from open
 * @param timeout in milliseconds
 * @return 1 = OK 0 = FAIL
 */

int cvora_get_timeout(struct vmeio_handle *h, int *timeout);

/**
 * @brief Get driver debug level
 * @param handle returned from open
 * @param level of debug
 * @return 1 = OK 0 = FAIL
 */

int cvora_get_debug(struct vmeio_handle *h, int *level);

/**
 * @brief make an interrupt now
 * @param handle returned from open
 * @param mask of interrupt to make
 * @return 1 = OK 0 = FAIL
 */

int cvora_do_interrupt(struct vmeio_handle *h, int *mask);

/**
 * ============================================
 * Get a window descriptor for a given lun
 * @param handle returned from open
 * @param win pointer to your descriptor
 * @return 1 = OK 0 = FAIL
 */

int cvora_get_window(struct vmeio_handle *h, struct vmeio_get_window_s *win);

/**
 * ============================================
 * @brief do raw IO to a memory mapped device
 * @param handle returned from open
 * @param buf is a structure containing a pointer to your data area
 * @param flag 0=read 1=write
 * @return 1 = OK 0 = FAIL
 */

int cvora_raw(struct vmeio_handle *h, struct vmeio_riob_s *buf, int flag);

/**
 * @brief Transfer data via cvora_dma, WARNING byte swapping is your problem
 * @param handle returned from open
 * @param buf is a structure containing a pointer to your data area
 * @param flag 0=read 1=write
 * @return 1 = OK 0 = FAIL
 */

int cvora_dma(struct vmeio_handle *h, struct vmeio_riob_s *buf, int flag);

/**
 * ============================================
 * @brief wait for an interrupt event
 * @param handle returned from open
 * @param event will contain mask 0 if timeout occured
 * @return 1 = OK 0 = FAIL
 */

int cvora_wait(struct vmeio_handle *h, struct vmeio_read_buf_s *event);

/*
 * ============================================
 * Convieniance routines calling the basic ones
 */

/**
 * @brief Set default parameter for READ/WRITE REG calls
 * @param handle returned from open
 * @param winnum window number 1..2
 * @param dmaflag 0 use map 1 use cvora_dma
 * @param dmaswap 0 not swap 1 swap
 * @return 1 = OK 0 = FAIL
 */

int cvora_set_params(struct vmeio_handle *h, int winnum, int dmaflag, int dmaswap);

/**
 * @brief read a register
 * @param handle returned from open
 * @param reg_num register number (not byte offset)
 * @param reg_val value read
 * @return 1 = OK 0 = FAIL
 */

int cvora_read_reg(struct vmeio_handle *h, int reg_num, int *reg_val);

/**
 * @brief write a register
 * @param handle returned from open
 * @param reg_num register number (not byte offset)
 * @param reg_val value read
 * @return 1 = OK 0 = FAIL
 */

int cvora_write_reg(struct vmeio_handle *h, int reg_num, int *reg_val);

/**
 * ============================================
 * @brief Set global block offset
 * @param handle returned from open
 * @param offset to be applied
 * @return 1 = OK 0 = FAIL
 */

int cvora_set_offset(struct vmeio_handle *h, int *offset);

/**
 * @brief Get global block offset
 * @param handle returned from open
 * @param offset will contain current offset
 * @return 1 = OK 0 = FAIL
 */

int cvora_get_offset(struct vmeio_handle *h, int *offset);

#ifdef __cplusplus
}
#endif

#endif
