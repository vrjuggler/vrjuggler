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

#ifndef _JCCL_CHUNKDESC_H_
#define _JCCL_CHUNKDESC_H_

#include <jccl/jcclConfig.h>
#include <jccl/Config/PropertyDesc.h>
#include <jccl/Config/ChunkDescPtr.h>
#include <jccl/Config/ConfigChunkPtr.h>

#include <cppdom/cppdom.h>

namespace jccl
{

/** Description of a single chunk "type"
* 
* @note: We do not maintain an explicit list of property descs,
*        but instead we just create them dynamically from the 
*        children of our ChunkDesc XML node.
*/
class JCCL_CLASS_API ChunkDesc
{
public:
   /** Constructor */
   ChunkDesc ();

   /** Construct a chunk desc using the node given */
   ChunkDesc(cppdom::NodePtr node);

   /** Copy Constructor */
   ChunkDesc (const ChunkDesc& desc);

   /** Destructor */
   ~ChunkDesc ();

#ifdef JCCL_DEBUG
   void assertValid () const;
#else
   inline void assertValid () const
   {;}
#endif

   /** equality operator */
   bool operator== (const ChunkDesc& d) const;

   inline bool operator!= (const ChunkDesc& d) const
   {
      return !(*this == d);
   }

   /** Sets the user-friendly name of self */
   void setName (const std::string& name);

   /** Sets the token identifier of self */
   void setToken (const std::string& token);

   /** Sets the help string for self */
   void setHelp (const std::string& help);

   /** Return name
   * @return "" if not set
   */
   std::string getName() const;
   
   /** Return token
   * @return "" if not set
   */
   std::string getToken() const;
   
   /** Return help text
   * @return "" if not set
   */
   std::string getHelp() const;

   /*
   void setDefaultChunk (DOM_Node* n);
   ConfigChunkPtr getDefaultChunk() const;
   void setDefaultChunk (ConfigChunkPtr ch);
   */

   /** Adds a PropertyDesc to self.
   * NOTE: Any PropertyDesc previously in self with the
   *      same token as pd is removed.
   */
   void add (PropertyDesc pd);

   /** Removes named PropertyDesc from self.
   * @param _token - token to search for
   * @returns true - a PropertyDesc with that token was found
   * @returns false - no such PropertyDesc was found.
   */
   bool remove (const std::string& _token);

   /** Gets a PropertyDesc from self with matching token
   * @param   _token - non-NULL token for the desired PropertyDesc
   * @returns pdesc - Pointer to propertydesc in self with matching token
   */
   PropertyDesc getPropertyDesc (const std::string& _token) const;

   /** Get all the PropertyDescs from the ChunkDesc */
   std::vector<PropertyDesc> getAllPropertyDesc() const;

   /** Set the node.
   * Do any specialized processing necessary
   */
   void setNode(cppdom::NodePtr node)
   { mNode = node; }
   cppdom::NodePtr getNode()
   { return mNode; }

   /** Writes self to the given output stream */
   friend JCCL_API(std::ostream&) operator << (std::ostream& out,
                                               const ChunkDesc& self);

protected:
   bool               mIsValid;   /**< Validation flag */
   cppdom::NodePtr  mNode;      /**< The xml node for this chunk desc */
};

} // End of jccl namespace

#endif

