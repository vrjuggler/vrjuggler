SHCC= ${CC} -fPIC
MINOR= 1
MAJOR= 0
LIB_NAME= libpfstat
SHLIB= ${LIB_NAME}.so.${MAJOR}.${MINOR}
TARGETS= ${SHLIB}

OBJS= pfstat.o

all: ${TARGETS}

${TARGETS}: ${OBJS}
	${SHCC} ${CFLAGS} -nostdlib -shared -o ${SHLIB} -Wl,--soname -Wl,${LIB_NAME}.so.${MAJOR} ${OBJS}
	ln -sf ${SHLIB} ${LIB_NAME}.so
	@echo ""
	@echo ""
	@echo "Make sure you do this: "
	@echo "           export LD_PRELOAD=/path/to/libpfstat.so"


clean:
	-rm -f *.so *.so.* *.o *~ ${TARGETS}
