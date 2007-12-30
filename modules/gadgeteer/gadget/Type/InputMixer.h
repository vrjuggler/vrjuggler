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

#ifndef _GADGET_INPUT_MIXER_H_
#define _GADGET_INPUT_MIXER_H_

#include <gadget/gadgetConfig.h>
#include <vpr/vpr.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <vpr/IO/SerializableObject.h>

#include <gadget/Type/InputPtr.h>

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

namespace gadget
{

/** \class InputPlaceHolder InputMixer.h gadget/Type/InputMixer.h
 *
 * Placeholder class.
 * This class is provides a generic implementation of the virtual
 * methods of a device.
 *
 * This class is meant to be used to create "place holder" devices
 * that don't actually function on their own but instead just
 * look like a device with a given interface.
 * This can be useful for devices that are progmatically set
 * by systems such as the remote input manager
 */
template <class ParentType>
class InputPlaceHolder  : public ParentType
{
public:
   static InputPtr create()
   {
      return InputPtr(new InputPlaceHolder);
   }

   virtual bool sample()
   {
      return false;
   }

   virtual bool startSampling()
   {
      return false;
   }

   virtual bool stopSampling()
   {
      return false;
   }

   virtual void updateData()
   {
      /* Do nothing. */ ;
   }
};

/**
 * @example "Example use of gadget::InputMixer"
 *
 * \code
 * InputMixer<InputMixer<InputMixer<Input,Digital>,Analog>,Position>
 * \endcode
 */

/** \class InputMixer InputMixer.h gadget/Type/InputMixer.h
 *
 * Class for mixin in base classes of input devices.
 * All devices should derive from a mixed type like below based on their types.
 *
 * This allows us to implement any custom functions that are needed
 * such as writeObject and readObject.
 */
template <class ComposedParent, class NewParent>
class InputMixer : public ComposedParent, public NewParent
{
private:
   // Make sure that we are using serializable objects
   BOOST_STATIC_ASSERT((::boost::is_base_and_derived<vpr::SerializableObject,NewParent>::value));

public:
   /** Type of the placeholder object that can be used to represent us */
   typedef InputPlaceHolder< InputMixer<ComposedParent, NewParent> > MixedPlaceholderType;

   /**
    * Serializes this object into the given object writer.
    *
    * @param writer The object writer to which this object will be serialized.
    *
    * @throw vpr::IOException is thrown if serialization fails.
    */
   void writeObject(vpr::ObjectWriter* writer)
   {
      writer->beginTag(InputMixer<ComposedParent,NewParent>::getInputTypeName());
      ComposedParent::writeObject(writer);
      NewParent::writeObject(writer);
      writer->endTag();
   }

   /**
    * De-serializes this object.
    *
    * @param reader The object reader from which this object will be
    *               de-serialized.
    *
    * @throw vpr::IOException is thrown if de-serialization fails.
    */
   void readObject(vpr::ObjectReader* reader)
   {
      reader->beginTag(InputMixer<ComposedParent,NewParent>::getInputTypeName());
      ComposedParent::readObject(reader);
      NewParent::readObject(reader);
      reader->endTag();
   }

   virtual std::string getInputTypeName()
   {
      return(ComposedParent::getInputTypeName() + NewParent::getInputTypeName());    //Input,Digital,Analog,Position, NEED THIS TOO
   }
};

} // end namespace


#endif   /* _GADGET_INPUT_MIXER_H_ */
