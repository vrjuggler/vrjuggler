/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJ_TS_OBJECT_H_
#define _VJ_TS_OBJECT_H_
//#pragma once

#include <vjConfig.h>

//-----------------------------------------------------------------
//: Base Thread Specific object.
//
// Used so that we can have an array of heterogenous TS objects.
// Also defines some memeber that all TS Objects need.
//-----------------------------------------------------------------
class vjTSBaseObject
{
public:
   virtual ~vjTSBaseObject()
   {;}

   //: Creates an instance of the object
   //! NOTE: This is not a copy
   virtual vjTSBaseObject* createNew()
   { return(NULL);}
};

//-----------------------------------------------------------------
//: This is the object actually stored in the system.
//-----------------------------------------------------------------
template <class T>
class vjTSObject : public vjTSBaseObject
{
public:
   virtual ~vjTSObject()
   {
   //   delete mLocalObj;
   }

   //-----------------------------------------------------------------
   //: Return the address of our object
   //-----------------------------------------------------------------
   T* getObject()
   { return &mLocalObj; }

   //-----------------------------------------------------------------
   //: Clone the object
   //
   //!NOTE: This creates a new object using the default constructor.
   //+      It does NOT copy this object.
   //-----------------------------------------------------------------
   virtual vjTSBaseObject* createNew()
   {
      vjTSObject<T>* new_object = new vjTSObject<T>();
      return new_object;
   }

private:
   T mLocalObj;
};


#endif
