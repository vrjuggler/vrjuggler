#ifndef _VJ_DEVICE_FACTORY_H_
#define _VJ_DEVICE_FACTORY_H_
#pragma once

#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>
#include <Config/vjConfigChunk.h>

//: Base class for virtual construction of devices
// Implementations of this class are registered with the device factory
// for each device in the system
class vjDeviceConstructorBase
{
public:
   //: Constructor
   //! POST: Device is registered
   vjDeviceConstructorBase() {;}

   //: Create the device
   virtual vjInput* createDevice(vjConfigChunk* chunk)
   {
      vjDEBUG(0) << "ERROR: vjDeviceConstructorBase::createDevice: Should never be called" << vjDEBUG_FLUSH;
      return NULL;
   }

   //: Get the string desc of the type of chunk we can create
   virtual std::string    getChunkType()
   { return std::string("BaseConstructor: Invalid type"); }
};


template <class DEV>
class vjDeviceConstructor : public vjDeviceConstructorBase
{
public:
   vjDeviceConstructor();

   vjInput* createDevice(vjConfigChunk* chunk)
   {
      DEV* new_dev = new DEV;
      bool success = new_dev->config(chunk);
      if(success)
      {
         return new_dev;
      } else {
         delete new_dev;
         return NULL;
      }
   }

   virtual std::string getChunkType()
   { return DEV::getChunkType(); }
};


//: Object used for creating devices
//!NOTE: Singleton
class vjDeviceFactory
{
private:
   // Singleton so must be private
   vjDeviceFactory()
   {
      mConstructors = std::vector<vjDeviceConstructorBase*>(0);
      vjASSERT(mConstructors.size() == 0);
   }

   // This should be replaced with device plugins
   //! POST: Devices are loaded that the system knows about
   void hackLoadKnownDevices();

public:
   void registerDevice(vjDeviceConstructorBase* constructor);

   //: Query if the factory knows about the given device
   //!PRE: chunk != NULL, chunk is a valid chunk
   //!ARGS: chunk - chunk we are requesting about knowledge to create
   //!RETURNS: true - factory knows how to create the device
   //+          false - factory does not know how to create the device
   bool recognizeDevice(vjConfigChunk* chunk);

   //: Load the specified device
   //!PRE: recognizeDevice(chunk) == true
   //!ARGS: chunk - specification of the device to load
   //!RETURNS: null - Device failed to load
   //+         other - Pointer to the loaded device
   vjInput* loadDevice(vjConfigChunk* chunk);

private:
   //: Find a constructor for the given device type
   //!RETURNS: -1 - Not found
   //+            - Index of the constructorck
   int   findConstructor(vjConfigChunk* chunk);

   void debugDump();


private:
   std::vector<vjDeviceConstructorBase*> mConstructors;  //: List of the device constructors


public:     // ------ SINGLETON ----- ///
   //: Return singleton instance of the class
   static vjDeviceFactory* instance()
   {
      if(mInstance == NULL)
      {
         mInstance = new vjDeviceFactory();
         mInstance->hackLoadKnownDevices();
      }

      return mInstance;
   }

private:
   static vjDeviceFactory* mInstance;
};

#endif

