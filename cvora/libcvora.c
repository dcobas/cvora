/**
 * =============================================================================
 * Implement user library for cvora
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Predefined cvora support library routines on which this library
 * can make calls. See vmeio_support.h if DRV_NAME is cvora then
 * the following interface is implemented by support...
 *
 * void *cvora_open(int lun);
 * void  cvora_close(void *handle);
 * int   cvora_get_window(void *handle, struct vmeio_get_window_s *win);
 * int   cvora_raw(void *handle, struct vmeio_riob_s *buf, int flag);
 * int   cvora_dma(void *handle, struct vmeio_riob_s *buf, int flag);
 * int   cvora_wait(void *handle, struct vmeio_read_buf_s *event);
 * int   cvora_set_params(void *handle, int winnum, int dmaflag);
 * int   cvora_read_reg(void *handle, int reg_num, int *reg_val);
 * int   cvora_write_reg(void *handle, int reg_num, int *reg_val);
 */

#include "libcvora.h"

/**
 * Private data structure for library handle
 */

struct cvora_handle {
   void *hand;                    /* Support library handle */
   struct vmeio_get_window_s win; /* Address windows descriptor */
   int cmd;                       /* Command register */
   int mode;                      /* Mode register */
   int modid;                     /* Module ID */
 };

/**
 * ===============================================================================
 * These next routines implement the user library extra functions
 */

/**
 * ===================
 * @brief Initialize cvora user library
 * @param lun logical unit number
 * @return pointer to handle or null on error, see errno
 */

struct cvora_handle *cvora_init(int lun) {

void *sh = NULL;           /* Support library handle */
struct cvora_handle *ch; /* Cvora library handle */
struct vmeio_get_window_s win;
int cc;

   sh = cvora_open(lun);
   if (!sh) return NULL;

   ch = malloc(sizeof(struct cvora_handle));
   if (!ch) return NULL;
   ch->hand = sh;

   cc = cvora_get_window(sh,&win);
   if(!cc) goto init_err;
   memmove(&ch->win, &win, sizeof(struct vmeio_get_window_s));

   ch->cmd  = CVA_CMD_POLARITY & CVA_POLARITY_POS;             /* Positive polarity */
   ch->cmd |= CVA_CMD_ENABLE_MODULE & CVA_ENABLE_MODULE;       /* Enable module */
   ch->cmd |= ch->win.vec << CVA_VECTOR_SHIFT;                 /* Interrupt vector */
   ch->cmd |= CVA_CMD_ENABLE_INTERRUPT & CVA_ENABLE_INTERRUPT; /* Enable interrupts */
   cc = cvora_write_reg(sh,CVA_REG_WO_CONTROL,&ch->cmd);
   if(!cc) goto init_err;

   ch->mode = CVA_MDE_PARALLEL;
   cc = cvora_write_reg(sh,CVA_REG_WO_MODE,&ch->mode);
   if(!cc) goto init_err;

   cc = cvora_read_reg(sh,CVA_REG_RO_MODULE_ID,&ch->modid);
   if(!cc) goto init_err;
   return (void *) ch;

init_err:
   cvora_close(ch->hand);
   free(ch);
   return NULL;
}

/**
 * ===================
 * @brief get module ID
 * @param handle returned from open
 * @param modid module ID
 * @return 1=OK 0=Error
 */

int cvora_get_module_id(void *handle, int *modid) {

struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {
      *modid = ch->modid;
      return 1;
   }
   return 0;
}

/**
 * ===================
 * @brief get mode
 * @param handle returned from open
 * @param mode
 * @return 1=OK 0=Error
 */

int cvora_get_mode(void *handle, int *mode) {

struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {
      *mode = ch->mode;
      return 1;
   }
   return 0;
}

/**
 * ===================
 * @brief set mode
 * @param handle returned from open
 * @param mode (See CVA_MODE)
 * @return 1=OK 0=Error
 */

int cvora_set_mode(void *handle, int *mode) {

int cc;
struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if ((ch) && (*mode>=CVA_MDES_FIRST) && (*mode <=CVA_MDES_LAST)) {
      cc = cvora_write_reg(ch->hand,CVA_REG_WO_MODE,mode);
      if (!cc) return 0;
      ch->mode = *mode;
      return 1;
   }
   return 0;
}

/**
 * ===================
 * @brief read hardware status
 * @param handle returned from open
 * @param status (See CVA_STATUS)
 * @return 1=OK 0=Error
 */

int cvora_get_hardware_status(void *handle, int *status) {

struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {
      return cvora_read_reg(ch->hand,CVA_REG_RO_STATUS,status);
   }
   return 0;
}

/**
 * ===================
 * @brief wait for end of sample interrupt
 * @param handle returned from open
 * @return 1=OK 0=Error
 */

int cvora_wait2(void *handle) {

int cc;
struct vmeio_read_buf_s event;
struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {
      cc = cvora_wait(ch->hand,&event);
      if (cc <= 0) return 0;
      return 1;
   }
   return 0;
}

