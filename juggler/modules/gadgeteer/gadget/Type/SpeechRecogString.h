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

#ifndef _GADGET_SPEECH_RECOG_STRING_H_
#define _GADGET_SPEECH_RECOG_STRING_H_

#include <gadget/gadgetConfig.h>
#include <vector>
#include <boost/concept_check.hpp>   /* for ignore_unused_variable_warning */
#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Type/StringData.h>
#include <gadget/Type/SampleBuffer.h>
#include <vpr/Util/Debug.h>
#include <vpr/IO/SerializableObject.h>
#include <gadget/Util/DeviceSerializationTokens.h>


namespace gadget
{

   const unsigned short MSG_DATA_SPEECH_RECOG_STRING = 430;

   /**
   *  SpeechRecogString inherits from Input, so it has pure virtual function
   *  constraints from Input in the following functions: StartSampling,
   *  StopSampling, Sample, and UpdateData. <br>
   *  SpeechRecogString adds one new pure virtual function, getStringData() for
   *  retreiving the digital data, similar to the addition for gadget::Position
   *  and gadget::Analog.
   *
   * @see Input
   */
   class GADGET_CLASS_API SpeechRecogString : public vpr::SerializableObject
   {
   public:
      typedef gadget::SampleBuffer<StringData> SampleBuffer_t;

   public:
      /* Constructor/Destructors */
      SpeechRecogString() : mDefaultValue("")
      {
         //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SpeechRecogString::SpeechRecogString()\n"<< vprDEBUG_FLUSH;
      }

      virtual ~SpeechRecogString()
      {
      }

      virtual bool config(jccl::ConfigElementPtr e)
      {
         boost::ignore_unused_variable_warning(e);
         //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SpeechRecogString::config()\n"<< vprDEBUG_FLUSH;
         return true;
      }

      /**
       * Gets the digital data for the given devNum.
       *
       * @return SpeechRecogString 0 or 1, if devNum makes sense.
       *         -1 is returned if function fails or if devNum is out of range.
       * @note If devNum is out of range, function will fail, possibly issuing
       *       an error to a log or console - but will not ASSERT.
       */
      const StringData getStringData(int devNum = 0)
      {
         SampleBuffer_t::buffer_t& stable_buffer = mStringSamples.stableBuffer();

         if ( (!stable_buffer.empty()) &&
              (stable_buffer.back().size() > (unsigned)devNum) )  // If Have entry && devNum in range
         {
            return stable_buffer.back()[devNum];
         }
         else        // No data or request out of range, return default value
         {
            if ( stable_buffer.empty() )
            {
               vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Warning: SpeechRecogString::getStringData: Stable buffer is empty. If this is not the first read, then this is a problem.\n" << vprDEBUG_FLUSH;
            }
            else
            {
               vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Warning: SpeechRecogString::getStringData: Requested devNum " << devNum << " is not in the range available.  May have configuration error\n" << vprDEBUG_FLUSH;
            }
            return mDefaultValue;
         }
      }

      /** Helper method to add a sample to the sample buffers.
      * This MUST be called by all digital devices to add a new sample.
      * The data samples passed in will then be modified by any local filters.
      * @post Sample is added to the buffers and the local filters are run on that sample.
      */
      void addStringSample(const std::vector< StringData >& stringSample)
      {
         // Locks and then swaps the indices.
         mStringSamples.lock();
         mStringSamples.addSample(stringSample);
         mStringSamples.unlock();
      }

      /** Swap the digital data buffers.
       * @post If ready has values, then copy values from ready to stable
       *        if not, then stable keeps its old values
       */
      void swapStringBuffers()
      {
         mStringSamples.swapBuffers();
      }

      const SampleBuffer_t::buffer_t& getStringDataBuffer()
      {
         return mStringSamples.stableBuffer();
      }
      virtual std::string getBaseType()
      {
         return std::string("String");
      }

      virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer);

      virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader);

   protected:
      // gadget::SampleBuffer<T> is not copyable, so neither are we.
      SpeechRecogString(const gadget::SpeechRecogString& d) : vpr::SerializableObject() {;}
      void operator=(const gadget::SpeechRecogString& d) {;}

   private:
      SampleBuffer_t    mStringSamples; /**< String samples */
      StringData       mDefaultValue;   /**< Default String value to return */
   };

} // End of gadget namespace

#endif   /* _GADGET_SPEECH_RECOG_STRING_H_ */
