#smake

NAME = pinchGlove

# build defines
PROCESSOR = mips3
BINTYPE = n32
OPTIMIZATION = O2
COMPILER = CC
PARALLEL = -pca
NAMESPACES = namespaces=ON 
BOOL = bool=ON
FULLWARN = fullwarn
SMAKEMAX = -J 8
LINK_UNUSED_OBJECT_CODE = -all

PREPROCESS_OPTIONS = -I../../../Input/vjGlove $(SMAKEMAX) $(LINK_UNUSED_OBJECT_CODE)\
	 $(NAMESPACES) $(BOOL) -$(PROCESSOR) -$(BINTYPE) -$(OPTIMIZATION) 

LINK_OPTIONS =  $(SMAKEMAX) $(LINK_UNUSED_OBJECT_CODE) -$(PROCESSOR) -$(BINTYPE) -$(OPTIMIZATION)

OBJECTS = test.o 


all: $(OBJECTS)
	$(COMPILER) $(LINK_OPTIONS) ../../../Input/vjGlove/fsPinchGlove.cpp $(OBJECTS) -o $(NAME)
	-@echo "Done making FakeSpace pinch glove test driver..."

clean: 
	-rm -r ii_files
	-rm *.o
	-rm $(NAME)
	
# Compiles the files if necessary
.c.o:
	$(COMPILER) $(PREPROCESS_OPTIONS) $(INCLUDEDIRS) -c $@ $*.c 

.cxx.o:
	$(COMPILER) $(PREPROCESS_OPTIONS) $(INCLUDEDIRS) -c $@ $*.cxx 

.c++.o:
	$(COMPILER) $(PREPROCESS_OPTIONS) $(INCLUDEDIRS) -c $@ $*.c++

.C.o:
	$(COMPILER) $(PREPROCESS_OPTIONS) $(INCLUDEDIRS) -c $@ $*.C

.cpp.o:
	$(COMPILER) $(PREPROCESS_OPTIONS) $(INCLUDEDIRS) -c $@ $*.cpp 
