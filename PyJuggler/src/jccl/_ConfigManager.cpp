// PyJuggler is (C) Copyright 2002-2005 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <jccl/RTRC/ConfigElementHandler.h>
#include <jccl/RTRC/ConfigManager.h>

// Using =======================================================================
using namespace boost::python;


// Module ======================================================================
void _Export_ConfigManager()
{
   scope* jccl_ConfigManager_scope = new scope(
   class_<jccl::ConfigManager, boost::noncopyable>("ConfigManager",
       "Dynamic reconfiguration management for JCCL.  The Config Manager\n"
       "provides a complete solution for configuration an application via\n"
       "JCCL's ConfigElement type.  The Config Manager can configure based\n"
       "on static configuration files or dynamically via a network\n"
       "interface.\n\n"
       "The Config Manager can be used in a number of ways.  It provices a\n"
       "complete default solution to configuration, but it also exposes\n"
       "enough of its inner workings to allow applications to implement\n"
       "their own dynamic configuration algorithms.\n\n"
       "The simplest way to use the Config Manager is to create one or more\n"
       "types that implement the jccl.ConfigElementHandler interfaces and\n"
       "register instances of these using the method\n"
       "jccl.ConfigManager.addConfigElementHandler().\n\n"
       "Requests to add config elements can be added via the network\n"
       "connection or by the addPending*() methods of this class.  These\n"
       "requests are added to the Config Manager's \"pending\" list.  The\n"
       "Config Manager also maintains an \"active\" list that contains all\n"
       "the config elements that have been handled successfully.\n\n"
       "Once config element handlers have been registered with the Config\n"
       "Manager, the application should periodically call the method\n"
       "attemptReconfiguration().  This will try to match items in the\n"
       "pending list with instances of jccl.ConfigElementHandler that know\n"
       "how to handle them.\n\n"
       "For more advanced uses, jccl.ConfigManager provides accessor\n"
       "functions that allow direct manipulation of the pending and active\n"
       "lists.  This allows an application to decide on its own when and how\n"
       "to process requests in the pending list.  However, the\n"
       "attemptReconfiguration() interface should be sufficient for almost\n"
       "all uses."
       ,
       no_init
      )
      .def("getElementFromActive", &jccl::ConfigManager::getElementFromActive,
           "getElementFromActive(elementName) -> ConfigElement object\n"
           "Attempts to find a config element matching the given name in the\n"
           "active list.  If such an element is found, it is returned to the\n"
           "caller.  If not, an empty jccl.ConfigElement is returned.\n"
           "Arguments:\n"
           "elementName -- The name of the element to find."
       )
      .def("getElementFromPending",
           &jccl::ConfigManager::getElementFromPending,
           "getElementFromPending(elementName) -> ConfigElement object\n"
           "Attempts to find a config element matching the given name in the\n"
           "pending list.  If such an element is found, it is returned to the\n"
           "caller.  If not, an empty jccl.ConfigElement is returned.\n"
           "Arguments:\n"
           "elementName -- The name of the element to find."
       )
      .def("getElementNamed", &jccl::ConfigManager::getElementNamed,
           "getElementNamed(elementName) -> ConfigElement object\n"
           "Attempts to find a config element matching the given name, first\n"
           "in the active list.  If the element is not found in the active\n"
           "list, the pending list is searched.  If an element with a\n"
           "matching name is found in either list, it is returned to the\n"
           "caller.  If not, an empty jccl.ConfigElement is returned.\n"
           "Arguments:\n"
           "elementName -- The name of the element to find."
       )
      .def("isElementInActiveList",
           &jccl::ConfigManager::isElementInActiveList,
           "isElementInActiveList(elementName) -> Boolean\n"
           "Checks if the named config element is in the active\n"
           "configuration.  This locks the active list to do processing.\n"
           "Arguments:\n"
           "elementName -- The name of the element to find.\n\n"
           "NOTE: This should not be used often.  Use at your own risk."
       )
      .def("isElementTypeInActiveList",
           &jccl::ConfigManager::isElementTypeInActiveList,
           "isElementTypeInActiveList(elementType) -> Boolean\n"
           "Is the element of this type in the active configuration?\n"
           "This locks the active list to do processing.\n"
           "Arguments:\n"
           "elementType -- The type of the element to find.\n\n"
           "NOTE: This should not be used often.  Use at your own risk."
       )
      .def("isElementTypeInPendingList",
           &jccl::ConfigManager::isElementTypeInPendingList,
           "isElementTypeInPendingList(elementType) -> Boolean\n"
           "Is the element of this type in the pending list?\n"
           "This locks the pending list to do processing.\n"
           "Arguments:\n"
           "elementType -- The type of the element to find.\n\n"
           "NOTE: This should not be used often.  Use at your own risk."
       )
      .def("hasElementType", &jccl::ConfigManager::hasElementType,
           "hasElementType(elementType) -> Boolean\n"
           "Searches the active list for an element matching the given type.\n"
           "If none is found, the pending list is searched.\n"
           "Arguments:\n"
           "elementType -- The type of config element for which a search of\n"
           "               the pending and active lists will be performed.\n"
           "Returns:\n"
           "True is returned if an element of the given type is found in\n"
           "either the active or the pending list (in that order).  False is\n"
           "returned if no such element is found in either list."
       )
      .def("addConfigElementHandler",
           &jccl::ConfigManager::addConfigElementHandler,
           "addConfigElementHandler(handler)"
           "Registers the given handler (a subclass of\n"
           "jccl.ConfigElementHandler."
       )
      .def("removeConfigElementHandler",
           &jccl::ConfigManager::removeConfigElementHandler,
           "removeConfigElementHandler(handler)"
           "Un-registers the given handler (a subclass of\n"
           "jccl.ConfigElementHandler."
       )
      .def("instance", &jccl::ConfigManager::instance,
           return_value_policy< reference_existing_object >())
      .staticmethod("instance")
   );
   scope* jccl_ConfigManager_PendingElement_scope = new scope(
   class_<jccl::ConfigManager::PendingElement>("PendingElement", init<>())
      .def_readwrite("mType", &jccl::ConfigManager::PendingElement::mType)
      .def_readwrite("mElement",
                     &jccl::ConfigManager::PendingElement::mElement)
   );
   enum_<jccl::ConfigManager::PendingElement::Type>("Type")
      .value("ADD", jccl::ConfigManager::PendingElement::ADD)
      .value("REMOVE", jccl::ConfigManager::PendingElement::REMOVE)
   ;
   delete jccl_ConfigManager_PendingElement_scope;
   delete jccl_ConfigManager_scope;
}
