/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_ELEXOL_ETHER24_H_
#define _GADGET_ELEXOL_ETHER24_H_

#include <gadget/Devices/DriverConfig.h>
#include <vector>
#include <vpr/Thread/Thread.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/InputMixer.h>
#include <drivers/Elexol/Ether24/Ether24Standalone.h>


namespace gadget
{

/**
 * @see Digital
 */
class Ether24
   : public InputMixer<Input, Digital>
{
public:
   Ether24();
   virtual ~Ether24();

   /** Configures the device with specified config element. */
   bool config(jccl::ConfigElementPtr e);

   /**
    * Writes the initial values specified in the configuration element
    * to the specified \p port.
    *
    * @param port The port to set the initial values on.
    * @param elt The configuration element that contains the initial values.
    */
   void writeInitialValues(Elexol::Port port, jccl::ConfigElementPtr elt);

   /** Begins sampling. */
   virtual bool startSampling();

   /** Main thread of control for this active object. */
   void controlLoop();

   /** Stops sampling. */
   virtual bool stopSampling();

   /** Samples data. */
   virtual bool sample();

   /** Updates to the sampled data. */
   virtual void updateData();

   /** Returns what element type is associated with this class. */
   static std::string getElementType();

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
   jccl::ConfigElementPtr   mConfigElement;
   Ether24Standalone        mDevice;
   vpr::InetAddr            mAddress;
   std::vector<DigitalData> mDigitalData;
   bool                     mDone;
   vpr::Uint8               mInvertA;
   vpr::Uint8               mInvertB;
   vpr::Uint8               mInvertC;
};

} // End of gadget namespace

#endif /*_GADGET_ELEXOL_ETHER24_H_*/
