
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "vmeio.h"

#include "cvora_hard.h"
#include "libcvora.h"

cvora_t *cvora_init(int lun)
{
	cvora_t *h;
	char fname[256];
	int fnum;

	sprintf(fname, "/dev/cvora.%d", lun);
	if ((fnum = open(fname, O_RDWR, 0)) < 0) {
		fprintf(stderr, "Error:cvora_open:"
			"Can't open:%s for read/write\n", fname);
		return NULL;
	}

	h = malloc(sizeof(*h));
	if (h == NULL) {
		fprintf(stderr, "Error:cvora_open Can't allocate memory\n");
		close(fnum);
		return NULL;
	}
	memset(h, 0, sizeof(*h));
	h->file = fnum;

#if 0
	/* this sets positive polarity, enables the module, sets the int
	 * vector and enables interrupts. To be done in the driver.
	 */
	ch->cmd = CVA_CMD_POLARITY & CVA_POLARITY_POS;	/* Positive polarity */
	ch->cmd |= CVA_CMD_ENABLE_MODULE & CVA_ENABLE_MODULE;	/* Enable module */
	ch->cmd |= ch->win.vec << CVA_VECTOR_SHIFT;	/* Interrupt vector */
	ch->cmd |= CVA_CMD_ENABLE_INTERRUPT & CVA_ENABLE_INTERRUPT;	/* Enable interrupts */
	cc = cvora_write_reg(h, CVA_REG_WO_CONTROL, cmd);
	if (!cc)
		goto init_err;

	ch->mode = CVA_MDE_PARALLEL;
	cc = cvora_write_reg(sh, CVA_REG_WO_MODE, &ch->mode);
	if (!cc)
		goto init_err;

	cc = cvora_read_reg(sh, CVA_REG_RO_MODULE_ID, &ch->modid);
	if (!cc)
		goto init_err;
	return (void *) ch;
#endif

	return h;
}

static int read_reg(cvora_t *h, unsigned offset, unsigned *value)
{
	struct vmeio_riob_s cb;

	cb.winum = 1;
	cb.offset = offset;
	cb.bsize = sizeof(unsigned);
	cb.buffer = value;

	return ioctl(h->file, VMEIO_RAW_READ, &cb);
}


static int write_reg(cvora_t *h, unsigned offset, unsigned value)
{
	struct vmeio_riob_s cb;

	cb.winum = 1;
	cb.offset = offset;
	cb.bsize = sizeof(unsigned);
	cb.buffer = &value;

	return ioctl(h->file, VMEIO_RAW_WRITE, &cb);
}

static int set_reg_bit(cvora_t *h, unsigned offset, unsigned bit,
		      int value)
{
	unsigned int contents;
	int cc;

	if ((cc = read_reg(h, offset, &contents)) != 0)
		return cc;
	contents &= ~(1 << bit);
	contents |= (value & 1) << bit;
	return write_reg(h, offset, contents);
}

static int get_reg_bit(cvora_t *h, unsigned offset, unsigned bit,
		      int *value)
{
	unsigned int contents;
	int cc;

	if ((cc = read_reg(h, offset, &contents)) != 0)
		return cc;
	contents &= ~(1 << bit);
	contents >>= bit;
	*value = contents;
	return 0;
}

int cvora_get_version(cvora_t *h, int *version)
{
	unsigned int ver;
	int cc;

	if ((cc = read_reg(h, CVORA_CONTROL, &ver)) != 0)
		return cc;
	ver >>= CVORA_VERSION_BIT;
	*version = ver;
	return 0;
}

int cvora_set_pulse_polarity(cvora_t *h, int polarity)
{
	return set_reg_bit(h, CVORA_CONTROL, CVORA_POLARITY_BIT, polarity);
}

int cvora_get_pulse_polarity(cvora_t *h, int *polarity)
{
	return get_reg_bit(h, CVORA_CONTROL, CVORA_POLARITY_BIT, polarity);
}

int cvora_set_module_enabled(cvora_t *h, int enabled)
{
	return set_reg_bit(h, CVORA_CONTROL, CVORA_MODULE_ENABLE_BIT, enabled);
}

