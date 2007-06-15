/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

/////////////////////////////////////////////////////////////////////////
//
// ibox tracking class
//
////////////////////////////////////////////////////////////////////////

#ifndef _GADGET_IBOX_H_
#define _GADGET_IBOX_H_

#include <gadget/Devices/DriverConfig.h>
#include <vector>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Analog.h>
#include <drivers/Immersion/IBox/IBoxStandalone.h>


namespace gadget
{

/**
 * The Immersion Box input class.
 *
 * The Immersion Box is a 4 Digital, 4 Analog input device, the Ibox class
 * therefore must inherit from both Digital and Analog.  The class uses
 * the HCI library for a simple interface to the IBox.
 */
//class IBox : public Input, public Digital, public Analog
class IBox
   : public input_digital_analog_t
{
protected:
   struct IboxData
   {
      /** Constructor
      * Init both vectors to the default size of 4
      */
      IboxData()
         : button(4), analog(4)
      {;}

      /** Helper to set all the times */
      void setTime()
      {
         button[0].setTime();
         button[1].setTime( button[0].getTime() );
         button[2].setTime( button[0].getTime() );
         button[3].setTime( button[0].getTime() );
         analog[0].setTime( button[0].getTime() );
         analog[1].setTime( button[0].getTime() );
         analog[2].setTime( button[0].getTime() );
         analog[3].setTime( button[0].getTime() );
      }

      std::vector<DigitalData> button;    // size of 4
      std::vector<AnalogData>  analog;    // size of 4
   };

public:
   /** Construction/Destruction */
   IBox() : mDoneFlag(false) //: Input(), Digital(), Analog()  <<<<< HERE
   {
      // re-set Analog min/max values to ibox defaults.
      this->setMin( 0.0f );
      this->setMax( 255.0f );
   }
   ~IBox();

   virtual bool config(jccl::ConfigElementPtr e);

   // Input Pure Virtual Functions
   bool startSampling();
   bool stopSampling();
   bool sample();
   void updateData();

   static std::string getElementType();
   void controlLoop(void* nullParam);

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
   // juggler ibox data in the range of [0..255]
   //IBOX_DATA   theData[3];

   // ibox native data in the range of [0..255]
   IboxStandalone mPhysicalIbox;
   std::string    mPortName;
   long           mBaudRate;
   bool           mDoneFlag;
};

} // End of gadget namespace

#endif
