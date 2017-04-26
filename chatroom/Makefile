
include ../Makefile.param

ROOT := ..
PROGS := server01 client01 server02
MODPROGS := server01.o client01.o server02.o pthreadpool.o

CFLAGS += -I$(ROOT)/$(INC_DIR)
LIBDIR += $(ROOT)/$(LIB_DIR)
LDLIBS += -L$(LIBDIR) $(LIBDIR)/$(UNP_LIBA) -lpthread -lrt

.PTONY := all clean $(PROGS)

all : $(PROGS) $(MODPROGS)

server01 : server01.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

client01 : client01.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

server02 : server02.o pthreadpool.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -rf $(PROGS) $(MODPROGS) $(CLEANFILES)
