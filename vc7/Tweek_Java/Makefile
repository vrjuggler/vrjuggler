# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
# *************** <auto-copyright.pl END do not edit this line> ***************

ANT=		..\..\external\ant\bin\ant
IDL=		..\..\external\JacORB\idl
IDL_OPTS=	
IDL_INCLUDES=	-I..\..\modules\tweek
IDL_DIR=	..\..\modules\tweek\tweek\idl

tweek\Subject.java: $(IDL_DIR)\Subject.idl
	$(IDL) $(IDL_OPTS) -noskel $(IDL_INCLUDES) $(IDL_DIR)\Subject.idl

tweek\SubjectManager.java: $(IDL_DIR)\SubjectManager.idl
	$(IDL) $(IDL_OPTS) -noskel $(IDL_INCLUDES) $(IDL_DIR)\SubjectManager.idl

tweek\Observer.java: $(IDL_DIR)\Observer.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES) $(IDL_DIR)\Observer.idl

tweek\BeanDeliverySubject.java: $(IDL_DIR)\BeanDeliverySubject.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES) $(IDL_DIR)\BeanDeliverySubject.idl

VPATH=		tweek

JAVA_OUTPUT=	tweek\SubjectManager.java		\
		tweek\Subject.java			\
		tweek\Observer.java			\
		tweek\BeanDeliverySubject.java

all: $(JAVA_OUTPUT) java

java:
	$(ANT)
	$(ANT) -f build-ext.xml

clean:
	-rmdir /S /Q tweek
	$(ANT) clean
	$(ANT) -f build-ext.xml clean
