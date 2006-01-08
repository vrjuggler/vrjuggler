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

#ifndef _GADGET_NETDEVICE_FACTORY_H_
#define _GADGET_NETDEVICE_FACTORY_H_
//#pragma once

#include <gadget/gadgetConfig.h>
#include <vector>
#include <boost/concept_check.hpp>

#include <gadget/Type/Input.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <vpr/Util/Singleton.h>

#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

namespace gadget
{

/** \class BaseTypeConstructorBase BaseTypeFactory.h gadget/Type/BaseTypeFactory.h
 *
 * Base class for virtual construction of devices.
 * Implementations of this class are registered with the device factory
 * for each device in the system.
 */
class BaseTypeConstructorBase
{
public:
   /**
    * Constructor.
    * @post Device is registered.
    */
   BaseTypeConstructorBase()
   {;}

   virtual ~BaseTypeConstructorBase()
   {;}

   /** Creates the device. */
   virtual Input* createNetDevice(std::string baseType)
   {
      boost::ignore_unused_variable_warning(baseType);
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "ERROR: DeviceConstructorBase::createDevice: Should never be called"
         << std::endl << vprDEBUG_FLUSH;
      return NULL;
   }

   /** Gets the name of the type of element we can create. */
   virtual std::string getInputTypeName()
   {
      return std::string("BaseConstructor: Invalid type");
   }
};

/** \class BaseTypeFactory BaseTypeFactory.h gadget/Type/BaseTypeFactory.h
 *
 * Object used for creating devices.
 */
class GADGET_CLASS_API BaseTypeFactory
{
private:
   // Singleton so must be private
   BaseTypeFactory()
   {
      mConstructors = std::vector<BaseTypeConstructorBase*>(0);
      vprASSERT(mConstructors.size() == 0);
   }

   // This should be replaced with device plugins.
   /**
    * @post Devices are loaded that the system knows about.
    */
   void hackLoadKnownDevices();

public:
   void registerNetDevice(BaseTypeConstructorBase* constructor);

   /**
    * Queries if the factory knows about the given device.
    *
    * @pre base_type is a valid configuration element type.
    * @param base_type The base type of the config element element we are
    *                  requesting about knowledge to create.
    *
    * @return true if the factory knows how to create the device; false if not.
    */
   bool recognizeNetDevice(std::string base_type);

   /**
    * Loads the specified device.
    *
    * @pre recognizeNetDevice(base_type) == true.
    *
    * @param base_type The base type of the specification of the device to
    *                  load.
    *
    * @return NULL is returned if the device failed to load.
    *         Otherwise, a pointer to the loaded device is returned.
    *
    * @see recognizeNetDevice
    */
   Input* loadNetDevice(std::string base_type);

private:
   /**
    * Finds a constructor for the given device type.
    * @return -1 is returned if the constructor is not found.
    *         Otherwise, the index of the constructor is returned.
    */
   int   findConstructor(std::string base_type);

   void debugDump();

private:
   std::vector<BaseTypeConstructorBase*> mConstructors;  /**<  List of the device constructors */

   vprSingletonHeaderWithInitFunc(BaseTypeFactory, hackLoadKnownDevices);
};

/** \class BaseTypeConstructor BaseTypeFactory.h gadget/Type/BaseTypeFactory.h
 *
 * Type-specific input device creator.
 */
template <class DEV>
class BaseTypeConstructor : public BaseTypeConstructorBase, public DEV
{
public:
   BaseTypeConstructor()
   {
      vprASSERT(BaseTypeFactory::instance() != NULL);
      BaseTypeFactory::instance()->registerNetDevice(this);
   }

   virtual ~BaseTypeConstructor() throw ()
   {
      /* Do nothing. */ ;
   }

   Input* createNetDevice(std::string baseType)
   {
      boost::ignore_unused_variable_warning(baseType);
      DEV* new_dev = new DEV;
      //bool success = new_dev->config(element);
      //if(success)
      //{
         return new_dev;
      //}
      //else
      //{
      //   delete new_dev;
      //   return NULL;
      //}
   }

   virtual std::string getInputTypeName()
   {
      return DEV::getInputTypeName();
   }

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
};

} // end namespace gadget

#endif
