APP_NAME = reconfigApp

C++COMPILER =   c++ 
C_COMPILER =    c++
COMPILER =      c++ 

LD =            ld

MY_INC_DIR =    -I. -I$(VJ_BASE_DIR)/include
MYLIBS =   -L/usr/lib -L$(VJ_BASE_DIR)/lib -Wl,-Bstatic -Wl,--whole-archive -lJuggler -lJuggler_ogl -lvpr -ljccl -lgadget -Wl,-Bdynamic -Wl,--no-whole-archive


# These are assorted other libraries that must be linked with 
GLLIBS =    -lGLU -lGL 
XLIBS =     -lX11
OTHERLIBS = -lm -lpthread -pthread

LIBS =   $(GLLIBS) $(OTHERLIBS) $(MYLIBS)

INCLUDES = $(MY_INC_DIR)

CFLAGS =   -D__linux__ -D__i386__ -g

OBJS = reconfig.o reconfigApp.o

all: $(APP_NAME)

# Does linking of the files and libraries
$(APP_NAME):    $(OBJS)
		@echo "----------------------------------------  Linking...  ----"
		$(COMPILER) $(CFLAGS) $(OBJS) -o $(APP_NAME) $(LIBS)
		@echo ""
		@echo "\n   >>>  Make is Complete <<<\n"
		@echo ""

# Compiles the files if necessary
.cpp.o:           
	$(C++COMPILER) $(INCLUDES) $(CFLAGS) -o $@ -c $*.cpp

#clean up code
clean:
	rm -f *.o
	rm $(APP_NAME)
	@echo "Squeaky Clean"
