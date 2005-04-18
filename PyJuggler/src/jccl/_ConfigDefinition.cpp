// PyJuggler is (C) Copyright 2002-2005 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Includes ====================================================================
#include <jccl/Config/ConfigDefinition.h>

// Using =======================================================================
using namespace boost::python;


// Module ======================================================================
void _Export_ConfigDefinition()
{
   scope* jccl_ConfigDefinition_scope = new scope(
   class_< jccl::ConfigDefinition >("ConfigDefinition",
      "Definition of a single configuration element type.",
      init<  >(
         "__init__()\n"
         "Default constructor.\n\n"
         "__init__(def)\n"
         "Copy constructor."
       )
      )
      .def(init< const jccl::ConfigDefinition& >())
      .def("getName", &jccl::ConfigDefinition::getName,
           "getName() -> string object\n"
           "Returns the human-readable name of this definition."
       )
      .def("getToken", &jccl::ConfigDefinition::getToken,
           "getToken() -> string object\n"
           "Returns the token (a valid XML element identifier) of this\n"
           "definition.  This corresponds to the ID of a config element."
       )
      .def("getVersion", &jccl::ConfigDefinition::getVersion,
           "getVersion() -> int\n"
           "Gets the version of this definition.  If it is not set, 0 is\n"
           "returned."
       )
      .def("getHelp", &jccl::ConfigDefinition::getHelp,
           "getHelp() -> string object\n"
           "Returns the help text for this definition."
       )
      .def("isParent", &jccl::ConfigDefinition::isParent,
           "isParent(token) -> Boolean\n"
           "Tests to see if we derive from the given ConfigDefinition.\n"
           "Arguments:\n"
           "token -- A token for the ConfigDefinition that may be our\n"
           "         parent.\n"
           "Returns:\n"
           "True is returned if the named ConfigDefinition type is a\n"
           "parent of this definition.  False is returned otherwise."
       )
      .def("getPropertyDefinition",
           &jccl::ConfigDefinition::getPropertyDefinition,
           "getPropertyDefinition(token) -> PropertyDefinition object\n"
           "Gets a PropertyDefinition object from self with matchin token.\n"
           "Arguments:\n"
           "token -- A string that is the token of the desired\n"
           "         PropertyDefinition object.\n"
           "Returns:\n"
           "A PropertyDefinition object in self whose token matches the\n"
           "given value."
       )
      .def("getAllPropertyDefinitions",
          &jccl::ConfigDefinition::getAllPropertyDefinitions,
          "getAllPropertyDefinitions() -> jccl.PropertyDefinitionVec (indexable container)\n"
          "Gets an indexable container of all the PropertyDefinition objects\n"
          "from self."
       )
      .def(self_ns::str(self))
      .def( self == self )
      .def( self != self )
   );
   register_ptr_to_python< boost::shared_ptr< jccl::ConfigDefinition > >();
   delete jccl_ConfigDefinition_scope;

   class_< std::vector<jccl::PropertyDefinition> >("PropertyDefinitionVec",
       "An indexable container of jccl.PropertyDefinition objects"
       )
       .def(vector_indexing_suite< std::vector<jccl::PropertyDefinition> >())
   ;
}
