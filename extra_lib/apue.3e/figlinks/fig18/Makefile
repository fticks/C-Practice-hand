#
# Makefile for Book routines figlinks.
#
ROOT=../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

CFLAGS=-std=c99 -g -I$(ROOT)/include -Wall -Wno-unused -DLINUX -D_GNU_SOURCE $(EXTRA)

EXES = fig18.10.c fig18.11.c fig18.14.c fig18.16.c \
	fig18.18.c fig18.21.c fig18.22.c \
	lserial.c test_uart1.c 

PROGS = test_uart2
MOREPROGS = serial

SRCS = $(wildcard $(EXES).c)
OBJS = $(patsubst %.c,%,$(SRCS))

all:$(OBJS) $(PROGS)

%:%.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS) -lpthread -lrt -lbsd

test_uart2:test_uart2.o serial.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS) -lpthread -lrt -lbsd

clean:
	rm -f *.o a.out core temp.* $(OBJS) $(PROGS)

include $(ROOT)/Make.libapue.inc
