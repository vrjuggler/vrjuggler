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

#ifndef _GADGET_CYBER_GLOVE_H_
#define _GADGET_CYBER_GLOVE_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Glove.h>
#include <gadget/Devices/VirtualTechnologies/CyberGloveBasic.h>

namespace gadget
{

//: Cyberglove device
//!PUBLIC_API:
class CyberGlove : virtual public Input, public Glove
{
public:
   //: Construct using chunk
   CyberGlove() : mGlove( NULL ), mCalDir( NULL )
   {
   }

   //: Destroy the glove
   virtual ~CyberGlove();

   virtual bool config(ConfigChunk* c);

   static std::string getChunkType() { return std::string("CyberGlove");}

   virtual int startSampling();
   virtual int stopSampling();
   virtual int sample();
   virtual void updateData ();


protected:
   //: The main control loop for the object
   void controlLoop(void* nullParam);

   void copyDataFromGlove();

protected:
   CyberGloveBasic*  mGlove;              // The actual glove
   char*             mCalDir;             // Calibration file directory
};

};

#endif   /* _GADGET_CYBER_GLOVE_H_ */
