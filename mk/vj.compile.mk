# -----------------------------------------------------------------------------
# $Id$
#
# This include file <vj.compile.mk> handles source code compiling.
# -----------------------------------------------------------------------------
# The Makefile including this file should define the following variables:
#
# OBJDIR          - The directory where the object file will be created.
# C_COMPILE       - The C compiler command line (without -c or -o <outfile>
#                   options).
# CXX_COMPILE     - The C++ compiler command line (without -c or -o <outfile>
#                   options).
# OBJ_NAME_FLAG   - Flag (with any extra options) used for naming the object
#                   file being compiled here.
# -----------------------------------------------------------------------------

${OBJDIR}/%.o : %.c
	${C_COMPILE} -c ${OBJ_NAME_FLAG} $<

${OBJDIR}/%.o : %.cpp
	${CXX_COMPILE} -c ${OBJ_NAME_FLAG} $<

${OBJDIR}/%.o : %.C
	${CXX_COMPILE} -c ${OBJ_NAME_FLAG} $<
