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

#ifndef _JCCL_ELEMENT_FACTORY_H_
#define _JCCL_ELEMENT_FACTORY_H_

#include <jccl/jcclConfig.h>

#include <cppdom/cppdom.h>
#include <vpr/Util/Singleton.h>

#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ConfigDefinition.h>
#include <jccl/Config/ConfigDefinitionRepository.h>


namespace jccl
{

   /** \class ElementFactory ElementFactory.h jccl/Config/ElementFactory.h
    *
    * Factory for elements and a place holder for some other system-wide
    * factory functions.
    */
   class JCCL_CLASS_API ElementFactory
   {
   public:
      /**
       * Adds definitions from definition files found in the given search path.
       * The given path is treated as the default search path.  If the
       * environment variable \c JCCL_DEFINITION_PATH is set, the value of
       * that variable is appended to the default search path.
       *
       * @param defaultPath The default search path to use for finding .jdef
       *                    files.
       */
      void loadDefs(const std::string& defaultPath);
      

      /** Adds the definition in file 'filename' to the factory. */
      bool loadDef(const std::string& filename,
                   const std::string& parentFile = "");

      // we actually do need this so that the EM can send the definitions to
      // the gui...
      // XXX: Then isn't this a big hack?  [-PH 6/15/2003]
      ConfigDefinitionRepository* getConfigDefinitionRepository()
      {
         return &mRepository;
      }

      /** Returns the newest version of the named configuration definition. */
      ConfigDefinitionPtr getConfigDefinition(const std::string& token);

      /** Creates an Element using the named definition. */
      ConfigElementPtr createElement(const std::string& defToken);

      /** Creates an element using the given definition. */
      ConfigElementPtr createElement(ConfigDefinitionPtr d);

      /** Get the global XML context that we are using system-wide. */
      cppdom::ContextPtr getXMLContext();

      /** Creates a new (empty) XML node using global context. */
      cppdom::NodePtr createXMLNode();

      /** Creates a new (empty) XML document using global context. */
      cppdom::DocumentPtr createXMLDocument();

   private:
      ElementFactory();

      ConfigDefinitionRepository mRepository;
      cppdom::ContextPtr         mGlobalContext; /**< The global context to use for jccl */

      vprSingletonHeader(ElementFactory);
   };

} // namespace jccl

#endif
