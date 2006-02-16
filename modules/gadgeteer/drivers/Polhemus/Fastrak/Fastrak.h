/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

// ----------------------------------------------------------------------------
// Author:
//     Nicolas Tarrin <nicolas.tarrin@inria.fr>
//     February 2001
// ----------------------------------------------------------------------------

// 4 stations: from 0 to 3

#ifndef _GADGET_FASTRAK_
#define _GADGET_FASTRAK_

#include <gadget/Devices/DriverConfig.h>

#include <iostream>
#include <string>
#include <gmtl/Matrix.h>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/InputMixer.h>

#include <drivers/Polhemus/Fastrak/FastrakStandalone.h>


namespace gadget
{

class Fastrak : public InputMixer<InputMixer<Input,Digital>, Position>
{
public:
   Fastrak();           // must call gadget::Analog()? see gadget/Type/Analog.h
   virtual ~Fastrak() throw ();

   // return what element type is associated with this class.
   static std::string getElementType();

public:
   /// gadget::Input pure virtual functions
   virtual bool config(jccl::ConfigElementPtr fastrakElement);

   /** Starts a new thread. */
   virtual bool startSampling();

   /** Reads data from the Fastrak. */
   virtual bool sample();

   /** Swaps the data and gadget::Input tri-buffered indicies. */
   virtual void updateData();

   /** Kills the sample thread. */
   virtual bool stopSampling();

   /** Device's config element name (match the definition). */
   virtual char* getDeviceName()
   {
      return "Fastrak";
   }

public:
   /** gadget::Digital pure virtual function. */
   virtual int getDigitalData(int station = 0);
   /** gadget::Position pure virtual function. */
   virtual gmtl::Matrix44f getPosData(int station = 0);

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

private:
   void controlLoop();

   int mButtonState;                   /**< only one button on station 0 */
   float mTrackersPosition[4][3];      /**< 4 stations, 3 coordinates */
   float mTrackersOrientation[4][3];   /**< 4 stations, 3 orientations */
   vpr::Thread* mSampleThread;
   bool mExitFlag;

   char* mConfigFile;

   FastrakStandalone mFastrakDev;
};

} // End of gadget namespace


#endif // _GADGET_FASTRAK_
