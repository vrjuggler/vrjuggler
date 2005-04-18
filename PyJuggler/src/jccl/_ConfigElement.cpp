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
#include <jccl/Config/ConfigElement.h>
#include <container_conversions.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj
{

template<typename T>
T ConfigElement_getProperty(jccl::ConfigElement* elt,
                            const std::string& propName, int index)
{
   return elt->template getProperty<T>(propName, index);
}

template bool ConfigElement_getProperty<bool>(jccl::ConfigElement*,
                                              const std::string&, int);
template int ConfigElement_getProperty<int>(jccl::ConfigElement*,
                                            const std::string&, int);
template double ConfigElement_getProperty<double>(jccl::ConfigElement*,
                                                  const std::string&, int);
template jccl::ConfigElementPtr ConfigElement_getProperty<jccl::ConfigElementPtr>(jccl::ConfigElement*,
                                                                                  const std::string&, int);
template std::string ConfigElement_getProperty<std::string>(jccl::ConfigElement*,
                                                            const std::string&,
                                                            int);

template<typename T>
bool ConfigElement_setProperty(jccl::ConfigElement* elt,
                               const std::string& propName, const int index,
                               T value)
{
   return elt->setProperty(propName, index, value);
}

// Only provide specializations for the signatures that are not already
// specialized in jccl::ConfigElement.
template bool ConfigElement_setProperty(jccl::ConfigElement*,
                                        const std::string&, const int, int);
template bool ConfigElement_setProperty(jccl::ConfigElement*,
                                        const std::string&, const int, double);
template bool ConfigElement_setProperty(jccl::ConfigElement*,
                                        const std::string&, const int,
                                        std::string);

}


