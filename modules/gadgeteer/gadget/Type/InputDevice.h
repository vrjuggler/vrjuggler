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

#ifndef _GADGET_INPUT_DEVICE_H_
#define _GADGET_INPUT_DEVICE_H_

#include <gadget/gadgetConfig.h>

#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <vpr/vprTypes.h>

#include <gadget/Type/TypeHelpers.h>


namespace gadget
{

/**
 * @example "Example use of gadget::InputDevice"
 *
 * The code below shows how to set the base class for a device that uses
 * digital, analog, and positonal data. The order of the types listed in
 * boost::mpl::inherit does not matter.
 *
 * \code
 * #include <boost/mpl/inherit.hpp>
 * #include <gadget/Type/InputDevice.h>
 *
 * typedef
 *    boost::mpl::inherit<
 *         gadget::Digital
 *       , gadget::Analog
 *       , gadget::Position
 *    >::type
 * base_input_type;
 *
 * class MyDriver
 *    : public gadget::InputDevice<base_input_type>
 * {
 * };
 * \endcode
 *
 * Do not include gadget::Input in the type list. It is already a base class
 * of gadget::InputDevice.
 *
 * For the case of a device that uses only one type of data (such as analog),
 * the usage is even simpler:
 *
 * \code
 * #include <gadget/Type/InputDevice.h>
 *
 * class MyDriver
 *    : public gadget::InputDevice<gadget::Analog>
 * {
 * };
 * \endcode
 */

/** \class InputDevice InputDevice.h gadget/Type/InputDevice.h
 *
 * @since 2.1.18
 */
template<typename Base>
class InputDevice
   : public Input
   , public Base
{
public:
   /** Convenience typedef for use by subclasses. */
   typedef InputDevice<Base> input_device_;

   /**
    * Computes the ordered subset of all_base_types that is the list of base
    * types for this instantiation.
    *
    * @see writeObject()
    * @see readObject()
    */
   typedef typename
      boost::mpl::copy_if<
           all_base_types
         , boost::is_base_of<boost::mpl::_1, Base>
         , boost::mpl::back_inserter<boost::mpl::vector<> >
      >::type
   type_list;

   /**
    * The unique identifier for this type instantiation. This is computed as a
    * bitwise OR of the index value of each member of type_list within
    * all_base_types.
    */
   static const type_id_type type_id = type::compose_id<type_list>::type::value;

   /** @name vpr::SerializableObject Overrides */
   //@{
   virtual void writeObject(vpr::ObjectWriter* writer)
   {
      Input::writeObject(writer);
      Caller<WriteInvoker> caller(this, writer);
      boost::mpl::for_each<type_list, wrap<boost::mpl::_1> >(caller);
   }

   virtual void readObject(vpr::ObjectReader* reader)
   {
      Input::readObject(reader);
      Caller<ReadInvoker> caller(this, reader);
      boost::mpl::for_each<type_list, wrap<boost::mpl::_1> >(caller);
   }
   //@}

   virtual type_id_type getTypeId() const
   {
      return type_id;
   }

private:
   /**
    * This exists solely to allow abstract Gadgeteer device types (such as
    * gadget::Input) to be used in the type sequence that is used for
    * iteration in the call to boost::mpl::for_each<>().
    */
   template<typename T>
   struct wrap
   {
#if defined(__GNUC__) && __GNUC__ < 4
      /**
       * This type exists to fix compile errors with versions of GCC older
       * than 4.0. It is used internally by MPL when wrap<T> is utilized by
       * boost::mpl::for_each().
       */
      template<typename U>
      struct apply
      {
         typedef wrap<U> type;
      };
#endif
   };

   struct WriteInvoker
   {
      typedef vpr::ObjectWriter* arg_type;

      template<typename BaseType>
      static void invoke(BaseType* obj, vpr::ObjectWriter* writer)
      {
         obj->BaseType::writeObject(writer);
      }
   };

   struct ReadInvoker
   {
      typedef vpr::ObjectReader* arg_type;

      template<typename BaseType>
      static void invoke(BaseType* obj, vpr::ObjectReader* reader)
      {
         obj->BaseType::readObject(reader);
      }
   };

   template<typename InvokerType
          , typename ArgType = typename InvokerType::arg_type
          >
   class Caller
   {
   public:
      Caller(InputDevice* owner, ArgType arg)
         : owner(owner)
         , arg(arg)
      {
      }

      template<typename BaseType>
      void operator()(wrap<BaseType>)
      {
         InvokerType::template invoke<BaseType>(owner, arg);
      }

   private:
      InputDevice* owner;
      ArgType arg;
   };
};

}


#endif /* _GADGET_INPUT_DEVICE_H_ */
