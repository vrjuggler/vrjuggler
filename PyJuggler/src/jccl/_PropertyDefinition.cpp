// PyJuggler is (C) Copyright 2002-2005 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Includes ====================================================================
#include <boost/python.hpp>
#include <jccl/Config/PropertyDefinition.h>

// Using =======================================================================
using namespace boost::python;


// Module ======================================================================
void _Export_PropertyDefinition()
{
   class_< jccl::PropertyDefinition >("PropertyDefinition",
       "Describes properties.  Holds all the information describing a\n"
       "property.  Instances of this class are read-only objects.\n\n"
       "Properties are composed to make up a config element.  Each property\n"
       "has zero or more values depending on how it is defined.  The basic\n"
       "type of all the values for a given property will be the same."
       ,
       init<  >(
          "__init__()\n"
          "Default constructor.  self is initialized with default values\n\n"
          "__init__(propdef)\n"
          "Copy constructor."
       )
      )
      .def(init< const jccl::PropertyDefinition& >())
      .def("getToken", &jccl::PropertyDefinition::getToken,
           "getToken() -> string object\n"
           "Returns the token (a valid XML element identifier) of this\n"
           "property."
       )
      .def("getName", &jccl::PropertyDefinition::getName,
           "getName() -> string object\n"
           "Returns the human-readable name of this property."
       )
      .def("getHelp", &jccl::PropertyDefinition::getHelp,
           "getHelp() -> string object\n"
           "Returns the help text for this definition."
       )
      .def("getVarType", &jccl::PropertyDefinition::getVarType,
           "getVarType() -> VarType object\n"
           "Returns an identifier that indicates the type of the values of\n"
           "this proeprty."
       )
      .def("getNumAllowed", &jccl::PropertyDefinition::getNumAllowed,
           "getNumAllowed() -> int\n"
           "Returns the numbmer of values allowed for this property."
       )
      .def("getDefaultValueString",
           &jccl::PropertyDefinition::getDefaultValueString,
           "getDefaultValueString(index) -> string object\n"
           "Retrieves the default value (as an XML string) for the i'th\n"
           "item in this property definition."
       )
      .def(self_ns::str(self))
      .def( self == self )
      .def( self != self )
   ;
}
