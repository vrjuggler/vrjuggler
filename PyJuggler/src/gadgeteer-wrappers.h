/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
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

#ifndef _GADGETEER_WRAPPERS_H_
#define _GADGETEER_WRAPPERS_H_

#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/PositionInterface.h>


namespace gadgetWrapper
{
   class AnalogInterface
   {
   public:
      void init(const std::string& proxyName)
      {
         mInter.init(proxyName);
      }

      void refresh()
      {
         mInter.refresh();
      }

      std::string getProxyName()
      {
         return mInter.getProxyName();
      }

      bool isConnected()
      {
         return mInter.isConnected();
      }

      gadget::AnalogProxy* getProxy()
      {
         return mInter.getProxy();
      }

/*
      vpr::Interval getTimeStamp()
      {
         return mInter->getTimeStamp();
      }
*/

      float getData()
      {
         return mInter->getData();
      }

   private:
      gadget::AnalogInterface mInter;
   };

   class DigitalInterface
   {
   public:
      void init(const std::string& proxyName)
      {
         mInter.init(proxyName);
      }

      void refresh()
      {
         mInter.refresh();
      }

      std::string getProxyName()
      {
         return mInter.getProxyName();
      }

      bool isConnected()
      {
         return mInter.isConnected();
      }

      gadget::DigitalProxy* getProxy()
      {
         return mInter.getProxy();
      }

/*
      vpr::Interval getTimeStamp()
      {
         return mInter->getTimeStamp();
      }
*/

      int getData()
      {
         return mInter->getData();
      }

   private:
      gadget::DigitalInterface mInter;
   };

   class PositionInterface
   {
   public:
      void init(const std::string& proxyName)
      {
         mInter.init(proxyName);
      }

      void refresh()
      {
         mInter.refresh();
      }

      std::string getProxyName()
      {
         return mInter.getProxyName();
      }

      bool isConnected()
      {
         return mInter.isConnected();
      }

      gadget::PositionProxy* getProxy()
      {
         return mInter.getProxy();
      }

      vpr::Interval getTimeStamp()
      {
         return mInter->getTimeStamp();
      }

      gmtl::Matrix44f getData(float scaleFactor = gadget::PositionUnitConversion::ConvertToFeet)
      {
         return mInter->getData(scaleFactor);
      }

   private:
      gadget::PositionInterface mInter;
   };
}


#endif /* _GADGETEER_WRAPPERS_H_ */
