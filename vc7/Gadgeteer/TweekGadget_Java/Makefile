# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

ANT=		..\..\..\external\ant\bin\ant.bat
IDL=		..\..\..\external\JacORB\idl.bat
IDL_OPTS=	
IDL_INCLUDES=	-I..\..\..\modules\tweek				\
		-I..\..\..\modules\gadgeteer\drivers\Open\Tweek\idl
IDL_DIR=	..\..\..\modules\gadgeteer\drivers\Open\Tweek\idl

gadget\TweekAnalogSubject.java: $(IDL_DIR)\TweekAnalogSubject.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES) $(IDL_DIR)\TweekAnalogSubject.idl

gadget\TweekDigitalSubject.java: $(IDL_DIR)\TweekDigitalSubject.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES) $(IDL_DIR)\TweekDigitalSubject.idl

gadget\TweekPositionSubject.java: $(IDL_DIR)\TweekPositionSubject.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES) $(IDL_DIR)\TweekPositionSubject.idl

VPATH=		gadget

JAVA_OUTPUT=	gadget\TweekAnalogSubject.java		\
		gadget\TweekDigitalSubject.java		\
		gadget\TweekPositionSubject.java

all: $(JAVA_OUTPUT) java

java:
	$(ANT)

clean:
	-rmdir /S /Q jccl
	$(ANT) clean