/**
 * ===================
 * @brief read memory buffer samples size
 * @param handle returned from open
 * @param memsz memory size in bytes
 * @return 1=OK 0=Error
 */

int cvora_get_sample_size(void *handle, int *memsz) {

int cc, memp;              /* Memory pointer */
struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {
      cc = cvora_read_reg(ch->hand,CVA_REG_RW_MEM_POINTER,&memp);
      if ((cc) && (memp >= CVA_MEM_MIN) && (memp <= CVA_MEM_MAX)) {
	 *memsz = memp - CVA_MEM_MIN;
	 return 1;
      }
   }
   return 0;
}

/**
 * ===================
 * @brief Read memory sample buffer
 * @param handle returned from open
 * @param maxsz max size to read
 * @param actsz actual size read
 * @param buf pointer to data area
 * @return 1=OK 0=Error
 */

int cvora_read_samples(void *handle, int maxsz, int *actsz, int *buf) {

struct cvora_handle *ch; /* Cvora library handle */
struct vmeio_riob_s riob;

   ch = (struct cvora_handle *) handle;
   if (ch) {
      if (!cvora_get_sample_size(handle,actsz)) return 0;

      if (*actsz > maxsz) *actsz = maxsz;

      riob.winum  = 1;
      riob.offset = CVA_OFFSET_RW_MEMORY;
      riob.bsize  = *actsz;
      riob.buffer = buf;

      return cvora_dma(ch->hand,&riob,0);
   }
   return 0;
}

/**
 * ===================
 * @brief Write memory sample buffer
 * @param handle returned from open
 * @param maxsz max size to read
 * @param buf pointer to data area
 * @return 1=OK 0=Error
 */

int cvora_write_samples(void *handle, int maxsz, int *buf) {

struct cvora_handle *ch; /* Cvora library handle */
struct vmeio_riob_s riob;

   ch = (struct cvora_handle *) handle;
   if (ch) {

      riob.winum  = 1;
      riob.offset = CVA_OFFSET_RW_MEMORY;
      riob.bsize  = maxsz;
      riob.buffer = buf;

      return cvora_dma(ch->hand,&riob,1);
   }
   return 0;
}

/**
 * ===================
 * @brief Issue a software start
 * @param handle returned from open
 * @return 1=OK 0=Error
 */

int cvora_soft_start(void *handle) {

int cmd;
struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {

      cmd = ch->cmd | CVA_CMD_SOFT_START;
      return cvora_write_reg(ch->hand,CVA_REG_WO_CONTROL,&cmd);
   }
   return 0;
}

/**
 * ===================
 * @brief Issue a software stop
 * @param handle returned from open
 * @return 1=OK 0=Error
 */

int cvora_soft_stop(void *handle) {

int cmd;
struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {

      cmd = ch->cmd | CVA_CMD_SOFT_STOP;
      return cvora_write_reg(ch->hand,CVA_REG_WO_CONTROL,&cmd);
   }
   return 0;
}

/**
 * ===================
 * @brief read DAC
 * @param handle returned from open
 * @param dacv dac value
 * @return 1=OK 0=Error
 */

int cvora_get_dac(void *handle, int *dacv) {

struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {
      return cvora_read_reg(ch->hand,CVA_REG_RO_DAC,dacv);
   }
   return 0;
}

/**
 * ===================
 * @brief read clock frequency
 * @param handle returned from open
 * @param freq frequency value
 * @return 1=OK 0=Error
 */

int cvora_get_clock_frequency(void *handle, int *freq) {

struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {
      return cvora_read_reg(ch->hand,CVA_REG_RO_FREQ,freq);
   }
   return 0;
}

/**
 * ===================
 * @brief set plot input
 * @param handle returned from open
 * @param plti plot input P2 channel routed to output DAC 1..32
 * @return 1=OK 0=Error
 */

int cvora_set_plot_input(void *handle, int *plti) {

struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {
      return cvora_write_reg(ch->hand,CVA_REG_WO_PLOT,plti);
   }
   return 0;
}

/**
 * ===================
 * @brief read parralell channels mask
 * @param handle returned from open
 * @param chans value
 * @return 1=OK 0=Error
 */

int cvora_get_channels_mask(void *handle, int *chans) {

struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {
      return cvora_read_reg(ch->hand,CVA_REG_RO_CHANNELS,chans);
   }
   return 0;
}

/**
 * ===================
 * @brief set parralell channels mask
 * @param handle returned from open
 * @param chans value
 * @return 1=OK 0=Error
 */

int cvora_set_channels_mask(void *handle, int *chans) {

struct cvora_handle *ch; /* Cvora library handle */

   ch = (struct cvora_handle *) handle;
   if (ch) {
      return cvora_write_reg(ch->hand,CVA_REG_RO_CHANNELS,chans);
   }
   return 0;
}
