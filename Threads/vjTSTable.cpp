/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <vjConfig.h>
#include <Threads/vjTSTable.h>
#include <Kernel/vjDebug.h>


//: Delete the table
// Delete all objects in the table
vjTSTable::~vjTSTable()
{
   // For all elements in the table
   for(unsigned int i=0;i<mTSObjects.size();i++)
      if(mTSObjects[i] != NULL)        // If valid object
         delete mTSObjects[i];         // Delete them
}

//: Get the object with the spcified key
vjTSBaseObject* vjTSTable::getObject(unsigned int objectKey)
{
   vjASSERT(objectKey < mTSObjects.size());    // Did you check to make sure the table contained it
   return mTSObjects[objectKey];
}


//-----------------------------------------------------------------
//: Set an object entry in the table.
//-----------------------------------------------------------------
void vjTSTable::setObject(vjTSBaseObject* object, long key)
{
   vjASSERT(key >= 0);
   while(mTSObjects.size() <= (unsigned)key)
      mTSObjects.push_back(NULL);
   mTSObjects[key] = object;
}

//-----------------------------------------------------------------
//: Release the object given by key.
//
//! POST: Obj(key) is deleted, and the ptr is set to NULL.
//-----------------------------------------------------------------
void vjTSTable::releaseObject(unsigned long key)
{
   vjASSERT(key < mTSObjects.size());
   if (mTSObjects[key] != NULL)
      delete mTSObjects[key];
   mTSObjects[key] = NULL;
}
