#
# Makefile for Book routines figlinks.
#
ROOT=../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

CFLAGS=-std=c99 -g -I$(ROOT)/include -Wall -Wno-unused -DLINUX -D_GNU_SOURCE $(EXTRA)

EXES = fig5.11.c fig5.15.c 

SRCS = $(wildcard $(EXES).c)
OBJS = $(patsubst %.c,%,$(SRCS))

all:$(OBJS)

%:%.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS) -lpthread -lrt -lbsd

clean:
	rm -f *.o a.out core temp.* $(OBJS)

include $(ROOT)/Make.libapue.inc
