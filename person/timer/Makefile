include ../Makefile.param

ROOT := ..
PROGS := timer01 timer02 timer03 timer04
MODPROGS := 

CFLAGS += -I$(ROOT)/$(INC_DIR)
LIBDIR += $(ROOT)/$(LIB_DIR)
LDLIBS += $(LIBDIR)/$(UNP_LIBA) -lpthread -lrt -lm -lz
#LDLIBS += -L$(LIBDIR) $(LIBDIR)/$(UNP_LIBA) -lpthread -lrt -lm -lz
#we know, static lib not need -L

.PTONY := all clean $(PROGS)

all : $(PROGS) $(MODPROGS)

timer01 : timer01.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

timer02 : timer02.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

timer03 : timer03.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)	

timer04 : timer04.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)	
	
clean:
	rm -rf $(PROGS) $(MODPROGS) $(CLEANFILES)
