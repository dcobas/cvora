#================================================================
# Makefile to produce acdxing library
#================================================================

CPU=L865

include /ps/dsc/src/co/Make.auto

CFLAGS= -g -Wall -I. -I/user/pnikiel/CVORA/cvora/include

SRCS=cvora_user.c cvora_user.h cvora_hard.h

all: cvora_user.$(CPU).a

cvora_user.$(CPU).o: $(SRCS)

cvora_user.$(CPU).a: cvora_user.$(CPU).o
	-$(RM) $@
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean:
	rm -f cvora_user.$(CPU).o
	rm -f cvora_user.$(CPU).a
