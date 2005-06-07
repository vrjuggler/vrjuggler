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
# File:          dzr.java-app.mk,v
# Date modified: 2003/10/11 07:27:13
# Version:       1.2
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Basic Java application mk file.  This sets all the general variables that
# will be needed to compile Java code.
#
# dzr.java-app.mk,v 1.2 2003/10/11 07:27:13 patrickh Exp
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
# The following is the list of variables that users can set in their makefiles
# to extend the default values:
#
# EXTRA_JAVAC_FLAGS - Extra flags for the Java compiler.
# -----------------------------------------------------------------------------

include $(DZR_BASE_DIR)/mk/dzr.hosttype.mk

ifdef JDK_HOME
   JAVA_HOME?=	$(JDK_HOME)
else
   JAVA_HOME?=	/usr/java
endif

JAVAC?=		$(JAVA_HOME)/bin/javac
JAVAH?=		$(JAVA_HOME)/bin/javah
JAR?=		$(JAVA_HOME)/bin/jar

# Basic comipler debugging and optimization flags.
JAVA_DEBUG_FLAGS?=	-g $(EXTRA_JAVA_DEBUG_FLAGS)
JAVA_OPTIM_FLAGS?=	-O $(EXTRA_JAVA_OPTIM_FLAGS)

# Backwards compatibility shim.
ifeq ($(DEBUG_APP), TRUE)
   BUILD_TYPE?=	dbg
else
   BUILD_TYPE?=	opt
endif

# Add debugging or optimizing flags to $(EXTRA_JAVAC_FLAGS) depending on
# whether this application is compiled as a debugging application or an
# optimized application.
ifeq ($(BUILD_TYPE), dbg)
   EXTRA_JAVAC_FLAGS+=	$(JAVA_DEBUG_FLAGS)
else
   EXTRA_JAVAC_FLAGS+=	$(JAVA_OPTIM_FLAGS)
endif

ifdef CLASSPATH
   CLASSPATH_ARG=	-classpath "$(CLASSPATH)"
endif

ifdef EXTRA_SOURCEPATH_DIRS
   SOURCEPATH_ARG=	-sourcepath "$(srcdir)$(JCPS)$(EXTRA_SOURCEPATH_DIRS)"
else
   SOURCEPATH_ARG=	-sourcepath "$(srcdir)"
endif

# Basic compiler flags.  They are as follows:
#
#    JAVAC_FLAGS - Standard javac compiler flags
#
JAVAC_FLAGS=	$(SOURCEPATH_ARG) $(CLASSPATH_ARG) $(EXTRA_JAVAC_FLAGS)

# The compilers and the linker with all their flags.
JAVA_COMPILE=	$(JAVAC) $(JAVAC_FLAGS)

define DZR_JAVA_APP_echo_vars
@echo "Doozer Java App settings"
@echo "JAVA_COMPILE: $(JAVA_COMPILE)"
@echo "JAVAC_FLAGS: $(JAVAC_FLAGS)"
@echo "EXTRA_JAVAC_FLAGS: $(EXTRA_JAVAC_FLAGS)"
endef

include $(DZR_BASE_DIR)/mk/dzr.java.mk
