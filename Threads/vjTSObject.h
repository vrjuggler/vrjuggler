/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
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
