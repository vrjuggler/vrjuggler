SRCS = soundtest.cxx
PROJECT_NAME = soundtest
OBJDIR = object_code

include ../Makefile.base
include ../Makefile.rec
include ../Makefile.dep

# Do any pre init stuff that has to happen before build rules 
#  are processed
# Check that everything is defined before making.
prebuild:
	@if ! [ -d ${OBJDIR} ]; then									 \
		echo "";															  \
		echo "-------------------------------------------" ;	\
		echo "Creating directory \"${OBJDIR}\" ..." ;			\
		echo "-------------------------------------------" ;	\
		mkdir ${OBJDIR} ;												 \
	fi
	-@echo ""
	-@echo "-------------------------------------------"
	-@echo "Creating the library"
	-@echo "-------------------------------------------"

# Compile then Link
build: $(OBJS)
	-@echo ""
	-@echo "-------------------------------------------"
	-@echo "Linking... (.$(OBJ_FILE_SUFFIX),.lib --> Executable)"
	-@echo "-------------------------------------------"
	$(EXE_LINKER) $(EXE_FLAGS) $(OBJS) $(OUTPUT_FILE) $(LIB_PATHS) $(LIBS) -L../src -lsoundengine -L../sl -lsl
	-@echo "-------------------------------------------"
	-@echo "Done... (type $(FILENAME) to run)"
	-@echo "-------------------------------------------"
