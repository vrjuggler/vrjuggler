/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_GUID_H_
#define _VPR_GUID_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/vprTypes.h>
#include <boost/concept_check.hpp>  // for ignore_unused_variable_warning

#include <vpr/IO/SerializableObject.h>    // For serializing GUID


namespace vpr
{

class GUIDFactory;

class VPR_CLASS_API GUID : public vpr::SerializableObject
{
public:
   /** Tag to the constructor to force generation: dso::GUID guid(dso::GUID::generate_tag);
*/
   class GenerateTag { };
   static GenerateTag generateTag;

public:
   virtual ~GUID (void) {;}

   GUID(const GenerateTag tag)
   {
      boost::ignore_unused_variable_warning(tag);
      generate();
   }

   /**
    * Converts this GUID to its corresponding string representation.
    */
   std::string toString(void) const;

   bool operator==(const GUID& guid_obj) const;

   bool operator!= (const GUID& guid_obj) const
   {
      return ! (*this == guid_obj);
   }

   bool operator<(const GUID& r) const
   {
      bool ret_val(false);

      if (mGuid.packed.l0 < r.mGuid.packed.l0)
      {  ret_val = true; }
      else if(mGuid.packed.l0 == r.mGuid.packed.l0)
      {
         if(mGuid.packed.l1 < r.mGuid.packed.l1)
         {  ret_val = true; }
         else if(mGuid.packed.l1 == r.mGuid.packed.l1)
         {
            if(mGuid.packed.l2 < r.mGuid.packed.l2)
            {  ret_val = true; }
            else if(mGuid.packed.l2 == r.mGuid.packed.l2)
            {
               if(mGuid.packed.l3 < r.mGuid.packed.l3)
               {
                  ret_val = true;
               }
            }
         }
      }

      return ret_val;
   }

   struct StdGUID
   {
      vpr::Uint32 m0;
      vpr::Uint16 m1;
      vpr::Uint16 m2;
      vpr::Uint8  m3;
      vpr::Uint8  m4;
      vpr::Uint8  m5[6];
   };

public:
   /**
    * Generates empty guid - Sets equal to GUID::NullGUID
    */
   GUID();

   /**
    * Generates a GUID from the given struct.
    */
   GUID(const struct StdGUID& guid);

   /**
    * Generates a GUID from the given string representation of the GUID using
    * a char*.
    * Format: "8x-4x-4x-2x2x-2x2x2x2x2x2x"
    *
    * @param guid_string Ptr to a string that is used to inialize guid. Must
    *                    be non-NULL.
    */
   GUID(const char* guid_string);

   /**
    * Generates a GUID from the given string representation of the GUID using
    * a std::string.
    * Format: "8x-4x-4x-2x2x-2x2x2x2x2x2x"
    *
    * @param guid_string Ptr to a string that is used to inialize guid. Must
    *                    be non-NULL.
    */
   GUID (const std::string& guid_string)
   {
      fromString(guid_string);
   }

   /**
    * Generates a GUID based on the given name that is part of the namespace
    * identified by the given namespace GUID.
    */
   GUID(const GUID& ns_guid, const std::string& name);

   /** Copy constructor. */
   GUID(const GUID& obj) : vpr::SerializableObject(obj)
   {
      mGuid.packed.l0 = obj.mGuid.packed.l0;
      mGuid.packed.l1 = obj.mGuid.packed.l1;
      mGuid.packed.l2 = obj.mGuid.packed.l2;
      mGuid.packed.l3 = obj.mGuid.packed.l3;
   }

   GUID& operator= (const GUID& obj)
   {
      if(&obj == this) // Check for self
         return *this;

      mGuid.packed.l0 = obj.mGuid.packed.l0;
      mGuid.packed.l1 = obj.mGuid.packed.l1;
      mGuid.packed.l2 = obj.mGuid.packed.l2;
      mGuid.packed.l3 = obj.mGuid.packed.l3;
      return *this;
   }

   void generate();
   void generate(const GUID& ns_guid, const std::string& name);

   /** @name Reader/Writer methods */
   //@{
   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer)
   {
      writer->writeUint32(mGuid.standard.m0);
      writer->writeUint16(mGuid.standard.m1);
      writer->writeUint16(mGuid.standard.m2);
      writer->writeUint8(mGuid.standard.m3);
      writer->writeUint8(mGuid.standard.m4);
      writer->writeUint8(mGuid.standard.m5[0]);
      writer->writeUint8(mGuid.standard.m5[1]);
      writer->writeUint8(mGuid.standard.m5[2]);
      writer->writeUint8(mGuid.standard.m5[3]);
      writer->writeUint8(mGuid.standard.m5[4]);
      writer->writeUint8(mGuid.standard.m5[5]);
      return vpr::ReturnStatus::Succeed;
   }

   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader)
   {
      mGuid.standard.m0 = reader->readUint32();
      mGuid.standard.m1 = reader->readUint16();
      mGuid.standard.m2 = reader->readUint16();
      mGuid.standard.m3 = reader->readUint8();
      mGuid.standard.m4 = reader->readUint8();
      mGuid.standard.m5[0] = reader->readUint8();
      mGuid.standard.m5[1] = reader->readUint8();
      mGuid.standard.m5[2] = reader->readUint8();
      mGuid.standard.m5[3] = reader->readUint8();
      mGuid.standard.m5[4] = reader->readUint8();
      mGuid.standard.m5[5] = reader->readUint8();
      return vpr::ReturnStatus::Succeed;
   }
   //@}

   /**
    * Multi-format GUID/UUID container.  This will always be 128 bits, but
    * its memory may be accessed differently depending on the needs of the
    * code.
    */
   union _vpr_guid
   {
      /** Mozilla UUID format. */
      struct _moz
      {
         vpr::Uint32 m0;
         vpr::Uint16 m1;
         vpr::Uint16 m2;
         vpr::Uint8 m3[8];
      } moz;

      /** Standard GUID memory layout. */
      struct StdGUID standard;

      /**
       * Friendly packed UUID format.
       *
       * @note This format cannot be used for network transmission of GUIDs
       *       due to byte ordering issues.
       */
      struct _packed
      {
         vpr::Uint32 l0;
         vpr::Uint32 l1;
         vpr::Uint32 l2;
         vpr::Uint32 l3;
      } packed;
   } mGuid;

   friend class vpr::GUIDFactory;

   static const vpr::GUID NullGUID;

   /**
    * Hasher for vpr::GUID.  This can be used with std::hash_map and
    * friends.
    */
   struct hash
   {
      vpr::Uint32 operator() (const vpr::GUID& guid) const
      {
         return guid.mGuid.packed.l0 + guid.mGuid.packed.l1 + guid.mGuid.packed.l2 + guid.mGuid.packed.l3;
      }
   };

private:

   /**
    * Performs the real work of generating a GUID from a string.
    * Format: "8x-4x-4x-2x2x-2x2x2x2x2x2x"
    */
   void fromString(const std::string& guid_string);
};

} // End of vpr namespace

namespace std
{

inline std::ostream& operator<<(std::ostream& out, const vpr::GUID& guid)
{
   out << guid.toString();
   return out;
}

}


#endif /* _VPR_GUID_H_ */
