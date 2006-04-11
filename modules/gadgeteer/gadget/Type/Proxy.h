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

#ifndef _GADGET_PROXY_H_
#define _GADGET_PROXY_H_

#include <gadget/gadgetConfig.h>

#include <typeinfo>

#include <gadget/InputManager.h>
#include <gadget/Type/Input.h>
#include <jccl/Config/ConfigChunk.h>
#include <gadget/Util/Debug.h>

namespace gadget
{

   //class Input;
   //class Kernel;

   /**
    * Base class for all input proxies.
    * @todo rename to InputProxy
    */
   class GADGET_CLASS_API Proxy
   {
   public:
      Proxy()
      {
         mStupified = true;
      }

      virtual ~Proxy()
      {
         ;
      }

      /**
       * Configures the proxy.
       * @post Proxy is configured (it is not registered yet though).
       * @return success.
       */
      virtual bool config(jccl::ConfigChunkPtr chunk)
      {
         mName = chunk->getFullName();
         return true;
      }

      /**
       * Refreshes the proxy.
       * Checks for the device that it is supposed to point at.
       */
      virtual bool refresh() = 0;

      virtual void updateData()
      {
         ;
      }

      /**
       * Return a pointer to the base class of the devices being proxied
       * @return NULL if no device is proxied.
       */
      virtual Input* getProxiedInputDevice() = 0;

      /**
       * Returns the string rep of the chunk type used to config this device.
       * Used by the Input Manager to find chunks that construct devices.
       */
      static std::string getChunkType()
      {
         return "Undefined";
      }

      std::string getName()
      {
         return mName;
      }

      virtual bool isStupified() const { return mStupified; }

      void stupify(bool newState = true) {mStupified = newState;}
      

   protected:
      std::string mName;         /**< The name of the proxy */
      bool        mStupified;    /**< Is the proxy current stupified (returns default data) */
   };


   template <class DEV_TYPE>
   class TypedProxy : public Proxy
   {
   public:
      TypedProxy() : mDeviceName("Unknown"), mTypedDevice(NULL)
      {
         ;
      }

      /**
       * Sets the proxy to point to the given type specific device.
       *
       * @pre devPtr must be a valid device of type DEV_TYPE.
       * @post The proxy now references the analog device.
       * @param anaPtr Pointer to the device.
       */
      virtual void set(DEV_TYPE* devPtr)
      {
         mTypedDevice = devPtr;
         stupify(false);
      }

      virtual bool refresh()
      {
         Input* input_dev = NULL;
         input_dev = InputManager::instance()->getDevice(mDeviceName);
                
         if ( NULL == input_dev )       // Not found, so stupify
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
            << "vjTypedProxy::refresh: Could not find device: "
            << mDeviceName << std::endl << vprDEBUG_FLUSH;
            stupify(true);
         }
         else
         {
            DEV_TYPE* typed_dev = dynamic_cast<DEV_TYPE*>(input_dev);
            if ( NULL == typed_dev )
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "vjTypedProxy::config: Device was of wrong type: "
               << mDeviceName << " it was type:" << typeid(input_dev).name()
               << std::endl << vprDEBUG_FLUSH;
               stupify(true);
               return false;
            }
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
            << "   Proxy config()'ed" << std::endl << vprDEBUG_FLUSH;
            set(typed_dev);    // Set the proxy
         }
         return true;
      }
   virtual std::string getDeviceName() const { return mDeviceName; } 

protected:
   std::string    mDeviceName;   /**< Name of the device to link up with */
   DEV_TYPE*      mTypedDevice;  /**< The device (type specific pointer) */
};

} // end namespace

#endif
