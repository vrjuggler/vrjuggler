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

#ifndef _VPR_TS_TABLE_H_
#define _VPR_TS_TABLE_H_
//#pragma once

#include <vpr/vprConfig.h>
#include <vector>
#include <vpr/Thread/TSObject.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Assert.h>


namespace vpr {

/**
 * This class is the actual TS Table.
 *
 * This class maintains a table that has ptrs to all the TS data
 * in the system for a specific thread.
 * Only the owning thread may actually access the table
 *
 * @author Allen Bierbaum
 */
class VPR_CLASS_API TSTable
{
public:
   TSTable()
   {;}

   /**
    * Deletes the table. It deletes all objects in the table.
    */
   ~TSTable();

public:
   /**
    * Returns true if the table contains the given key.
    * If false, then the user should setObject(...,key) before
    * attempting to access the object.
    */
   bool containsKey(long key)
   {
      vprASSERT((key >= 0) && "Called contains key with invalid key");
      return ((key>=0)&&((unsigned)key<mTSObjects.size()));
   }

   /// Gets the object with the spcified key.
   TSBaseObject* getObject(unsigned int objectKey);

   /// Sets an object entry in the table.
   void setObject(TSBaseObject* object, long key);

   /**
    * Releases the object given by key.
    *
    * @post Obj(key) is deleted, and the ptr is set to NULL.
    */
   void releaseObject(unsigned long key);

private:
   std::vector<TSBaseObject*> mTSObjects; //! Map object key to TS Object ptr
};

}; // End of vpr namespace


#endif
