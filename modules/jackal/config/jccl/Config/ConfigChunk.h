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

#ifndef _JCCL_CONFIG_CHUNK_H_
#define _JCCL_CONFIG_CHUNK_H_

#include <jccl/jcclConfig.h>

#include <sstream>
#include <cppdom/cppdom.h>
#include <vpr/Util/Assert.h>

#include <jccl/Util/Debug.h>

#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/Config/ChunkDescPtr.h>

namespace jccl
{

/** A unit of configuration information.
 * References a config chunk dom node and
 * a description for the given configuration chunk type
 */
class JCCL_CLASS_API ConfigChunk
{
public:
   /** Constructor. */
   ConfigChunk();

   /** Constructs a ConfigChunk matching the given description.
    *  @param desc points to a valid ChunkDesc
    */
   ConfigChunk(ChunkDescPtr desc);

   /** Copy constructor. */
   ConfigChunk(const ConfigChunk& c);

   /** Destroys a ConfigChunk and frees associated memory.
    *  @post Self has been destroyed, along with all properties
    *        and values associated with it (but _not_ the memory
    *        associated with its ChunkDesc).
    */
   ~ConfigChunk();

   bool isValid()
   { return mValid; }

#ifdef JCCL_DEBUG
   void assertValid() const
   {
      vprASSERT(mValid == true && "Trying to use deleted config chunk");
   }
#else
   inline void assertValid() const
   {;}
#endif

   /** Initialize from cppdom::Node.
   * Constructs the chunk to reference the given chunk node
   * @post: Chunk setup and initialized.
   * @returns: false if failed to initialize
   */
   bool initFromNode(cppdom::NodePtr chunkNode);

   /** Assignment operator. */
   ConfigChunk& operator=(const ConfigChunk& c);

   /** Tests for value equality of two ConfigChunks.
    *  @return True iff self and c have the same values for all properties.
    */
   bool operator==(const ConfigChunk& c) const;

   /** Inequality */
   bool operator!=(const ConfigChunk& c) const
   {
      return !(*this == c);
   }

   /** Lexical comparison based on chunk names (alphabetically) */
   bool operator<(const ConfigChunk& c) const;

   /** Gets an embedded chunk from a property of self.
    *  @param path The complete name of an embedded chunk in self.
    *              The form is, for example,
    *              "prop_name/instance_name".
    *  @note This uses the name attribute of the elements
    *  @return A ConfigChunkPtr to the embedded chunk.  This may be
    *          A NULL ConfigChunkPtr, if the embedded chunk name was
    *          not found.  Check before use.
    */
   ConfigChunkPtr getChildChunk(const std::string &path);

   /** Writes a representation of self to out.
    *  @param out An ostream.
    *  @param self A ConfigChunk.
    */
   friend JCCL_API(std::ostream&) operator<<(std::ostream& out,
                                             const ConfigChunk& self);

   /** Returns number of values for the specified property.
    *  @param property The token of a property.
    *  @return The number of values that exist for the given property,
    *          or 0 if the property does not exist in self.
    */
   int getNum(const std::string& property) const;

   /** Returns the instance name of this ConfigChunk. */
   std::string getName() const;

   /**
    * Returns the fully qualified, unique name of this chunk.  This will be
    * different from getName() when this chunk is a child of another chunk.
    * In that case, the name will be based on the chunk hierarchy and the
    * property token.  The format will be
    * "chunk name 0/property name 0/chunk name 1/property name 1/..."
    */
   std::string getFullName() const;

   /** Returns the token string that identifies self's ChunkDesc. */
   std::string getDescToken() const;

   /** Returns a value from one of self's properties.
    *  @param property The token string for a property.
    *  @param ind The index to the property's list of values.  Use
    *             getNum() to determine the number of values for a
    *             given property.  The valid range is 0 to getNum()-1.
    * @note Call with a template parameter equal to the type to return
    *        ie. getProperty<int>("int_property", 3);
    * @todo This function could throw io exceptions.  Maybe catch those and turn them into something jccl'ish
    */
   template<class T>
   T getProperty(const std::string& prop, int ind) const
   {
      std::string prop_string = getPropertyString(prop,ind);

      T ret_val;
      std::istringstream iss(prop_string);
      iss >> ret_val;
      return ret_val;
   }

   // Specializations of getProperty<T> placed inline for Visual Studio 7.
   // MIPSpro and GCC do not handle this.  They get out-of-line
   // specializations, found below.
#ifdef _MSC_VER
   template<>
   std::string getProperty<std::string>(const std::string& prop, int ind) const
   {
      std::string prop_string = getPropertyString(prop,ind);
      return prop_string;
   }

   /**
    * Specialization for booleans so that we can read "true" and "false" rather
    * than "1" and "0".
    */
   template<>
   bool getProperty<bool>(const std::string& prop, int ind) const
   {
      std::string prop_string = getPropertyString(prop,ind);
      if ("false" == prop_string || "0" == prop_string)
      {
         return false;
      }
      else if ("true" == prop_string || "1" == prop_string)
      {
         return true;
      }
      else
      {
         vprDEBUG(jcclDBG_CONFIG, vprDBG_CONFIG_LVL)
            << "Expecting boolean string for property '" << prop
            << "' in config chunk '" << getName() << "'. Got '"
            << prop_string << " instead. Assuming value of true."
            << vprDEBUG_FLUSH;
         return true;
      }
   }

