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

#include <drivers/Open/TUIO/TuioStandalone.h>

using namespace vpr;
using namespace osc;

TuioStandalone::TuioStandalone() :
mActive(false)
{
}

bool TuioStandalone::open(int port)
{
   mPort = port;

   if (mActive)
   {
      return false;
   }
   else
   {

      InetAddr myAddr;

      myAddr.setPort(port);

      mSocket = new SocketDatagram();
      mSocket->setLocalAddr(myAddr);

      mSocket->open();

      try
      {
         mSocket->bind();
      }
      catch (IOException&)
      {
         mActive = false;
         return false;
      }

      mActive = true;
      return true;
   }
}

bool TuioStandalone::close()
{
   mSocket->close();
   delete mSocket;
   mActive = false;
   return true;
}

bool TuioStandalone::updateData()
{
   char szData[20000];
   InetAddr theirAddr;
   const Interval timeout(10, Interval::Msec);
   Uint32 bytes = 0;
   try
   {
      bytes = mSocket->recvfrom(szData, sizeof(szData), theirAddr, Interval::NoWait);
   }
   catch (TimeoutException& ex)
   {
      return false;
   }

   if (bytes > 0)
   {
      const ReceivedPacket * oscPacket = new ReceivedPacket(szData, bytes);
      ReceivedBundle * oscBundle = new  ReceivedBundle(*oscPacket);
      ProcessBundle(*oscBundle);
      return true;
   }

   return false;
}

void TuioStandalone::ProcessBundle(const ReceivedBundle& bundle)
{
   try
   {
      for (ReceivedBundle::const_iterator i = bundle.ElementsBegin();
           i != bundle.ElementsEnd(); ++i)
      {
         if (i->IsBundle())
         {
            ProcessBundle(ReceivedBundle(*i));
         }
         else
         {
            ProcessMessage(ReceivedMessage(*i));
         }
      }
   }
   catch (vpr::Exception& e)
   {
      std::cerr << "\tMalformed OSC bundle: " << e.what() << std::endl;
   }
}

void TuioStandalone::ProcessMessage(const ReceivedMessage& msg)
{
   try
   {
      ReceivedMessageArgumentStream argStream = msg.ArgumentStream();

      if (0 == strcmp(msg.AddressPattern(), "/tuio/2Dcur"))
      {
         const char* cmd;
         argStream >> cmd;
         if (0 == strcmp(cmd, "alive"))
         {
            int32 sessionID;

            // Add alive sessions to mAliveSessions and add any new sessions to mPoints.
            mAliveSessions.clear();
            while(!argStream.Eos())
            {
               argStream >> sessionID;
               if (!findSession(sessionID))
               {
                  int pointID = getLowestAvailID();
                  TuioPoint * newPoint = new TuioPoint(sessionID, pointID);
                  mPoints.push_back(newPoint);
               }
               mAliveSessions.push_back(sessionID);
            }
            // Remove any points from mPoints that are no longer alive.
            std::list<TuioPoint*>::iterator pointIter = mPoints.begin();
            while (pointIter != mPoints.end())
            {
               if (!findAliveSession((*pointIter)->getSessionID()))
               {
                  // No longer alive, remove point
                  pointIter = mPoints.erase(pointIter);
               }
               else
               {
                  ++pointIter;
               }
            }
         }
         else if (0 == strcmp(cmd, "set"))
         {
            int32 sessionID;
            float xpos, ypos, xspeed, yspeed, maccel;
            argStream >> sessionID >> xpos >> ypos >> xspeed >> yspeed >> maccel;

            TuioPoint * point = getPoint(sessionID);
            point->update(sessionID, xpos, ypos, xspeed, yspeed, maccel);
         }
         else if (0 == strcmp(cmd, "fseq"))
         {
            int32 fseq;
            argStream >> fseq;
         }
      }
   }
   catch (vpr::Exception& e)
   {
      std::cerr << "error parsing TUIO message: "<< msg.AddressPattern() <<  " - " << e.what() << std::endl;
   }
}

long TuioStandalone::getLowestAvailID()
{
   int min = 0;
   bool found = false;

   std::list<TuioPoint*>::iterator pointIter;

   while (!found)
   {
      pointIter = mPoints.begin();
      if (pointIter != mPoints.end())
      {
         while (pointIter != mPoints.end())
         {
            if (min == (*pointIter)->getPointID())
            {
               min++;
               found = false;
               break;
            }
            else
            {
               found = true;
               ++pointIter;
            }
         }
      }
      else
      {
         break;
      }
   }
   return min;
}

bool TuioStandalone::findSession(long sessionID)
{
   bool found = false;
   std::list<TuioPoint*>::iterator pointIter;
   for (pointIter = mPoints.begin(); pointIter != mPoints.end(); pointIter++)
   {
      if (sessionID == (*pointIter)->getSessionID())
      {
         found = true;
         break;
      }
   }
   return found;
}

bool TuioStandalone::findAliveSession(long sessionID)
{
   bool found = false;
   std::list<long>::iterator sessionIter;
   for (sessionIter = mAliveSessions.begin(); sessionIter != mAliveSessions.end(); sessionIter++)
   {
      if (sessionID == (*sessionIter))
      {
         found = true;
         break;
      }
   }
   return found;
}

TuioPoint * TuioStandalone::getPoint(long sessionID)
{
   for (std::list<TuioPoint*>::iterator pointIter = mPoints.begin(); pointIter != mPoints.end(); pointIter++)
   {
      if (sessionID == (*pointIter)->getSessionID())
      {
         return (*pointIter);
      }
   }
   return NULL;
}
