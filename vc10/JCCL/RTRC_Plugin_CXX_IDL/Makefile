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

IDL=		$(OMNIORB_BIN)\omniidl
IDL_OPTS=	-bcxx -Wbh=.h,s=.cpp -C$(IDL_OUT_DIR)
IDL_INCLUDES=	-I../../../modules/tweek -I../../../modules/jackal/plugins
IDL_OUT_DIR=	..\RTRC_Plugin_CXX
TWEEK_IDL_DIR=	..\..\..\modules\tweek\tweek\idl
IDL_SRC_DIR=	..\..\..\modules\jackal\plugins\corba_rtrc\idl

$(IDL_OUT_DIR)\RemoteReconfigSubject.cpp $(IDL_OUT_DIR)\RemoteReconfigSubject.h: \
   $(IDL_SRC_DIR)\RemoteReconfigSubject.idl $(TWEEK_IDL_DIR)\Subject.idl
	$(IDL) $(IDL_OPTS) $(IDL_INCLUDES) \
           $(IDL_SRC_DIR)\RemoteReconfigSubject.idl

BASE_OUTPUT=	\
		$(IDL_OUT_DIR)\RemoteReconfigSubject.cpp	\
		$(IDL_OUT_DIR)\RemoteReconfigSubject.h

OUTPUT=		$(BASE_OUTPUT)

all: $(OUTPUT)

clean:
	-del $(BASE_OUTPUT)