   template<>
   ConfigChunkPtr getProperty<ConfigChunkPtr>(const std::string& prop, int ind) const
   {
      return getProperty_ChunkPtr(prop,ind);
   }
#endif /* ifdef _MSC_VER */

   /** Get property that defaults to element 0
   * @note Can't use default param because GCC doesn't
   *       allow specialization in a declaration.
   *       In other words we couldn't use a default value for the property
   *       index within the std::string and ConfigChunkPtr specializations.
   */
#ifdef _MSC_VER
   template<class T>
   T getProperty(const std::string& prop) const
   {
      return getProperty<T>(prop, 0);
   }
#else
   template<class T>
   T getProperty(const std::string& prop) const;
#endif

   /** Sets a value for the given property.
    *  @param property The token string for a property.
    *  @param ind The index to the property's list of values.
    *  @param val The value to set.
    *  @note If property does not exist yet, then we create it in the chunk
    */
   template<class T>
   bool setProperty(const std::string& prop, const int ind, T val)
   {
      cppdom::NodePtr cdata_node = getPropertyCdataNode(prop, ind, true);
      vprASSERT(cdata_node.get() != NULL && "Autogrow failed");

      std::ostringstream oss;
      oss << val;
      cdata_node->setCdata(oss.str());
      return true;
   }

   /**
    * Specialization for boolean values.
    */
   bool setProperty(const std::string& prop, const int ind, bool val)
   {
      cppdom::NodePtr cdata_node = getPropertyCdataNode(prop, ind, true);
      vprASSERT(cdata_node.get() != NULL && "Autogrow failed");

      std::string str_val;

      if (true == val)
      {
         str_val = "true";
      }
      else
      {
         str_val = "false";
      }
      cdata_node->setCdata(str_val);

      return true;
   }

   /** Specialization for ConfigChunkPtrs */
   bool setProperty(const std::string& prop, int ind, ConfigChunkPtr val)
   {
      vprASSERT(false && "Not implemented");
      return true;
   }

   /** Return a list of self's depenencies.
    *  Dependencies are any ConfigChunks named by a "Chunk Pointer" property
    *  of self (or any chunk embedded in self).
    *  @return A vector of the names of all ConfigChunks referenced by self,
    *          which can be used for dependency checking.
    */
   std::vector<std::string> getChunkPtrDependencies() const;

   /** Return a list of self's embedded chunks.
    *  @return A vector of ConfigChunkPtrs to embedded ConfigChunks
    *          within self.
    */
   std::vector<jccl::ConfigChunkPtr> getEmbeddedChunks() const;

   /** Associates the description d with this Chunk
    *  @post Set the description for the chunk
    */
   void setDesc(ChunkDescPtr d);

   cppdom::NodePtr getNode()
   {
      return mNode;
   }

protected:
   /** Returns the string value of the given property
   * @param prop The token string for the property
   * @param ind  The index of the property
   * @post  If property is found, then return the contents.  Else return "".
   */
   std::string getPropertyString(const std::string& prop, int ind) const;

   /** Get the property's cdata node
   * @param prop  The property token
   * @param ind   The index inside the property
   * @param autoCreate If true, then autocreate the property node to use
   *
   * @note We always autocreate the cdata node if need be
   */
   cppdom::NodePtr getPropertyCdataNode(const std::string& prop, int ind, bool autoCreate) const;

   /** Get a chunk ptr from the given property
   * Have to call this way because specialization would use symbols that aren't available
   */
   ConfigChunkPtr getProperty_ChunkPtr(const std::string& prop, int ind) const;

protected:
   cppdom::NodePtr mNode;        /**< Node for the Config chunk element */
   ChunkDescPtr      mDesc;      /**< Description for this Chunk. */
   bool              mValid;     /**< Flag to signal wether chunk is valid. */
};

#ifndef _MSC_VER
template<>
inline std::string ConfigChunk::getProperty<std::string>(const std::string& prop, int ind) const
{
   std::string prop_string = getPropertyString(prop,ind);
   return prop_string;
}

/**
 * Specialization for booleans so that we can read "true" and "false" rather
 * than "1" and "0".
 */
template<>
inline bool ConfigChunk::getProperty<bool>(const std::string& prop, int ind) const
{
   std::string prop_string = getPropertyString(prop,ind);
   if ("false" == prop_string || "0" == prop_string)
   {
      return false;
   }
   else if ("true" == prop_string || "1" == prop_string)
   {
      return true;
   }
   else
   {
      vprDEBUG(jcclDBG_CONFIG, vprDBG_CONFIG_LVL)
         << "Expecting boolean string for property '" << prop
         << "' in config chunk '" << getName() << "'. Got '"
         << prop_string << " instead. Assuming value of true."
         << vprDEBUG_FLUSH;
      return true;
   }
}

/** Specialization for ConfigChunkPtr's
* @note Needed for embedded chunks
*/
template<>
inline ConfigChunkPtr ConfigChunk::getProperty<ConfigChunkPtr>(const std::string& prop, int ind) const
{
   return getProperty_ChunkPtr(prop,ind);
}

template<class T>
inline T ConfigChunk::getProperty(const std::string& prop) const
{
   return getProperty<T>(prop, 0);
}
#endif /* ifndef _MSC_VER */

} // namespace jccl

#endif
