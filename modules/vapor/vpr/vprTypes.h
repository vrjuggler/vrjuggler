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

#ifndef _VPR_TYPES_H_
#define _VPR_TYPES_H_


#ifdef VPR_USE_NSPR
#include <vpr/md/NSPR/vprTypesNSPR.h>
#else
namespace vpr {
#if SIZEOF_CHAR == 1
    typedef char Int8;
    typedef unsigned char Uint8;
#endif

#if SIZEOF_SHORT == 2
    typedef short Int16;
    typedef unsigned short Uint16;
#elif SIZEOF_INT == 2
    typedef int Int16;
    typedef unsigned int Uint16;
#endif
    
#if SIZEOF_INT == 4
    typedef int Int32;
    typedef unsigned int Uint32;
#elif SIZEOF_LONG == 4
    typedef long Int32;
    typedef unsigned long Uint32;
#endif

#if SIZEOF_LONG == 8
    typedef long Int64;
    typedef unsigned long Uint64;
#elif SIZEOF_LONG_LONG == 8
    typedef long long Int64;
    typedef unsigned long long Uint64;
#elif SIZEOF_INT == 8
    typedef int Int64;
    typedef unsigned int Uint64;
#endif
};
#endif



// --- HASH Functions ---- //
#if defined(HAVE_HASH_MAP) || defined(HAVE_HASH_MAP_H)

#ifdef HAVE_HASH_MAP
#   include <hash_map>
#else
#   include <hash_map.h>
#endif

namespace std
{
/// Nice little helper class for hashing a vpr::Uint64.
template<>
struct hash<vpr::Uint64>
{
   union Uint64_val
   {
      vpr::Uint64 full;
      struct
      {
         vpr::Uint32 low32;
         vpr::Uint32 high32;
      } nested;
   };

   vpr::Uint32 operator() (vpr::Uint64 val) const
   {
      Uint64_val temp;
      temp.full = val;
      return temp.nested.low32 + temp.nested.high32;      
   }
};

} // End of std namespace

#endif



#endif	/* _VPR_TYPES_H_ */
