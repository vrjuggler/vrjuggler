# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

ANT=		..\..\external\ant\bin\ant

all: java

java:
	$(ANT) -f commoneditors\build-commoneditors.xml
	$(ANT) -f build-cave.xml
	$(ANT) -f build-display_window.xml
	$(ANT) -f build-flock.xml
	$(ANT) -f build-intersense.xml
	$(ANT) -f build-motionstar.xml
	$(ANT) -f build-pinchglove.xml
	$(ANT) -f build-proxyeditor.xml
	-$(ANT) -f build-surfacedisplayeditor.xml
	$(ANT) -f build-wizard-cluster.xml
	$(ANT) -f build-wizard-newdevice.xml
	$(ANT) -f build-wizard-vrsystem.xml
	$(ANT)

clean:
	$(ANT) clean
	$(ANT) -f commoneditors\build-commoneditors.xml clean
	$(ANT) -f build-cave.xml clean
	$(ANT) -f build-display_window.xml clean
	$(ANT) -f build-flock.xml clean
	$(ANT) -f build-intersense.xml clean
	$(ANT) -f build-motionstar.xml clean
	$(ANT) -f build-pinchglove.xml clean
	$(ANT) -f build-proxyeditor.xml clean
	$(ANT) -f build-surfacedisplayeditor.xml clean
	$(ANT) -f build-wizard-cluster.xml clean
	$(ANT) -f build-wizard-newdevice.xml clean
	$(ANT) -f build-wizard-vrsystem.xml clean
