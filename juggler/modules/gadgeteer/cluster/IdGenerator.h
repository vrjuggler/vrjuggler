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

#ifndef _CLUSTER_ID_GENERATOR_H
#define _CLUSTER_ID_GENERATOR_H

#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>
#include <list>
#include <algorithm>


namespace cluster
{

template <class T> class IdGenerator
{
private:
   T mMAXID;
   T mLargestActiveId;
   std::list<T> mReleasedIds;
public:
   IdGenerator(T max_id) : mMAXID(max_id)
   {
      mLargestActiveId = 0; // 0 is not used
   }

   IdGenerator() : mMAXID(255)
   {
      mLargestActiveId = 0; // 0 is not used
   }

   T generateNewId()
   {
      if ( mReleasedIds.size() > 0 )
      {
         T return_value = mReleasedIds.back();   // get a previously released id
         mReleasedIds.pop_back();             // remote it from list
         return return_value;                            // and return it
      }
      else if ( mLargestActiveId + 1 < mMAXID )
      {
         return ++mLargestActiveId;      // use the next free id
      }
      else
      {
         return 0;
      }
   }

   void releaseId(T id)
   {
      if ( mLargestActiveId == id )
      {
         mLargestActiveId--;
      }
      else
      {
         typename std::list<T>::iterator result = std::find(mReleasedIds.begin(), mReleasedIds.end(), id);

         if ( result != mReleasedIds.end() )
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[IdGenerator] ")
               << "Id has already been released!\n" << vprDEBUG_FLUSH;                        
         }
         mReleasedIds.push_back(id);        // remove it from list
      }
   }

   // following function should only be used before ids are generated.
   void setMaxId(T max_id)
   {
      mMAXID = max_id;
   }
};


} // end namespace gadget

#endif
