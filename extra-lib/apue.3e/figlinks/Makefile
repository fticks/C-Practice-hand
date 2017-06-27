#
# Makefile for Book routines figlinks.
#
ROOT=..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

EXES = fig11.2.c 

SRCS = $(wildcard $(EXES).c)
OBJS = $(patsubst %.c,%,$(SRCS))

all:$(OBJS)

%:%.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS) -lpthread

clean:
	rm -f *.o a.out core temp.* $(OBJS)

include $(ROOT)/Make.libapue.inc
