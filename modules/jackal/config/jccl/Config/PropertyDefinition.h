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

#ifndef _JCCL_PROPERTY_DEFINITION_H_
#define _JCCL_PROPERTY_DEFINITION_H_

#include <jccl/jcclConfig.h>
#include <cppdom/cppdom.h>

#include <jccl/Config/ConfigTokens.h>

namespace jccl
{

/**
 * Describes properties.  Holds all the information describing a property.
 * Instances of this class are read-only objects.
 */
class PropertyDefinition
{
public:

   /** Constructor.
    * Initialized with default values.
    */
   PropertyDefinition();

   /** Constructor from XML node. */
   PropertyDefinition(cppdom::NodePtr node);

   /** Copy Constructor. */
   PropertyDefinition(const PropertyDefinition& d);

   /**
    * Convenience constructor.
    * @post name = token = n, help = h, num = i, type = t,
    *       valuelabels & enumerations are empty.
    */
   /*
   PropertyDefinition(const std::string& n, int i, VarType t, const std::string& h);
   */

   /** Destroys a PropertyDefinition, and frees all allocated memory. */
   ~PropertyDefinition();

#ifdef JCCL_DEBUG
   void assertValid() const;
#else
   void assertValid() const
   {}
#endif

   /** Returns the token string for the described property. */
   std::string getToken() const;

   std::string getName() const;

   std::string getHelp() const;

   VarType getVarType() const;

   int getNumAllowed() const;

   /**
    * Retrieves the default value (as an XML string) for the i'th item in this
    * property definition.
    */
   std::string getDefaultValueString(int index);

   /** Writes a PropertyDefinition to the given ostream. */
   friend std::ostream& operator<< (std::ostream& out,
                                    const PropertyDefinition& self);

   /** Equality operator. */
   // BUG (IPTHACK) - doesn't check equality of enumerations and valuelabels
   bool operator== (const PropertyDefinition& pd) const;

   /** Inequality operator. */
   bool operator!= (const PropertyDefinition& pd) const;

   /** Get the XML node pointer.
    * Users should not use this directly.
    */
   cppdom::NodePtr getNode();
protected:
   bool              mIsValid; /**< Validation flag */
   cppdom::NodePtr   mNode;    /**< The XML node for this property definition */
};

} // End of jccl namespace


#endif
