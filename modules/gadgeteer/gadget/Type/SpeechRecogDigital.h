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

#ifndef _GADGET_SPEECH_RECOG_DIGITAL_H_
#define _GADGET_SPEECH_RECOG_DIGITAL_H_

#include <gadget/gadgetConfig.h>
#include <vector>
#include <boost/concept_check.hpp>   /* for ignore_unused_variable_warning */
#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Type/DigitalData.h>
#include <gadget/Type/SampleBuffer.h>
#include <vpr/Util/Debug.h>
#include <vpr/IO/SerializableObject.h>
#include <gadget/Util/DeviceSerializationTokens.h>


namespace gadget
{

   const unsigned short MSG_DATA_SPEECH_RECOG_DIGITAL = 423;

   /**
    * SpeechRecogDigital is the abstract base class for devices that
    * translate spoken commends into integer-identified commands.  Drivers
    * for all such devices must derive from this class (through
    * gadget::InputMixer).  This is in addition to gadget::Input.
    * gadget::Input provides pure virtual function constraints in the
    * following functions: startSampling(), stopSampling(), sample(), and
    * updateData().
    *
    * gadget::SpeechRecogDigital adds the function getDigitalData() for
    * retreiving the received commands.  This is similar to the additions made
    * by gadget::Position and gadget::Analog.
    *
    * @see Input, InputMixer
    */
   class GADGET_CLASS_API SpeechRecogDigital : public vpr::SerializableObject
   {
   public:
      typedef gadget::SampleBuffer<DigitalData> SampleBuffer_t;

   public:
      /* Constructor/Destructors */
      SpeechRecogDigital()
      {
         //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SpeechRecogDigital::SpeechRecogDigital()\n"<< vprDEBUG_FLUSH;
      }

      virtual ~SpeechRecogDigital()
      {
      }

      virtual bool config(jccl::ConfigElementPtr e)
      {
         boost::ignore_unused_variable_warning(e);
         //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SpeechRecogDigital::config()\n"<< vprDEBUG_FLUSH;
         return true;
      }

      /**
       * Gets the digital data for the given devNum.
       *
       * @return SpeechRecogDigital 0 or 1, if devNum makes sense.
       *         -1 is returned if function fails or if devNum is out of range.
       * @note If devNum is out of range, function will fail, possibly issuing
       *       an error to a log or console - but will not ASSERT.
       */
      const DigitalData getDigitalData(int devNum = 0)
      {
         SampleBuffer_t::buffer_t& stable_buffer = mDigitalSamples.stableBuffer();

         if ( (!stable_buffer.empty()) &&
              (stable_buffer.back().size() > (unsigned)devNum) )  // If Have entry && devNum in range
         {
            return stable_buffer.back()[devNum];
         }
         else        // No data or request out of range, return default value
         {
            if ( stable_buffer.empty() )
            {
               vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Warning: SpeechRecogDigital::getDigitalData: Stable buffer is empty. If this is not the first read, then this is a problem.\n" << vprDEBUG_FLUSH;
            }
            else
            {
               vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Warning: SpeechRecogDigital::getDigitalData: Requested devNum " << devNum << " is not in the range available.  May have configuration error\n" << vprDEBUG_FLUSH;
            }
            return mDefaultValue;
         }
      }

      /**
       * Helper method to add a collection of digital samples to the sample
       * buffers.  This MUST be called by all digital devices to add new
       * samples.
       *
       * @post The given digital samples are added to the buffers.
       *
       * @param digSample A vector of DigitalData objects that represent the
       *                  newest samples taken.
       */
      void addDigitalSample(const std::vector< DigitalData >& digSample)
      {
         // Locks and then swaps the indices.
         mDigitalSamples.lock();
         mDigitalSamples.addSample(digSample);
         mDigitalSamples.unlock();
      }

      /**
       * Swaps the digital data buffers.
       *
       * @post If the ready queue has values, then those values are copied from
       *       the ready queue to the stable queue.  If not, then stable queue
       *       is not changed.
       */
      void swapDigitalBuffers()
      {
         mDigitalSamples.swapBuffers();
      }

      /**
       * Returns the current stable sample buffers for this device.
       */
      const SampleBuffer_t::buffer_t& getDigitalDataBuffer()
      {
         return mDigitalSamples.stableBuffer();
      }

      virtual std::string getInputTypeName()
      {
         return std::string("SpeechRecogDigital");
      }

      /** Serializes this object. */
      virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer);

      /** De-serializes this object. */
      virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader);

   protected:
      // gadget::SampleBuffer<T> is not copyable, so neither are we.
      SpeechRecogDigital(const gadget::SpeechRecogDigital&)
         : vpr::SerializableObject()
      {;}

      void operator=(const gadget::SpeechRecogDigital&) {;}

   private:
      SampleBuffer_t    mDigitalSamples; /**< SpeechRecogDigital samples */
      DigitalData       mDefaultValue;   /**< Default digital value to return */
   };

} // End of gadget namespace

#endif   /* _GADGET_SPEECH_RECOG_DIGITAL_H_ */
