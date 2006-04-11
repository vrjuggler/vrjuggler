# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

# -----------------------------------------------------------------------------
# Device driver list for "normal" users.
# -----------------------------------------------------------------------------

# Do not modify this line.
DRIVERS=

# Comment out any of the following to disable building a specific driver.
DRIVERS+=	5DT/DataGlove
DRIVERS+=	Ascension/Flock
DRIVERS+=	Ascension/MotionStar
DRIVERS+=	Fakespace/PinchGlove
DRIVERS+=	Immersion/IBox
DRIVERS+=	Intersense/IS900
DRIVERS+=	Intersense/IntersenseAPI
DRIVERS+=	Logitech/ThreeDMouse
#DRIVERS+=	Open/DTK
DRIVERS+=       Open/LinuxJoydev
DRIVERS+=	Open/Trackd
DRIVERS+=	Open/VRPN
DRIVERS+=	Polhemus/Fastrak
DRIVERS+=	Tweek
#DRIVERS+=	VRCO/TrackdAPI
DRIVERS+=	VirtualTechnologies/CyberGlove
