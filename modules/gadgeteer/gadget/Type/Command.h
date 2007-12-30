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

#ifndef _GADGET_COMMAND_H_
#define _GADGET_COMMAND_H_

#include <gadget/gadgetConfig.h>

#include <boost/concept_check.hpp>   /* for ignore_unused_variable_warning */
#include <boost/noncopyable.hpp>
#include <vector>

#include <vpr/IO/SerializableObject.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Type/CommandPtr.h>
#include <gadget/Type/DigitalData.h>
#include <gadget/Type/SampleBuffer.h>
#include <gadget/Util/DeviceSerializationTokens.h>


namespace gadget
{

const unsigned short MSG_DATA_COMMAND = 423;

typedef DigitalData CommandData;

/** \class Command Command.h gadget/Type/Command.h
 *
 * Command is the abstract base class for devices that translate spoken
 * commends into integer-identified commands.  Drivers for all such
 * devices must derive from this class (through gadget::InputMixer).  This
 * is in addition to gadget::Input.  gadget::Input provides pure virtual
 * function constraints in the following functions: startSampling(),
 * stopSampling(), sample(), and updateData().
 *
 * gadget::Command adds the function getCommandData() for retreiving the
 * received commands.  This is similar to the additions made by
 * gadget::Position and gadget::Analog.
 *
 * @see Input, InputMixer
 */
class GADGET_CLASS_API Command
   : public vpr::SerializableObject
   , boost::noncopyable
{
public:
   typedef gadget::SampleBuffer<CommandData> SampleBuffer_t;

protected:
   /* Constructor/Destructors */
   Command();

public:
   /**
    * Creates a Command instance and returns it wrapped in a
    * CommandPtr object.
    *
    * @since 1.3.7
    */
   static CommandPtr create();

   virtual ~Command();

   virtual bool config(jccl::ConfigElementPtr e)
   {
      boost::ignore_unused_variable_warning(e);
      return true;
   }

   /**
    * Gets the command data for the given devNum.
    *
    * @return Command 0 or 1, if devNum makes sense.
    *         -1 is returned if function fails or if devNum is out of range.
    * @note If devNum is out of range, function will fail, possibly issuing
    *       an error to a log or console - but will not ASSERT.
    */
   const CommandData getCommandData(const int devNum = 0) const;

   /**
    * Helper method to add a collection of command samples to the sample
    * buffers.  This MUST be called by all command devices to add new
    * samples.
    *
    * @post The given command samples are added to the buffers.
    *
    * @param digSample A vector of CommandData objects that represent the
    *                  newest samples taken.
    */
   void addCommandSample(const std::vector< CommandData >& digSample)
   {
      // Locks and then swaps the indices.
      mCommandSamples.lock();
      mCommandSamples.addSample(digSample);
      mCommandSamples.unlock();
   }

   /**
    * Swaps the command data buffers.
    *
    * @post If the ready queue has values, then those values are copied from
    *       the ready queue to the stable queue.  If not, then stable queue
    *       is not changed.
    */
   void swapCommandBuffers()
   {
      mCommandSamples.swapBuffers();
   }

   /**
    * Returns the current stable sample buffers for this device.
    */
   const SampleBuffer_t::buffer_t& getCommandDataBuffer() const
   {
      return mCommandSamples.stableBuffer();
   }

   virtual std::string getInputTypeName()
   {
      return std::string("Command");
   }

   /**
    * Serializes this object into the given object writer.
    *
    * @param writer The object writer to which this object will be
    *               serialized.
    *
    * @throw vpr::IOException is thrown if serialization fails.
    */
   virtual void writeObject(vpr::ObjectWriter* writer);

   /**
    * De-serializes this object.
    *
    * @param reader The object reader from which this object will be
    *               de-serialized.
    *
    * @throw vpr::IOException is thrown if de-serialization fails.
    */
   virtual void readObject(vpr::ObjectReader* reader);

private:
   SampleBuffer_t mCommandSamples; /**< Command samples */
   CommandData    mDefaultValue;   /**< Default command value to return */
};

} // End of gadget namespace

#endif   /* _GADGET_COMMAND_H_ */
