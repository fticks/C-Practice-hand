include ../Makefile.param

ROOT := ..
PROGS := traceroute
MODPROGS := main.o sig_alrm.o traceloop.o tv_sub.o send_v4.o recv_v4.o icmpcode_v4.o in_checknum.o

CFLAGS += -I$(ROOT)/$(INC_DIR)
LIBDIR += $(ROOT)/$(LIB_DIR)
LDLIBS += -L$(LIBDIR) $(LIBDIR)/$(UNP_LIBA) -lpthread -lrt

.PTONY := all clean $(PROGS)

all : $(PROGS) $(MODPROGS)

traceroute : $(MODPROGS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -rf $(PROGS) $(MODPROGS) $(CLEANFILES)
