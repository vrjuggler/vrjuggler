# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VRJ.NET is (C) Copyright 2004 by Patrick Hartling
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

ObjectReader = ReferenceType('vpr', 'vpr::ObjectReader', 'vpr/IO/ObjectReader.h')
exclude(ObjectReader.attribExists)

ObjectWriter = ReferenceType('vpr', 'vpr::ObjectWriter', 'vpr/IO/ObjectWriter.h')
exclude(ObjectWriter.attribExists)

WriteableObject =  ReferenceType('vpr', 'vpr::WriteableObject',
                                 'vpr/IO/SerializableObject.h')
ReadableObject =  ReferenceType('vpr', 'vpr::ReadableObject',
                                'vpr/IO/SerializableObject.h')
SerializableObject = ReferenceType('vpr', 'vpr::SerializableObject',
                                   'vpr/IO/SerializableObject.h')

GUID = ReferenceType('vpr', 'vpr::GUID', 'vpr/Util/GUID.h')
sealed(GUID)
exclude(GUID.mGuid)
exclude(GUID.StdGuid)
exclude(GUID.hash)

Interval = ReferenceType('vpr', 'vpr::Interval', 'vpr/Util/Interval.h')
sealed(Interval)

ReturnStatus = ReferenceType('vpr', 'vpr::ReturnStatus',
                             'vpr/Util/ReturnStatus.h')
sealed(ReturnStatus)
