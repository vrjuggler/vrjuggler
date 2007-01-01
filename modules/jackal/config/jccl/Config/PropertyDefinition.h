/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _JCCL_PROPERTY_DEFINITION_H_
#define _JCCL_PROPERTY_DEFINITION_H_

#include <jccl/jcclConfig.h>
#include <cppdom/cppdom.h>

#include <jccl/Config/ConfigTokens.h>

namespace jccl
{

/** \class PropertyDefinition PropertyDefinition.h jccl/Config/PropertyDefinition.h
 *
 * Describes properties.  Holds all the information describing a property.
 * Instances of this class are read-only objects.
 *
 * Properties are composed to make up a config element.  Each property has
 * zero or more values depending on how it is defined.  The basic type of all
 * the values for a given property will be the same.
 *
 * @see VarType
 */
class JCCL_CLASS_API PropertyDefinition
{
public:

   /**
    * Constructor.  This property definition is initialized with default
    * values.
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

   /**
    * Returns the token (a valid XML element identifier) of this property.
    */
   std::string getToken() const;

   /** Returns the human-readable name of this property. */
   std::string getName() const;

   /** Returns the help text for this property. */
   std::string getHelp() const;

   /**
    * Returns an identifier that indicates the type of the values of this
    * property.
    */
   VarType getVarType() const;

   /**
    * Returns the number of values allowed for this property.
    */
   int getNumAllowed() const;

   /**
    * Retrieves the default value (as an XML string) for the i'th item in this
    * property definition.
    */
   std::string getDefaultValueString(int index);

   /** Writes a PropertyDefinition to the given ostream. */
   friend JCCL_API(std::ostream&) operator<< (std::ostream& out,
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
