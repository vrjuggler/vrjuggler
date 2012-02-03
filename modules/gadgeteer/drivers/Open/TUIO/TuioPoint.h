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
#ifndef _GADGET_TUIOPOINT_H_
#define _GADGET_TUIOPOINT_H_

class TuioPoint
{
public:
   int getSessionID()
   {
      return mSessionID;
   }

   int getPointID()
   {
      return mPointID;
   }

   float getXpos()
   {
      return mXpos;
   }

   float getYpos()
   {
      return mYpos;
   }

   float getAngle()
   {
      return mAngle;
   }

   float getXvel()
   {
      return mXvel;
   }

   float getYvel()
   {
      return mYvel;
   }

   float getRotVel()
   {
      return mAngle;
   }

   float getMotAccel()
   {
      return mMotAccel;
   }

   float getRotAccel()
   {
      return mRotAccel;
   }

   TuioPoint(int sessionID,
             int pointID,
             float xPos,
             float yPos,
             float angle,
             float xVel,
             float yVel,
             float rotVel,
             float motAccel,
             float rotAccel)
   {
      mSessionID = sessionID;
      mPointID = pointID;
      mXpos = xPos;
      mYpos = yPos;
      mAngle = angle;
      mXvel = xVel;
      mYvel = yVel;
      mRotVel = rotVel;
      mMotAccel = motAccel;
      mRotAccel = rotAccel;
   }

   TuioPoint(int sessionID,
             int pointID)
   {
      mSessionID = sessionID;
      mPointID = pointID;
      mXpos = 0;
      mYpos = 0;
      mAngle = 0;
      mXvel = 0;
      mYvel = 0;
      mRotVel = 0;
      mMotAccel = 0;
      mRotAccel = 0;
   }

   // update for /tuio/2Dobj message
   void update(int sessionID,
               float xPos,
               float yPos,
               float angle,
               float xVel,
               float yVel,
               float rotVel,
               float motAccel,
               float rotAccel)
   {
      mSessionID = sessionID;
      mXpos = xPos;
      mYpos = yPos;
      mAngle = angle;
      mXvel = xVel;
      mYvel = yVel;
      mRotVel = rotVel;
      mMotAccel = motAccel;
      mRotAccel = rotAccel;
   }

   // update for /tuio/2Dcur message
   void update(int sessionID,
               float xPos,
               float yPos,
               float xVel,
               float yVel,
               float motAccel)
   {
      mSessionID = sessionID;
      mXpos = xPos;
      mYpos = yPos;
      mXvel = xVel;
      mYvel = yVel;
      mMotAccel = motAccel;
   }

protected:
   int mSessionID;
   int mPointID;
   float mXpos;
   float mYpos;
   float mAngle;
   float mXvel;
   float mYvel;
   float mRotVel;
   float mMotAccel;
   float mRotAccel;
};

#endif
