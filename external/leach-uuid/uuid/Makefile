CC		= cc
CFLAGS		= -g $(DEFS) $(INCLUDES)
DEFS		= # -DWININC
INCLUDES	= -I../../libmd5-rfc

OBJS		= $(UUID_OBJS) $(MD5_OBJS)
UUID_OBJS	= utest.o uuid.o sysdep.o
MD5_OBJS	= md5.o

VPATH		= ../../libmd5-rfc

utest: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS)

clobber:
	$(MAKE) clean
	rm -f utest
