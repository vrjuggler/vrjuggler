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

#ifndef _GADGET_PINCH_GLOVE_H_
#define _GADGET_PINCH_GLOVE_H_

#include <gadget/gadgetConfig.h>
#include <string>

#include <gadget/Type/Input.h>
#include <gadget/Type/Glove.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/InputMixer.h>
#include <gadget/Devices/Fakespace/PinchGloveStandalone.h>

#include <gadget/Type/Finger.h>
#include <gadget/Type/Hand.h>

namespace gadget
{

/**
 * Fakespace Pinchglove Device.
 */
//class PinchGlove : virtual public Input, public Glove, public Digital
class PinchGlove : public InputMixer<Input,Digital>
{
public:
   /** Default constructor. */
   PinchGlove() : mGlove( NULL ), mDigitalData(10)
   {;}

   /** Shuts down the glove device. */
   virtual ~PinchGlove();

   virtual bool config( jccl::ConfigChunkPtr c );

   static std::string getChunkType() { return std::string("PinchGlove");}

   virtual int startSampling();
   virtual int stopSampling();
   virtual int sample();
   virtual void updateData ();

   /**
    * Finger values.
    */
   enum Finger
   {
      LTHUMB = 0, LINDEX = 1, LMIDDLE = 2, LRING = 3, LPINKY = 4,
      RTHUMB = 6, RINDEX = 7, RMIDDLE = 8, RRING = 9, RPINKY = 10
   };

   
protected:
   /** The main control loop for this device. */
   void controlLoop( void* nullParam );

   void copyDataFromGlove();
   void updateFingerAngles();

protected:
   //vjThread*         mControlThread; /**< The thread of control for the object */
   PinchGloveStandalone* mGlove;           /**< The actual glove */

   Hand left, right;
   std::vector<DigitalData> mDigitalData;  /**< Digitals for each finger.
                                                Doesn't store time correctly */
   std::string    mPortName;
   int            mBaudRate;
};


};

#endif   /* _GADGET_PINCH_GLOVE_H_ */
