# VRJ.NET is (C) Copyright 2004 by Patrick Hartling
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$

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
