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

#ifndef _VPR_GUID_FACTORY_H_
#define _VPR_GUID_FACTORY_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/Util/GUID.h>


namespace vpr
{

class VPR_CLASS_API GUIDFactory
{
public:
   ~GUIDFactory (void) {;}

   static vpr::GUID* createRandomGUID (void)
   {
      return new vpr::GUID(true);
   }

   static vpr::GUID* createGUID (const struct vpr::GUID::StdGUID& guid)
   {
      return new vpr::GUID(guid);
   }

   static vpr::GUID* createGUID (const char* name)
   {
      return createGUID(std::string(name));
   }

   static vpr::GUID* createGUID (const std::string& name)
   {
      return new vpr::GUID(name);
   }

   static vpr::GUID* createNameGUID (const vpr::GUID& namespace_guid,
                                     const std::string& name)
   {
      return new vpr::GUID(namespace_guid, name);
   }

protected:
   GUIDFactory (void) {;}
   GUIDFactory (const GUIDFactory& o) {;}
   void operator= (const GUIDFactory& o) {;}
};

} // End of vpr namespace


#endif /* _VPR_GUID_FACTORY_H_ */
