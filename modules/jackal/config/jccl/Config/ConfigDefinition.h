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

/**
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

#ifdef JCCL_DEBUG
   void assertValid() const;
#else
   inline void assertValid() const
   {;}
#endif

   /** Equality operator. */
   bool operator== (const ConfigDefinition& d) const;

   inline bool operator!= (const ConfigDefinition& d) const
   {
      return !(*this == d);
   }

   /** Returns name.
    * @return "" if not set.
    */
   std::string getName() const;
   
   /** Returns token.
    * @return "" if not set.
    */
   std::string getToken() const;

   /** Gets the version of this definition.
    * @return 0 if not set.
    */
   unsigned int getVersion() const;
   
   /** Returns help text.
    * @return "" if not set.
    */
   std::string getHelp() const;

   /** Gets a PropertyDefinitoin from self with matching token.
    * @param   token Non-NULL token for the desired jccl::PropertyDefinition.
    * @returns pdesc Pointer to propertydesc in self with matching token.
    */
   PropertyDefinition getPropertyDefinition(const std::string& token) const;

   /** Get all the PropertyDefinitions from the ConfigDefinition */
   std::vector<PropertyDefinition> getAllPropertyDefinitions() const;

   /** Set the node.  Do any specialized processing necessary. */
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
