# objects to be included in the project....
CODE_TO_TEST = Multicaster.cpp
TEST_SUITES = ExampleTestCase.cpp MulticasterTest.cpp 
CPPUNIT_TEXTUI_SRCS = TestRunner.cpp TextTestResult.cpp \
CPPUNIT_SRCS = TestCase.cpp TestResult.cpp TestFailure.cpp TestSuite.cpp
TESTDRIVER_SRCS = driver.cxx

SRCS = $(CODE_TO_TEST) \
		 $(TEST_SUITES) \
		 $(CPPUNIT_SRCS) \
       $(CPPUNIT_TEXTUI_SRCS) \
		 $(TESTDRIVER_SRCS)

# project name...
PROJECT_NAME = driver

OBJDIR = .
DEPDIR = 

PATH_TO_SRCS = .:../framework:../textui:Multicaster
MK_DIR = ../../mk

########################################################################
# system dependent stuff, that users will want to change...
# stuff that cant be abstracted in the build system...
##########################################################################
include ${MK_DIR}/mk.hosttype
# IRIX
ifeq ($(HOSTTYPE), IRIX)
   INCLUDE_PATHS =  -I. -I../framework -I../textui -IMulticaster
   LIBS = -lm
   DEPENDFLAGS = $(INCLUDE_PATHS)
   include ${MK_DIR}/mk.irix
endif

#LiNUX
ifeq ($(HOSTTYPE), Linux)
endif

# Win32
ifeq ($(HOSTTYPE), Win32)
   INCLUDE_PATHS = -I. -I../test/framework
   LIBS = 
   DEPENDFLAGS = -D__cplusplus -D_WIN32 -DWIN32 $(INCLUDE_PATHS)
   include ${MK_DIR}/mk.win32
endif

# gnu2
ifeq ($(HOSTTYPE), GNU)
   INCLUDE_PATHS = -I. -I../test/framework
   LIBS = -lm
   include ${MK_DIR}/mk.gnu
endif
########################################################################

include ${MK_DIR}/mk.objs

# Compile then Link
build: $(OBJS)
	-@echo ""
	-@echo ".------------------------------------------"
	-@echo "Linking... (.$(OBJ_FILE_SUFFIX)'s --> .so)"
	-@echo "-------------------------------------------"
	$(EXE_LINKER) $(EXE_FLAGS) $(OBJS) $(OUTPUT_EXE_FLAG) $(LIBS)
	-@echo ""
	-@echo "-------------------------------------------"
	-@echo "$(FILENAME) done"
	-@echo "-------------------------------------------"

# Remove the compiled stuff from the system
clean:
	-@echo ""
	-@echo ".------------------------------------------"
	-@echo "Removing compiled stuff from the system	 "
	-@echo "-------------------------------------------"
	-rm -r $(OBJS) $(FILENAME) *.ncb *.opt *.plg *.ilk *.idb *.pdb *.pch Debug/ Release/ ii_files/



clobber: clean
	-rm *.d

include ${MK_DIR}/mk.depend