int cvora_get_module_enabled(cvora_t *h, int *enabled)
{
	return get_reg_bit(h, CVORA_CONTROL, CVORA_MODULE_ENABLE_BIT, enabled);
}

int cvora_set_irq_enabled(cvora_t *h, int enabled)
{
	return set_reg_bit(h, CVORA_CONTROL, CVORA_INT_ENABLE_BIT, enabled);
}

int cvora_get_irq_enabled(cvora_t *h, int *enabled)
{
	return get_reg_bit(h, CVORA_CONTROL, CVORA_INT_ENABLE_BIT, enabled);
}

int cvora_set_irq_vector(cvora_t *h, int vector)
{
	unsigned status;
	int cc;

	if ((cc = read_reg(h, CVORA_CONTROL, &status)) != 0)
		return cc;
	status &= ~CVORA_VECTOR_MASK;
	status |= vector << CVORA_VECTOR_BIT;
	return 0;
}

int cvora_get_irq_vector(cvora_t *h, int *vector)
{
	unsigned status;
	int cc;

	if ((cc = read_reg(h, CVORA_CONTROL, &status)) != 0)
		return cc;
	*vector = (status & CVORA_VECTOR_MASK) >> CVORA_VECTOR_BIT;
	return 0;
}

int cvora_get_mode(cvora_t *h, enum cvora_mode *mode)
{
	unsigned modereg;
	int cc;

	if ((cc = read_reg(h, CVORA_MODE, &modereg)) != 0)
		return cc;
	*mode = modereg & CVORA_MODE_MASK;
	return 0;
}

int cvora_set_mode(cvora_t *h, enum cvora_mode mode)
{
	if (mode & CVORA_MODE_MASK)
		return -EINVAL;
	return write_reg(h, CVORA_MODE, mode);
}

int cvora_get_hardware_status(cvora_t *h, unsigned int *status)
{
	return read_reg(h, CVA_REG_RO_STATUS, status);
}

int cvora_wait(cvora_t *h)
{
	struct vmeio_read_buf_s event;

	return read(h->file, &event, sizeof(event));
}

int cvora_get_sample_size(cvora_t *h, int *memsz)
{
	int cc;
	unsigned memp;		/* Memory pointer */

	if ((cc = read_reg(h, CVORA_MEMORY_POINTER, &memp)) != 0)
		return cc;
	if (memp < CVORA_MEM_MIN || memp > CVORA_MEM_MAX)
		return -EINVAL;
	*memsz = memp - CVA_MEM_MIN;
	return 0;
}

int cvora_read_samples(cvora_t *h, int maxsz, int *actsz, unsigned int *buf)
{
	int cc;
	struct vmeio_riob_s riob;

	if ((cc = cvora_get_sample_size(h, actsz)) != 0)
		return cc;

	if (*actsz > maxsz)
		*actsz = maxsz;

	riob.winum = 1;
	riob.offset = CVORA_MEMORY;
	riob.bsize = *actsz;
	riob.buffer = buf;

	return ioctl(h->file, VMEIO_RAW_WRITE_DMA, &riob);
}

int cvora_soft_start(cvora_t *h)
{
	return set_reg_bit(h, CVORA_CONTROL, CVORA_SOFT_START_BIT, 1);
}


int cvora_soft_stop(cvora_t *h)
{
	return set_reg_bit(h, CVORA_CONTROL, CVORA_SOFT_STOP_BIT, 1);
}

int cvora_get_dac(cvora_t *h, unsigned int *dacv)
{
	return read_reg(h, CVORA_DAC, dacv);
}

int cvora_get_clock_frequency(cvora_t *h, unsigned int *freq)
{
	return read_reg(h, CVORA_FREQUENCY, freq);
}

int cvora_set_plot_input(cvora_t *h, unsigned int plti)
{
	return write_reg(h, CVORA_FREQUENCY, plti);
}

int cvora_get_channels_mask(cvora_t *h, unsigned int *chans)
{
	return read_reg(h, CVORA_CHANNEL, chans);
}

int cvora_set_channels_mask(cvora_t *h, unsigned int chans)
{
	return write_reg(h, CVORA_CHANNEL, chans);
}
