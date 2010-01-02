/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <jccl/Plugins/PluginConfig.h>

#include <string>
#include <list>
#include <sstream>

#include <jccl/Config/ElementFactory.h>
#include <jccl/Config/Configuration.h>
#include <jccl/Config/ConfigDefinitionRepository.h>
#include <jccl/Config/ConfigElement.h>
#include <jccl/RTRC/ConfigManager.h>

#include <corba_rtrc/RemoteReconfigSubjectImpl.h>

namespace jccl
{

void RemoteReconfigSubjectImpl::add(const char* configurationString)
   throw(CORBA::SystemException)
{
   jccl::Configuration new_elements;
   std::string cfg_string(configurationString);

   // Create a string that contains a full JCCL configuration.
   // Loading from an istream.
   std::istringstream input(cfg_string);

   input >> new_elements;

   //std::cout << "Adding a new Element: " << std::endl << cfg_string << std::endl;

   //Send pending changes to ConfigManager
   jccl::ConfigManager::instance()->addConfigurationAdditions(&new_elements);
}


void RemoteReconfigSubjectImpl::remove(const char* configurationString)
   throw(CORBA::SystemException)
{
   jccl::Configuration delete_elements;
   std::string cfg_string(configurationString);

   // Create a string that contains a full JCCL configuration.
   // Loading from an istream.
   std::istringstream input(cfg_string);

   input >> delete_elements;

   //std::cout << "Removing a new Element: " << std::endl << configurationString << std::endl;

   //Send pending changes to ConfigManager
   jccl::ConfigManager::instance()->addConfigurationRemovals(&delete_elements);
}

void RemoteReconfigSubjectImpl::swap(const char* addConfigurationString,
                                     const char* removeConfigurationString)
   throw(CORBA::SystemException)
{
   jccl::Configuration add_elements, delete_elements;
   std::string add_element_string(addConfigurationString);
   std::string remove_element_string(removeConfigurationString);

   //Loading from an istream
   std::istringstream add_input(add_element_string);
   std::istringstream remove_input(remove_element_string);

   add_input >> add_elements;
   remove_input >> delete_elements;

   //Send pending changes to ConfigManager
   jccl::ConfigManager::instance()->addConfigurationRemovals(&delete_elements);
   jccl::ConfigManager::instance()->addConfigurationAdditions(&add_elements);
}

char* RemoteReconfigSubjectImpl::getElements()
   throw(CORBA::SystemException)
{
   //Lock the config manager
   jccl::ConfigManager::instance()->lockActive();

   // Get the list of active config elements from config manager.
   Configuration* active_elements =
      jccl::ConfigManager::instance()->getActiveConfig();

   // Compile a string of the configuration.
   std::ostringstream active_output;

   active_output << *active_elements;

   //Unlock the list
   jccl::ConfigManager::instance()->unlockActive();

   //std::cout<< "Sending configuration: " << std::endl << active_output.str() << std::endl;

   //Return the full string
   return CORBA::string_dup(active_output.str().c_str());
}

char* RemoteReconfigSubjectImpl::getDefinitions()
   throw(CORBA::SystemException)
{
   // Get the list of definitions from the ElementFactory.
   ConfigDefinitionRepository* defs =
      jccl::ElementFactory::instance()->getConfigDefinitionRepository();

   // Compile a string of the repository.
   std::ostringstream defs_output;

   // TODO: Fix the ConfigDefinitions interface to allow vrjconfig to request
   // any definitions that it might not have.
   defs_output << defs;

   //Return the full string
   return CORBA::string_dup(defs_output.str().c_str());
}

} // namespace jccl
