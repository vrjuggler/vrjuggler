# VRJ.NET is (C) Copyright 2004 by Patrick Hartling
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$

default: all

CSC=		mbas /debug

LIBDIR=		$(VRJNET_BASE_DIR)/lib
EXE_NAME=	SimpleApp
EXE_DEPS=	/lib:$(LIBDIR) /r:vprnet.dll	\
		/r:jcclnet.dll			\
		/r:gmtlnet.dll			\
		/r:gadgetnet.dll		\
		/r:vrjnet.dll			\
		/r:Tao.OpenGl.dll

CS_SRCS=	AssemblyInfo.vb SimpleApp.vb

EXE=		$(EXE_NAME).exe

all: $(EXE)

$(EXE): $(CS_SRCS)
	$(CSC) /out:$@ $(CS_SRCS) $(EXE_DEPS)

clobber:
	rm -f $(EXE)
