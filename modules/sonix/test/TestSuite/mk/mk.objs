# If no object directory is being used, set it to the current directory.
OBJDIR		?= .
DEPDIR		?= .

# Define the list of supported source file suffixes.
_suffix_list = c C CC cc cpp c++ cxx

# Loop over the suffixes, translate them to $(OBJEXT) and store the
# result in $(OBJS).
makeobjs = $(filter %.$(OBJEXT), $(foreach _suffix, $(_suffix_list), $(1:.$(_suffix)=.$(OBJEXT))))

# add a directory prefix to each source file
# OBJS := ${addprefix ${OBJDIR}/,$(SRCS)}
# Construct $(OBJS) from $(SRCS) using the makeobjs function.
OBJS = $(call makeobjs, $(SRCS))

# Rules for compiling
CREATE_OBJ_FILE_COMMAND = $(CXX_COMPILE) $(WARNINGS_FLAGS) $(OBJ_NAME_FLAG) -c $<
CREATE_OBJ_FILE_COMMAND_C = $(C_COMPILE) $(WARNINGS_FLAGS) $(OBJ_NAME_FLAG) -c $<

$(OBJDIR)/%.$(OBJEXT): %.c
	$(CREATE_OBJ_FILE_COMMAND_C)

$(OBJDIR)/%.$(OBJEXT): %.cxx
	$(CREATE_OBJ_FILE_COMMAND)

$(OBJDIR)/%.$(OBJEXT): %.c++
	$(CREATE_OBJ_FILE_COMMAND)

$(OBJDIR)/%.$(OBJEXT): %.cc
	$(CREATE_OBJ_FILE_COMMAND)

$(OBJDIR)/%.$(OBJEXT): %.C
	$(CREATE_OBJ_FILE_COMMAND)

$(OBJDIR)/%.$(OBJEXT): %.cpp
	$(CREATE_OBJ_FILE_COMMAND)

$(OBJDIR)/%.$(OBJEXT): %.C
	$(CREATE_OBJ_FILE_COMMAND)

# Search path for source code and object files.
vpath
vpath %.c ${PATH_TO_SRCS}
vpath %.C ${PATH_TO_SRCS}
vpath %.cpp ${PATH_TO_SRCS}
vpath %.cxx ${PATH_TO_SRCS}
vpath %.c++ ${PATH_TO_SRCS}
vpath %.cc ${PATH_TO_SRCS}
vpath %.${OBJEXT} ${OBJDIR}
