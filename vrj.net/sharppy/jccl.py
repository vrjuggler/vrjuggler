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

ConfigElement = ReferenceType('jccl', 'jccl::ConfigElement',
                              'jccl/Config/ConfigElement.h')
use_shared_ptr(ConfigElement)
sealed(ConfigElement)
exclude(ConfigElement.initFromNode)
exclude(ConfigElement.getNode)
exclude(ConfigElement.getElementPtrDependencies) # Returns a vector
exclude(ConfigElement.getChildElements) # This one does too
# The following are protected non-virtuals.  Exporting them does not work
# very well right now due to the complexities of shared_ptrs.
exclude(ConfigElement.getPropertyString)
exclude(ConfigElement.getPropertyCdataNode)
exclude(ConfigElement.getProperty_bool)
exclude(ConfigElement.getProperty_ElementPtr)

ConfigDefinition = ReferenceType('jccl', 'jccl::ConfigDefinition',
                                 'jccl/Config/ConfigDefinition.h')
use_shared_ptr(ConfigDefinition)
sealed(ConfigDefinition)
exclude(ConfigDefinition.setNode)
exclude(ConfigDefinition.getNode)
exclude(ConfigDefinition.getAllPropertyDefinitions) # Returns a vector
exclude(ConfigDefinition.getPropertyDefinition)

#PropertyDefinition = ReferenceType('jccl', 'jccl::PropertyDefinition',
#                                   'jccl/Config/PropertyDefinition.h')
##exclude(PropertyDefinition.PropertyDefinition[1])
#exclude(PropertyDefinition.getNode)
#sealed(ConfigElement)

ConfigElementHandler = ReferenceType('jccl', 'jccl::ConfigElementHandler',
                                     'jccl/RTRC/ConfigElementHandler.h')
