/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _GADGET_GLOVE_H_
#define _GADGET_GLOVE_H_

#include <gadget/gadgetConfig.h>

#include <boost/noncopyable.hpp>
#include <boost/concept_check.hpp>

#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>

#include <vpr/IO/SerializableObject.h>

#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Type/GloveData.h>
#include <gadget/Type/DigitalData.h> /* For getGloveDataFromDigitalData */
#include <gadget/Type/SampleBuffer.h>
#include <gadget/Type/DeviceInterface.h>
#include <gadget/Type/GlovePtr.h>

namespace gadget
{

const unsigned short MSG_DATA_GLOVE = 520;

/** \class Glove Glove.h gadget/Type/Glove.h
 *
 * This is the abstract base glove class.
 * It specifies the interface to all glove objects in the system.
 * Gadgeteer will deal only with gloves using this interface.
 */
class GADGET_CLASS_API Glove
   : public vpr::SerializableObject
   , boost::noncopyable
{
public:
   typedef SampleBuffer<GloveData> SampleBuffer_t;

protected:
   Glove();

public:
   /**
    * Creates a Glove instance and returns it wrapped in a
    * GlovePtr object.
    *
    * @since 1.3.7
    */
   static GlovePtr create();

   virtual ~Glove();

   virtual bool config(jccl::ConfigElementPtr element);

public:
   /** @name Data access functions */
   //@{
   /**
    * Returns a vector ponting "out" of the component.
    * Can be used for selection, etc.
    */
   const gmtl::Vec3f getTipVector(const GloveData::GloveComponent component,
                                  const int devNum)
      const;

   /**
    * Return the transform matrix of the component tip of the specified
    * component.
    */
   const gmtl::Matrix44f
      getTipTransform(const GloveData::GloveComponent component,
                      const int devNum)
      const;

   /**
    * Returns the transform matrix of the specified joint in world space.
    */
   const gmtl::Matrix44f
      getJointTransform(const GloveData::GloveComponent component,
                        const GloveData::GloveJoint joint, const int devNum)
      const;

   /** Returns a copy of the glove data struct. */
   const GloveData getGloveData(const int devNum) const;
   //@}

   /** @name Buffer functions */
   //@{
   /** 
    * Helper method to add a sample to the sample buffers.
    * This MUST be called by all glove devices to add a new sample.
    * The data samples passed in will then be modified by any local filters.
    *
    * @post Sample is added to the buffers and the local filters are run on
    *       that sample.
    */
   void addGloveSample(const std::vector<GloveData>& gloveSample)
   {
      // Locks and then swaps the indices.
      mGloveSamples.lock();
      mGloveSamples.addSample(gloveSample);
      mGloveSamples.unlock();
   }

   /**
    * Swap the glove data buffers.
    * @post If ready has values, then copy values from ready to stable
    *        if not, then stable keeps its old values
    */
   void swapGloveBuffers()
   {
      mGloveSamples.swapBuffers();
   }

   const SampleBuffer_t::buffer_t& getGloveDataBuffer() const
   {
      return mGloveSamples.stableBuffer();
   }
   //@}

   /**
    * Returns the Input type name used to map into the BaseTypeFactory. In a 
    * device driver, do not override this method unless the device driver is
    * also going to be implementing a new core Input device type.
    */
   virtual std::string getInputTypeName()
   {
      return std::string("Glove");
   }

   /**
    * Serializes this object into the given object writer.
    *
    * @param writer The object writer to which this object will be serialized.
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

   /** Utility function to generate GloveData from DigitalData. */
   const std::vector<GloveData>
      getGloveDataFromDigitalData(const std::vector<DigitalData>& digitalData)
      const;

protected:
   SampleBuffer_t  mGloveSamples;   /**< Glove samples */
   GloveData       mDefaultValue;   /**< Default glove data to return */

   std::vector<PositionInterface> mGlovePositions;
};

} // End of gadget namespace

#endif
