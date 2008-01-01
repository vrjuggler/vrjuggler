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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _VPR_TS_OBJECT_H_
#define _VPR_TS_OBJECT_H_

#include <vpr/vprConfig.h>


namespace vpr
{

/** \class TSBaseObject TSObject.h vpr/Thread/TSObject.h
 *
 * Base thread-specific object.
 *
 * Used so that we can have an array of heterogenous TS objects.  (We get some
 * type safety.)  This also defines some members that all TS Objects need.
 */
class VPR_CLASS_API TSBaseObject
{
public:
   virtual ~TSBaseObject();
};

/** \class TSObject TSObject.h vpr/Thread/TSObject.h
 *
 * Wrapper template for storing thread-specific objects.  Used so we can stay
 * type safe with a dynamic cast up from the base object to this type wrapper.
 *
 * @see vpr::TSTable, vpr::TSObjectProxy
 */
template<class T>
class TSObject : public TSBaseObject
{
public:
   virtual ~TSObject()
   {;}

   /** Returns the address of our object. */
   T* getObject()
   {
      return &mLocalObj;
   }

private:
   T mLocalObj;
};

} // End of vpr namespace


#endif
