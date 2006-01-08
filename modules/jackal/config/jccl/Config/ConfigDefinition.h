/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _JCCL_CONFIG_DEFINITION_H_
#define _JCCL_CONFIG_DEFINITION_H_

#include <jccl/jcclConfig.h>
#include <vector>
#include <cppdom/cppdom.h>
#include <jccl/Config/PropertyDefinition.h>
#include <jccl/Config/ConfigDefinitionPtr.h>
#include <jccl/Config/ConfigElementPtr.h>

namespace jccl
{

/** \class ConfigDefinition ConfigDefinition.h jccl/Config/ConfigDefinition.h
 *
 * Definition of a single configuration element "type".
 * 
 * @note We do not maintain an explicit list of property definitions,
 *       but instead we just create them dynamically from the 
 *       children of our ConfigDefinition XML node.
 */
class JCCL_CLASS_API ConfigDefinition
{
public:
   /** Constructor. */
   ConfigDefinition();

   /** Construct a configuration definition using the node given. */
   ConfigDefinition(const std::string& token, cppdom::NodePtr node);

   /** Copy constructor. */
   ConfigDefinition(const ConfigDefinition& desc);

   /** Destructor. */
   ~ConfigDefinition();

   /** Equality operator. */
   bool operator== (const ConfigDefinition& d) const;

   inline bool operator!= (const ConfigDefinition& d) const
   {
      return !(*this == d);
   }

   /** Returns the human-readable name of this definition. */
   std::string getName() const;
   
   /**
    * Returns the token (a valid XML element identifier) of this definition.
    * This corresponds to the ID of a config element.
    */
   std::string getToken() const;

   /**
    * Gets the version of this definition.
    * @return 0 if not set.
    */
   unsigned int getVersion() const;

   /** Returns the help text for this definition. */
   std::string getHelp() const;

   /**
    * Tests to see if we derive from the named ConfigDefinition.  This is used
    * in the context of config definition inheritance.
    *
    * @param token A token for the ConfigDefinition that may be our parent.
    *
    * @return true is returned if the named ConfigDefinition type is a parent
    *         of this definition.  false is returned otherwise.
    */
   bool isParent(const std::string& token) const;
   
   /**
    * Gets a PropertyDefinition object from self with matching token.
    *
    * @param token Non-NULL token for the desired jccl::PropertyDefinition.
    *
    * @returns A copy of a PropertyDefinition object in self whose token
    *          matches the given value.
    */
   PropertyDefinition getPropertyDefinition(const std::string& token) const;

   /** Gets all the PropertyDefinition objects from self. */
   std::vector<PropertyDefinition> getAllPropertyDefinitions() const;

   /** Sets the node.  Do any specialized processing necessary. */
   void setNode(cppdom::NodePtr node)
   {
      mNode = node;
   }

   cppdom::NodePtr getNode()
   {
      return mNode;
   }

   /** Writes self to the given output stream. */
   friend JCCL_API(std::ostream&) operator<< (std::ostream& out,
                                              const ConfigDefinition& self);

protected:
   bool             mIsValid;   /**< Validation flag */
   std::string      mToken;     /**< The token for this definition. */
   cppdom::NodePtr  mNode;      /**< The XML node for this definition. */
};

} // End of jccl namespace


#endif
