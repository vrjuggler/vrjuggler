#!gmake

CXX_COMPILE = CC
OBJS = TgaImporter.o SgiImporter.o FileHandler.o main.o ../Image.o
INCLUDE_PATHS = -I..
PATH_TO_SRCS = ..:.
PROGRAM_NAME = main

vpath
vpath %.cxx ${PATH_TO_SRCS}

all: $(OBJS)
	$(CXX_COMPILE) -o $(PROGRAM_NAME) $(OBJS) -lm
   
%.o: %.cpp
	$(CXX_COMPILE) $(INCLUDE_PATHS) $< -c -o $@

%.o: %.cxx
	$(CXX_COMPILE) $(INCLUDE_PATHS) $< -c -o $@



clean:
	rm -rf $(PROGRAM_NAME) $(OBJS) ii_files
