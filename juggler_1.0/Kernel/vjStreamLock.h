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


#ifndef _VJ_STREAM_LOCK_H_
#define _VJ_STREAM_LOCK_H_
//#pragma once

#include <iostream.h>
#include <Sync/vjMutex.h>

//: Class to lock a stream for multi-process output
// This class can be used to share a stream between two processes
// It is used by simply outputing in the stream a vjStreamLock constructed using a mutex <br>
// <br>
// EX: cout << vjStreamLock(mutex) << "Hello world" << flush << vjStreamUnLock(mutex);
class vjStreamLock
{
public:
   explicit vjStreamLock(vjMutex& mutex) : mMutex(mutex)
   {;}

   friend ostream& operator<<(ostream&, const vjStreamLock&);
	
private:
   vjMutex& mMutex;
};



//: Class to unlock a stream that has been previously locked
// This class can be used to share a stream between two processes
// It is used by simply outputing in the stream a vjStreamUnLock constructed using a mutex <br>
// <br>
// EX: cout << vjStreamLock(mutex) << "Hello world" << flush << vjStreamUnLock(mutex);
class vjStreamUnLock
{
public:
   explicit vjStreamUnLock(vjMutex& mutex) : mMutex(mutex)
   {;}

   friend ostream& operator<<(ostream&, const vjStreamUnLock&);
	
private:
   vjMutex& mMutex;
};


#endif
