/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _GADGET_TUIOSTANDALONE_H_
#define _GADGET_TUIOSTANDALONE_H_

#include <vpr/vpr.h>
#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketDatagram.h>
#include <vpr/Util/Interval.h>
#include <vpr/IO/TimeoutException.h>
#include <list>

#include "oscpack/osc/OscReceivedElements.h"
#include "TuioPoint.h"

#define DEFAULT_PORT 3333

class TuioStandalone
{
public:
   TuioStandalone();
   ~TuioStandalone() {;}

   bool open(int port);
   bool close();

   bool updateData();

   bool isActive()
   {
      return mActive;
   }
   std::list<TuioPoint*> points()
   {
      return mPoints;
   }

private:
   void ProcessBundle(const osc::ReceivedBundle& bundle);
   void ProcessMessage(const osc::ReceivedMessage& msg);
   long getLowestAvailID();
   bool findSession(long sessionID);
   bool findAliveSession(long sessionID);
   TuioPoint * getPoint(long sessionID);

   bool                 mActive;  /**< If the driver is active. */
   int                  mPort;
   vpr::SocketDatagram  *mSocket;
   vpr::InetAddr        mAddress; /**< Address of TUIO device. */

   std::list<long>		mAliveSessions;
   std::list<long>		mAliveIDs;
   std::list<TuioPoint*> mPoints;
};
#endif