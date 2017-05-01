
include ../Makefile.param

ROOT := ..
PROGS := server01 client01 client02 server02 client03 server03
MODPROGS := server01.o client01.o client02.o server02.o client03.o server03.o pthreadpool.o

CFLAGS += -I$(ROOT)/$(INC_DIR) -I$(ROOT)/chatroom
LIBDIR += $(ROOT)/$(LIB_DIR)
LDLIBS += -L$(LIBDIR) $(LIBDIR)/$(UNP_LIBA) -lpthread -lrt

.PTONY := all clean $(PROGS)

all : $(PROGS) $(MODPROGS)

#udp iteration-server client-list
#ok, after debug, it proved wronging
server01 : server01.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
client01 : client01.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

#udp client-connect
client02 : client02.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
server02 : server02.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

#udp server-connect
client03 : client03.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
server03 : server03.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -rf $(PROGS) $(MODPROGS) $(CLEANFILES)
