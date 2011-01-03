# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
#
# *************** <auto-copyright.pl END do not edit this line> ***************

ANT=		..\..\..\external\ant\bin\ant
IDL=		..\..\..\external\JacORB\idl
IDL_OPTS=	
IDL_INCLUDES=	-I..\..\..\modules\tweek			\
		-I..\..\..\modules\jackal\plugins\corba_rtrc
IDL_DIR=	..\..\..\modules\jackal\plugins\corba_rtrc\idl

jccl\RemoteReconfigSubject.java: $(IDL_DIR)\RemoteReconfigSubject.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES) $(IDL_DIR)\RemoteReconfigSubject.idl

VPATH=		jccl

JAVA_OUTPUT=	jccl\RemoteReconfigSubject.java

all: $(JAVA_OUTPUT) java

java:
	$(ANT)

clean:
	-rmdir /S /Q jccl
	$(ANT) clean
