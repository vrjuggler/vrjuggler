// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _GADGETEER_WRAPPERS_H_
#define _GADGETEER_WRAPPERS_H_

#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/EventWindowInterface.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/SpeechRecogDigitalInterface.h>
#include <gadget/Type/SpeechRecogStringInterface.h>


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

      vpr::Interval getTimeStamp()
      {
         return mInter->getTimeStamp();
      }

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

      vpr::Interval getTimeStamp()
      {
         return mInter->getTimeStamp();
      }

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

   class EventWindowInterface
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

      gadget::EventWindowProxy* getProxy()
      {
         return mInter.getProxy();
      }

      vpr::Interval getTimeStamp()
      {
         return mInter->getTimeStamp();
      }

      bool modifierOnly(gadget::Keys modKey)
      {
         return mInter->modifierOnly(modKey);
      }

      int keyPressed(gadget::Keys keyId)
      {
         return mInter->keyPressed(keyId);
      }

      gadget::EventWindow::EventQueue getEventQueue()
      {
         return mInter->getEventQueue();
      }

   private:
      gadget::EventWindowInterface mInter;
   };

   class SpeechRecogDigitalInterface
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

      gadget::SpeechRecogDigitalProxy* getProxy()
      {
         return mInter.getProxy();
      }

      vpr::Interval getTimeStamp()
      {
         return mInter->getTimeStamp();
      }

      float getData()
      {
         return mInter->getData();
      }

   private:
      gadget::SpeechRecogDigitalInterface mInter;
   };

   class SpeechRecogStringInterface
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

      gadget::SpeechRecogStringProxy* getProxy()
      {
         return mInter.getProxy();
      }

      vpr::Interval getTimeStamp()
      {
         return mInter->getTimeStamp();
      }

      std::string getData()
      {
         return mInter->getData();
      }

   private:
      gadget::SpeechRecogStringInterface mInter;
   };
}


#endif /* _GADGETEER_WRAPPERS_H_ */
