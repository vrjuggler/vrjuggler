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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
#include <uuid/sysdep.h>
#include <uuid/uuid.h>

#include <vpr/vprTypes.h>


namespace vpr
{

class GUIDFactory;

class VPR_CLASS_API GUID
{
public:
   ~GUID (void) {;}

   bool operator==(const GUID& guid_obj) const;

private:
   /**
    * Generates a random GUID.
    */
   GUID(void);

   /**
    * Generates a GUID for the given namespace.
    */
//   GUID(const std::string& name);

   /**
    * Generates a GUID based on the given name that is part of the namespace
    * identified by the given namespace GUID.
    */
   GUID(const GUID& ns_guid, const std::string& name);

   GUID (const GUID& obj) {;}
   void operator= (const GUID& obj) {;}

   union _vpr_guid
   {
      uuid_t leach;

      struct _standard
      {
         vpr::Uint32 m0;
         vpr::Uint16 m1;
         vpr::Uint16 m2;
         vpr::Uint16 m3;
         vpr::Uint8  m4[6];
      } standard;

      struct _packed
      {
         vpr::Uint32 l0;
         vpr::Uint32 l1;
         vpr::Uint32 l2;
         vpr::Uint32 l3;
      } packed;
   } m_guid;

   friend class vpr::GUIDFactory;
};

} // End of vpr namespace


#endif /* _VPR_GUID_H_ */
