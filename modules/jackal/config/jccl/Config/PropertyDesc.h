/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#ifndef _JCCL_PROPERTY_DESC_H_
#define _JCCL_PROPERTY_DESC_H_

#include <jccl/jcclConfig.h>
//#include <jccl/Config/EnumEntry.h>
#include <jccl/Config/ConfigTokens.h>
#include <jccl/Config/ParseUtil.h>

#include <cppdom/cppdom.h>

namespace jccl
{

/** Describes properties
*
* Holds all the information describing a property
*/
class PropertyDesc
{
public:

   /** Constructor
   * Initialized with default values
   */
   PropertyDesc ();

   /** Constructor from XML node*/
   PropertyDesc(cppdom::XMLNodePtr node);

   /** Copy Constructor */
   PropertyDesc (const PropertyDesc& d);

   /**
    * Convenience constructor.
    * @post name = token = n, help = h, num = i, type = t,
    *       valuelabels & enumerations are empty.
    */
   /*
   PropertyDesc (const std::string& n, int i, VarType t, const std::string& h);
   */

   /** Destroys a PropertyDesc, and frees all allocated memory. */
   ~PropertyDesc ();

#ifdef JCCL_DEBUG
   void assertValid () const;
#else
   inline void assertValid () const
   {
      ;
   }
#endif

   /** Returns the token string for the described property. */
   inline const std::string getToken () const
   {
      return mNode->getAttribute(jccl::token_TOKEN).getValue<std::string>();
   }

   inline void setToken (const std::string& tok)
   {
      mNode->setAttribute(jccl::token_TOKEN, tok);
   }

   inline const std::string getName () const
   {
      return mNode->getAttribute(jccl::name_TOKEN).getString();
   }

   inline void setName (const std::string& name)
   {
      mNode->setAttribute(jccl::name_TOKEN, name);
   }

   std::string getHelp() const;

   void setHelp(const std::string& help);

   inline VarType getVarType () const
   {
      return jccl::stringToVarType( mNode->getAttribute(jccl::type_TOKEN) );
   }

   inline void setVarType (VarType _type)
   {
      mNode->setAttribute(jccl::type_TOKEN, jccl::typeString(_type) );
   }

   inline int getNumAllowed () const
   {
      return mNode->getAttribute(jccl::num_TOKEN).getValue<int>();
   }

   inline void setNumAllowed (int num)
   {
      mNode->setAttribute(jccl::num_TOKEN, num );
   }

   /**
    * Retrieves the default value (as an XML string) for the i'th item in this
    * property description.
    */
   cppdom::XMLString getDefaultValueString(int index);

   /*
   //: Returns the number of individual value labels
   inline int getValueLabelsSize () const
   {
      return valuelabels.size();
   }

   void appendValueLabel (const std::string& _label);

   //: Returns the ith value label
   const std::string& getValueLabel (unsigned int index) const;
   */

   /** Writes a PropertyDesc to the given ostream
   */
   friend std::ostream& operator << (std::ostream& out, const PropertyDesc& self);

   /** Equality operator. */
   // BUG (IPTHACK) - doesn't check equality of enumerations and valuelabels
   bool operator== (const PropertyDesc& pd) const;

   /** Inequality operator. */
   inline bool operator!= (const PropertyDesc& pd) const
   {
      return !(*this == pd);
   }

   /** Get the XML node pointer.
   * Users should not use this directly.
   */
   cppdom::XMLNodePtr getNode()
   {
      return mNode;
   }

protected:
   bool                 mIsValid;   /**< Validation flag */
   cppdom::XMLNodePtr    mNode;      /**< The xml node for this chunk desc */
};

} // End of jccl namespace


#endif
