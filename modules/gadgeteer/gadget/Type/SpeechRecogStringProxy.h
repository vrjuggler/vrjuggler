/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _GADGET_SPEECH_RECOG_STRING_PROXY_H_
#define _GADGET_SPEECH_RECOG_STRING_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/SpeechRecogString.h>
#include <gadget/Type/Proxy.h>
#include <vpr/Util/Assert.h>

namespace gadget
{

/**
 * A proxy class to digital devices, used by the Input Manager.
 *
 * A SpeechRecogStringProxy always points to a digital device and subUnit number,
 * the inputgroup can therefore keep an array of these around and
 * treat them as digital devices which only return a single
 * subDevice's amount of data.  (one int)
 */
class GADGET_CLASS_API SpeechRecogStringProxy : public TypedProxy<SpeechRecogString>
{

public:
   /** @name Construction/Destruction */
   //@{
   SpeechRecogStringProxy()
      : mUnitNum(-1), mData("")
   {;}

   virtual ~SpeechRecogStringProxy()
   {;}
   //@}

   virtual void updateData();

   /** Returns time of last update. */
   virtual vpr::Interval getTimeStamp() const
   {
      return mData.getTime();
   }

   /**
    * Get the string data.
    */
   std::string getData() const
   {
      // If we're stupified, return empty string.  Otherwise, return
      // the current string value.
      return (isStupified() ? ""
                            : mData.getString());
   }

   StringData* getStringData()
   {
      return &mData;
   }

   SpeechRecogString* getSpeechRecogStringPtr()
   {
      // If we're stupified, return NULL.  Otherwise, return mTypedDevice.
      return (isStupified() ? NULL : mTypedDevice);
   }

   int getUnit() const
   {
      return mUnitNum;
   }

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);

   virtual Input* getProxiedInputDevice()
   {
      if((NULL == mTypedDevice) || (mStupified))
      {
         return NULL;
      }

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT((ret_val != NULL) && "Cross-cast in SpeechRecogStringProxy failed");
      return ret_val;
   }

private:
   int mUnitNum;     /**<  The sub-unit of the device we are working with */

   /**
    * Copy of the digital data we are dealing with.
    * @see getData()
    */
   StringData mData;
};

} // End of gadget namespace

#endif
