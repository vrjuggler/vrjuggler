# ************** <auto-copyright.pl BEGIN do not edit this line> **************
# Doozer
#
# Original Authors:
#   Patrick Hartling, Kevin Meinert
# -----------------------------------------------------------------------------
# VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          dzr.lib.mk,v
# Date modified: 2003/10/11 07:27:13
# Version:       1.19
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Basic library mk file.  This sets all the general variables that will be
# needed to compile and link libraries.
#
# dzr.lib.mk,v 1.19 2003/10/11 07:27:13 patrickh Exp
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# The following is the list of variables that users can set in their makefiles
# to extend the default values:
#
# EXTRA_CFLAGS   - Extra flags for the C compiler.
# EXTRA_CXXFLAGS - Extra flags for the C++ compiler.
# EXTRA_INCLUDES - Extensions to the default include path in the form
#                  -I/some/dir/include.
# EXTRA_ARFLAGS  - Extra flags for the archiver.
# EXTRA_LDFLAGS  - Extra flags for the linker.
# EXTRA_SYS_LIBS - Extra system libraries needed for linking.
# -----------------------------------------------------------------------------

# Determine the name of this platform.
include $(DZR_BASE_DIR)/mk/dzr.hosttype.mk

# Basic comipler debugging and optimization flags.
DEBUG_FLAGS?=	$(OS_DEBUG_FLAGS) $(EXTRA_DEBUG_FLAGS)
OPTIM_FLAGS?=	$(OS_OPTIM_FLAGS) $(EXTRA_OPTIM_FLAGS)

# Backwards compatibility shim.
ifeq ($(DEBUG_APP), TRUE)
   BUILD_TYPE?=	dbg
else
   BUILD_TYPE?=	opt
endif

# Add debugging or optimizing flags to $(EXTRA_CFLAGS) and $(EXTRA_CXXFLAGS)
# depending on whether this library is compiled as a debugging library or an
# optimized library.
ifeq ($(BUILD_TYPE), dbg)
   EXTRA_CFLAGS+=	$(DEBUG_FLAGS)
   EXTRA_CXXFLAGS+=	$(DEBUG_FLAGS)
else
   EXTRA_CFLAGS+=	$(OPTIM_FLAGS)
   EXTRA_CXXFLAGS+=	$(OPTIM_FLAGS)
endif

# Basic compiler flags.  They are as follows:
#
#    CFLAGS   - Standard C compiler flags
#    CXXFLAGS - Standard C++ compiler flags
#    ARFLAGS  - Standard archiver flags
#    LDFLAGS  - Standard linker flags
#
CFLAGS=		$(OS_ABI_FLAGS) $(OS_CFLAGS) $(INCLUDES) $(EXTRA_CFLAGS)
CXXFLAGS=	$(OS_ABI_FLAGS) $(OS_CXXFLAGS) $(INCLUDES) $(EXTRA_CXXFLAGS)
ARFLAGS=	$(OS_LIB_FLAGS) $(OS_ARFLAGS) $(EXTRA_ARFLAGS)
LDFLAGS=	$(OS_SHLIB_FLAGS) $(OS_LDFLAGS) $(EXTRA_LDFLAGS)

# The default include path.  We assume that the users will have .h files in
# their source directory.  It should not hurt to assume that unless the user
# is doing something bad.  In that case, it is their responsibility to resolve
# it.
INCLUDES=	-I$(srcdir) $(OS_INCLUDES) $(EXTRA_INCLUDES)

# Command to set the umask used for compiled files.
ifneq ($(UMASK), )
SET_UMASK=	$(UMASK) $(UMASK_VAL) ;
endif

# The compilers and the linker with all their flags.
C_COMPILE=	$(CC) $(CFLAGS)
CXX_COMPILE=	$(CXX) $(CXXFLAGS) 
STATIC_LINK=	$(AR) $(ARFLAGS)
SHARED_LINK=	$(LD) $(LDFLAGS)
SHARED_OUT=	$(OS_LD_NAME_FLAG)
STATIC_OUT=	$(OS_AR_NAME_FLAG)
LINKALL_BEGIN =	$(OS_LINKALL_BEGIN)
LINKALL_END =	$(OS_LINKALL_END)
LIBPATH_FLAG =	$(OS_LIBPATH_FLAG)


LIB_EXT=		$(OS_LIB_EXT)
SHLIB_EXT=	$(OS_SHLIB_EXT)
LIB_PREFIX=	$(OS_LIB_PREFIX)
SHLIB_PREFIX=	$(OS_SHLIB_PREFIX)
LIBBITSUF = 	$(OS_LIBBITSUF)

include $(DZR_BASE_DIR)/mk/dzr.obj.mk
include $(DZR_BASE_DIR)/mk/dzr.clean.mk
include $(DZR_BASE_DIR)/mk/dzr.dep.mk
