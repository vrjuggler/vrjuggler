/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_TYPES_H_
#define _VPR_TYPES_H_

#include <vpr/vprConfig.h>

#ifdef VPR_USE_BOOST
#  include <vpr/md/BOOST/vprTypesBOOST.h>
#else
namespace vpr
{
#if SIZEOF_CHAR == 1
   typedef signed   char Int8;
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
namespace vpr
{

/** \struct Uint64Hash vprTypes.h vpr/vprTypes.h
 *
 * Nice little helper class for hashing a vpr::Uint64.
 */
struct Uint64Hash
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

} // End of vpr namespace

// XXX: The following is a hack to deal with GCC 3.0.1 on IRIX not knowing how
// to deal with an unsigned long long being passed to operator<<.  This code
// should be removed as soon as possible!
#if defined(VPR_OS_IRIX) && defined(__GNUC__) &&                        \
    __GNUC__ == 3 && __GNUC_MINOR__ == 0 && __GNUC_PATCHLEVEL__ <= 1

#include <iostream>

namespace std
{

inline std::ostream& operator<< (std::ostream& out, const vpr::Uint64& v)
{
   out << (vpr::Uint32) v;
   return out;
}

}
#endif /* defined(VPR_OS_IRIX) && defined(__GNUC__) */


#endif   /* _VPR_TYPES_H_ */
