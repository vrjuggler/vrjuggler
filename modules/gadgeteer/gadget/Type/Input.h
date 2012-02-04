/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_INPUT_H_
#define _GADGET_INPUT_H_

#include <gadget/gadgetConfig.h>

#include <boost/noncopyable.hpp>

#include <vpr/vpr.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Thread/Thread.h>
#include <vpr/IO/SerializableObject.h>

#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Type/InputPtr.h>


namespace gadget
{

/** \class Input Input.h gadget/Type/Input.h
 *
 * gadget::Input is the abstract base class from which all input objects
 * derive.
 *
 * gadget::Input is the base class for all Input Devices, all the devices are
 * therefore forced to implement the pure virtual functions of sample(),
 * startSampling(), stopSampling(), and updateData().
 *
 * Dummy devices can use a default constructor, but physical devices should
 * have a Constructor which takes a config element and calls the Input
 * constructor taking a jccl::ConfigElementPtr.
 *
 * All Physical devices will inherit from not Input but another abstract
 * class which inherits from Input, currently there is support for
 * Positional Devices, Analog Devices, and Digital devices, each has its own
 * added pure virtual functions providing a simple and equal interface to
 * themselves.
 *
 * @note We make the assumption in all devices that while updateData() is being
 *       called, no other process will try to read the current data.
 *       We can make this assumption because the whole idea of UpdateData() is
 *       to bring in a current copy of the data for threads to process for a
 *       frame.  Because of this, threads should not be reading data while
 *       it is being updated to the most recent copy.
 */
class GADGET_API Input
   : public vpr::SerializableObject
   , boost::noncopyable
{
protected:
   /**
    * Default Constructor.
    */
   Input();

public:
   /**
    * Destructor.
    *
    * Free the memory for the Instance Name and Serial Port strings if
    * allocated.
    */
   virtual ~Input();

   /**
    * Config method.
    *
    * This baselevel config will fill the base datamembers
    * when found in the jccl::ConfigElementPtr such as instance name.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /**
    * Samples the device.
    *
    * Reads the next set of input. This method is normally used internally
    * by threaded drivers to sample data repetitively in a separate thread.
    * (This new data is not accessable until updateData() is called.)
    */
   virtual bool sample() = 0;

   /**
    * Starts a device sampling.
    *
    * Normally this will spawn a thread which will repeatedly calls sample().
    * This function should return true when it sucessfully starts and false
    * otherwise.
    */
   virtual bool startSampling() = 0;

   /**
    * Stops samping.
    *
    * Reverse the effects of startSampling().
    */
   virtual bool stopSampling() = 0;

   /**
    * Updates the data.
    *
    * After this function is called, subsequent calls to
    * gadget::TypedProxy::getData() will return the most recent data at the
    * time of \em this function call. Data is guaranteed to be valid and
    * static until the next call to updateData().
    */
   virtual void updateData() = 0;

   /**
    * @since 1.1.19
    */
   void updateDataIfNeeded();

   /**
    * @since 1.1.19
    */
   void resetData()
   {
      mNeedUpdate = true;
   }

   /**
    * Returns the string rep of the element type used to config this device.
    * This string is used by the device factory to look up device drivers
    * based up the type of element it is trying to load.
    */
   static const std::string getElementType()
   {
      return std::string("Undefined");
   }

   /**
    * Returns the name identifying this instance of the device.
    * This is the name given to the device in its config element (e.g.,
    * "MyFlockOfBirds", "The Ibox", etc.).
    */
   const std::string getInstanceName() const;

   /**
    * The type of the data returned by getTypeId().
    *
    * @since 2.1.19
    */
   typedef vpr::Uint16 type_id_type;

   /**
    * Returns the unique type identifier for the current device type. This is
    * used for type registration with BaseTypeFactory to build a "virtual"
    * representation of this device type for remote input. In other words,
    * this method is only used in conjunction with device types that can be
    * shared in a cluster configuration.
    *
    * @see gadget::BaseTypeFactory
    *
    * @note This method replaced \c getInputTypeName() in 2.1.19.
    *
    * @since 2.1.19
    */
   virtual type_id_type getTypeId() const = 0;

   /**
    * Serializes this object into the given object writer.
    *
    * @param writer The object writer to which this object will be serialized.
    */
   virtual void writeObject(vpr::ObjectWriter* writer);

   /**
    * De-serializes this object.
    *
    * @param reader The object reader from which this object will be
    *               de-serialized.
    */
   virtual void readObject(vpr::ObjectReader* reader);

   /** Is this input device active? */
   bool isActive() const
   {
      return mActive;
   }

protected:
   std::string    mInstName;
   vpr::Thread*   mThread;       /**< The thread being used by the driver. */
   bool           mActive;       /**< Is the driver active? */
   bool           mNeedUpdate;   /**< @since 1.1.19 */
};

} // end namespace

#endif   /* GADGET_INPUT_H */
