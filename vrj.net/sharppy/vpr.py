# VRJ.NET is (C) Copyright 2004 by Patrick Hartling
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$

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
