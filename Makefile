# CPU=L865
# KVER=2.6.24.7-rt21
# KVER=2.6.24.7-rt27
# KVER=2.6.29.4-rt15
# KVER=2.6.31.6-rt19
# KVER=2.6.24.7-rt27

CPU=L865
KVER=2.6.24.7-rt27
KERNELSRC=/acc/sys/$(CPU)/usr/src/kernels/$(KVER)

include /acc/dsc/src/co/Make.auto

all: modules libs test

modules: 
	cp Module.symvers.vmebus Module.symvers
	make -C $(KERNELSRC) M=`pwd` KVER=$(KVER) modules
clean:
	rm -f *.so
	make -C $(KERNELSRC) M=`pwd` KVER=$(KVER) clean
	make -C doc clean
docs:
	make -C doc

COMPILE_TIME:=$(shell date +%s)

CFLAGS= -DCOMPILE_TIME=$(COMPILE_TIME) -g -Wall -fPIC

libs: libcvora.$(CPU).a libcvora.$(CPU).so

libcvora.$(CPU).o: libcvora.c libcvora.h
libcvora.$(CPU).so: libcvora.$(CPU).o
	$(CC) $(CFLAGS) -shared -o $@ $^
libcvora.$(CPU).a: libcvora.$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