// Module ======================================================================
void _Export_ConfigElement()
{
    scope* jccl_ConfigElement_scope = new scope(
    class_< jccl::ConfigElement >("ConfigElement",
         "A unit of configuration information.\n"
         "References a config element DOM node and a definition for the given\n"
         "configuration element type."
         ,
         init<  >(
            "__init__()\n"
            "Default constructor.\n\n"
            "__init__(def)\n"
            "Constructs a ConfigElement matching the given definition.\n"
            "Arguments:\n"
            "def -- A ConfigDefinition object that refers to a valid\n"
            "       ConfigDefinition.\n\n"
            "__init__(elt)\n"
            "Copy constructor."
         )
        )
        .def(init< jccl::ConfigDefinitionPtr >())
        .def(init< const jccl::ConfigElement& >())
        .def("isValid", &jccl::ConfigElement::isValid)
        .def("getChildElement", &jccl::ConfigElement::getChildElement,
             "getChildElement(path) -> ConfigElement\n"
             "Gets a child element from a property of self.\n"
             "Arguments:\n"
             "path -- The complete name of a child element in self.\n"
             "        The form is, for example, \"property_token/instance_name\".\n"
             "NOTE: This uses the name attribute of the elements.\n"
             "Returns:\n"
             "A ConfigElement pointing to the child element.  This may be\n"
             "a NULL ConfigElement if the child element name was not found.\n"
             "Check before use."
         )
        .def("getNum", &jccl::ConfigElement::getNum,
             "getNum(property) -> int\n"
             "Returns the number of values for the specified property.\n"
             "Arguments:\n"
             "property -- The token of a property in this element.\n"
             "Returns:\n"
             "The number of values that exist for the given property which\n"
             "may be zero."
         )
        .def("getName", &jccl::ConfigElement::getName,
             "getName() -> string\n"
             "Returns the instance name of this ConfigElement."
         )
        .def("getFullName", &jccl::ConfigElement::getFullName,
             "getFullName() -> string\n"
             "Returns the fully qualified, unique name of this element.\n"
             "This will be different from getName() when this element is a\n"
             "child of another element.  In that case, the name will be\n"
             "based on the element hierarchy and the property token.  The\n"
             "format in that case will be\n"
             "\"element name 0/proeprty_token_0/element name 1/property_token_1/...\""
         )
        .def("getVersion", &jccl::ConfigElement::getVersion,
             "getVersion() -> int\n"
             "Returns the version number of the definition used by this\n"
             "ConfigElement.")
        .def("getID", &jccl::ConfigElement::getID,
             "getID() -> string\n"
             "Returns the string that identifies self's ConfigDefinition.")
        .def("getPropertyBool",
             (bool (*)(jccl::ConfigElement*, const std::string&, int))pyj::ConfigElement_getProperty<bool>,
             "getPropertyBool(prop, index) -> Boolean\n"
             "Returns a Boolean value from the named property in self.\n"
             "Arguments:\n"
             "prop -- The token string for the desired property.\n"
             "index -- The index of the property's list of values.  Use\n"
             "         getNum() to determine the number of values for a\n"
             "         given property.  The valid range is 0 to getNum() - 1.\n"
         )
        .def("getPropertyInt",
             (int (*)(jccl::ConfigElement*, const std::string&, int))pyj::ConfigElement_getProperty<int>,
             "getPropertyInt(prop, index) -> int\n"
             "Returns an integer value from the named property in self.\n"
             "Arguments:\n"
             "prop -- The token string for the desired property.\n"
             "index -- The index of the property's list of values.  Use\n"
             "         getNum() to determine the number of values for a\n"
             "         given property.  The valid range is 0 to getNum() - 1.\n"
         )
        .def("getPropertyFloat",
             (double (*)(jccl::ConfigElement*, const std::string&, int))pyj::ConfigElement_getProperty<double>,
             "getPropertyFloat(prop, index) -> floating-point value\n"
             "Returns a floating-point value from the named property in self.\n"
             "Arguments:\n"
             "prop -- The token string for the desired property.\n"
             "index -- The index of the property's list of values.  Use\n"
             "         getNum() to determine the number of values for a\n"
             "         given property.  The valid range is 0 to getNum() - 1.\n"
         )
        .def("getPropertyString",
             (std::string (*)(jccl::ConfigElement*, const std::string&, int))pyj::ConfigElement_getProperty<std::string>,
             "getPropertyString(prop, index) -> string object\n"
             "Returns a string object from the named property in self.\n"
             "Arguments:\n"
             "prop -- The token string for the desired property.\n"
             "index -- The index of the property's list of values.  Use\n"
             "         getNum() to determine the number of values for a\n"
             "         given property.  The valid range is 0 to getNum() - 1.\n"
         )
        .def("getPropertyConfigElement",
             (jccl::ConfigElementPtr (*)(jccl::ConfigElement*, const std::string&, int))pyj::ConfigElement_getProperty<jccl::ConfigElementPtr>,
             "getPropertyConfigElement(prop, index) -> ConfigElement object\n"
             "Returns a ConfigElement object from the named property in self.\n"
             "Arguments:\n"
             "prop -- The token string for the desired property.\n"
             "index -- The index of the property's list of values.  Use\n"
             "         getNum() to determine the number of values for a\n"
             "         given property.  The valid range is 0 to getNum() - 1.\n"
         )
        .def("setProperty",
             (bool (jccl::ConfigElement::*)(const std::string&, const int, bool) )&jccl::ConfigElement::setProperty,
             "setProperty(prop, index, value) -> Boolean\n"
             "Sets a value for the given property.\n"
             "Arguments:\n"
             "prop  -- The token string for the property whose value will be\n"
             "         set.\n"
             "index -- The index of the property's list of values.\n"
             "value -- The value to set.  It may be of type Boolean, integer,\n"
             "         float, string, or ConfigElement."
         )
        .def("setProperty", (bool (jccl::ConfigElement::*)(const std::string&, int, jccl::ConfigElementPtr) )&jccl::ConfigElement::setProperty)
        .def("setProperty",
             (bool (*)(jccl::ConfigElement*, const std::string&, const int, int))pyj::ConfigElement_setProperty)
        .def("setProperty",
             (bool (*)(jccl::ConfigElement*, const std::string&, const int, double))pyj::ConfigElement_setProperty)
        .def("setProperty",
             (bool (*)(jccl::ConfigElement*, const std::string&, const int, std::string))pyj::ConfigElement_setProperty)
        .def("getElementPtrDependencies",
             &jccl::ConfigElement::getElementPtrDependencies,
             "getElementPtrDependencies() -> jccl.StringVec (indexable container)\n"
             "Returns an indexable container of self's dependencies as string\n"
             "objects.  Dependencies are any config element objects named by\n"
             "an \"Element Pointer\" proeprty of self (or any element\n"
             "embedded in self).\n\n"
             "Returns:\n"
             "A jccl.StringVec that contains the names of all config elements\n"
             "referenced by self, which can be used for dependency checking."
         )
        .def("getChildElements", &jccl::ConfigElement::getChildElements,
             "getChildElements() -> ConfigElement tuple\n"
             "Returns a tuple of self's child (embedded) elements.\n\n"
             "Returns:\n"
             "A tuple of ConfigElement objects that are the config elements\n"
             "embedded in self."
         )
        .def("setDefinition", &jccl::ConfigElement::setDefinition,
             "setDefinition(def)\n"
             "Associates the given definition with this element."
         )
        .def("getConfigDefinition", &jccl::ConfigElement::getConfigDefinition,
             "getDefinition() -> ConfigDefinition\n"
             "Returns the ConfigDefinition for this config element."
         )
        .def(self_ns::str(self))
        .def( self == self )
        .def( self != self )
        .def( self < self )
    );
    register_ptr_to_python< boost::shared_ptr< jccl::ConfigElement > >();
    delete jccl_ConfigElement_scope;

    class_< std::vector<std::string> >("StringVec",
        "An indexable container of string objects."
        )
        .def(vector_indexing_suite< std::vector<std::string> >())
    ;

    pyj::std_vector_copyable_to_tuple<jccl::ConfigElementPtr>();
}
