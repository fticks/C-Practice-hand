#
# Makefile for Book routines figlinks.
#
ROOT=../..
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

CFLAGS=-std=c99 -g -I$(ROOT)/include -Wall -Wno-unused -DLINUX -D_GNU_SOURCE $(EXTRA)

EXES = fig11.2.c fig11.3.c fig11.4.c fig11.5.c \
	fig11.10.c fig11.11.c fig11.12.c fig11.13.c \
	fig11.14.c fig11.15.c fig11.16.c \
	ex11.1.c ex11.2.c \

SRCS = $(wildcard $(EXES).c)
OBJS = $(patsubst %.c,%,$(SRCS))

all:$(OBJS)

%:%.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS) -lpthread -lrt -lbsd

clean:
	rm -f *.o a.out core temp.* $(OBJS)

include $(ROOT)/Make.libapue.inc
