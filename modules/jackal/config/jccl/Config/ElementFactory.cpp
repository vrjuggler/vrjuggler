/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <jccl/jcclConfig.h>
#include <jccl/Util/Debug.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/Config/ConfigDefinitionReader.h>
#include <jccl/Config/ParseUtil.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

namespace fs = boost::filesystem;

namespace jccl
{

   ElementFactory::ElementFactory()
   {
      // Create global context
      mGlobalContext = cppdom::ContextPtr(new cppdom::Context);
   }

   void ElementFactory::loadDefs(const std::string& path)
   {
      // Split the path on the native separator character
      std::vector<std::string> search_path;
      std::string::size_type sep_index = 0;
      while (sep_index != std::string::npos)
      {
         // This will not work on windows since the path seperator is ";" not ":"
         std::string::size_type next_sep_index = path.find_first_of(":", sep_index);

         std::string dir_str;
         
         if(next_sep_index == std::string::npos)
         {
            dir_str = path.substr(sep_index, path.size() - sep_index);
         }
         else
         {
            dir_str = path.substr(sep_index, next_sep_index - sep_index);
         }

         vprDEBUG(jcclDBG_CONFIG, vprDBG_CONFIG_LVL)
            << "Adding directory to search path: " << dir_str << "\n"
            << vprDEBUG_FLUSH;
         search_path.push_back(dir_str);

         if (next_sep_index == std::string::npos)
         {
            break;
         }
         sep_index = next_sep_index + 1;
      }

      // Search each directory in the path for definition files
      for (std::vector<std::string>::const_iterator itr = search_path.begin();
           itr != search_path.end();
           ++itr)
      {
         const std::string dir_str = demangleFileName(*itr, "");
         try
         {
            fs::path dir(dir_str, boost::filesystem::native);

            if (fs::exists(dir) && fs::is_directory(dir))
            {
               fs::directory_iterator end_itr;
               for (fs::directory_iterator file(dir); file != end_itr; ++file)
               {
                  try
                  {
                     // Ignore directories
                     if (!fs::is_directory(*file))
                     {
                        // Only accept files with a .jdef extension
                        const std::string def_ext = ".jdef";
                        const std::string file_name = file->leaf();
                        
                        std::string::size_type result = file_name.rfind(".");
                        if(result != std::string::npos && file_name.substr(result) == def_ext)
                        {
                           vprDEBUG(jcclDBG_CONFIG, vprDBG_VERB_LVL)
                              << "Loading '" << file->native_file_string()
                              << "'\n" << vprDEBUG_FLUSH;
                           loadDef(file->native_file_string());
                        }

                        
                        //const std::string::size_type pos = file->leaf().size() - def_ext.size();
                        //const std::string file_ext = file->leaf().substr(pos);
                        //if (file_ext == def_ext)
                        //{
                        //   vprDEBUG(jcclDBG_CONFIG, vprDBG_VERB_LVL)
                        //      << "Loading '" << file->native_file_string()
                        //      << "'\n" << vprDEBUG_FLUSH;
                        //   loadDef(file->native_file_string());
                        //}
                     }
                  }
                  catch (fs::filesystem_error& err)
                  {
                     vprDEBUG(jcclDBG_CONFIG, vprDBG_WARNING_LVL)
                        << "Failed to read '" << file->native_file_string()
                        << "': " << err.what() << std::endl << vprDEBUG_FLUSH;
                  }
               }
            }
         }
         catch (fs::filesystem_error& err)
         {
            vprDEBUG(jcclDBG_CONFIG, vprDBG_WARNING_LVL)
               << "Failed to load definitions in directory '"
               << (*itr) << "': " << err.what() << "\n"
               << vprDEBUG_FLUSH;
         }
      }
   }

   // Adds definitions in file 'file_name' to the factory.
   bool ElementFactory::loadDef(const std::string& file_name,
                                const std::string& parentFile)
   {

      // Attempt to read all the definitions from the file
      try
      {
         const std::string abs_filename = demangleFileName(file_name, parentFile);
         ConfigDefinitionReader reader;
         std::vector<ConfigDefinitionPtr> defs = reader.read(abs_filename);

         // Add the definitions to the repository
         for (std::vector<ConfigDefinitionPtr>::const_iterator itr = defs.begin();
              itr != defs.end();
              ++itr)
         {
            ConfigDefinitionPtr cfg_def = (*itr);
            mRepository.add(cfg_def);
         }
      }
      catch (std::runtime_error& e)
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "Failed to load definition file: '" << file_name.c_str()
            << ": " << e.what() << "'.\n" << vprDEBUG_FLUSH;
         return false;
      }

      vprDEBUG(jcclDBG_CONFIG,vprDBG_CONFIG_LVL)
         << "Loaded definition file: '" << file_name.c_str() << "'.\n"
         << vprDEBUG_FLUSH;

      return true;
   }

   ConfigDefinitionPtr ElementFactory::getConfigDefinition(const std::string& token)
   {
      return mRepository.get(token);
   }

   // Creates an Element using the named definition.
   ConfigElementPtr ElementFactory::createElement(const std::string& defToken)
   {
      return createElement(mRepository.get(defToken));
   }

   // Creates an element using the given config definition.
   ConfigElementPtr ElementFactory::createElement(ConfigDefinitionPtr d)
   {
      if ( d.get() != NULL )
      {
         d->assertValid();
         return ConfigElementPtr(new ConfigElement(d));
      }
      else
      {
         return ConfigElementPtr();
      }
   }

   // Get the global XML context that we are using system-wide.
   cppdom::ContextPtr ElementFactory::getXMLContext()
   {
      vprASSERT(mGlobalContext.get() != NULL);
      return mGlobalContext;
   }

   // Creates a new (empty) XML node using global context.
   cppdom::NodePtr ElementFactory::createXMLNode()
   {
      return cppdom::NodePtr(new cppdom::Node(getXMLContext()));
   }

   // Creates a new (empty) XML document using global context.
   cppdom::DocumentPtr ElementFactory::createXMLDocument()
   {
      return cppdom::DocumentPtr(new cppdom::Document(getXMLContext()));
   }

   vprSingletonImp(ElementFactory);

} // namespace jccl
