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

#include <gadget/gadgetConfig.h>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/not_equal_to.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/bitand.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <gadget/Type/InputDevice.h>
#include <gadget/Devices/Sim/SimInputDevice.h>
#include <gadget/Type/BaseTypeFactory.h>


namespace
{

template<typename Base>
class PlaceholderDevice
   : public Base
{
private:
   PlaceholderDevice()
   {
      /* Do nothing. */ ;
   }

   /** @name gadget::Input Interface Implementation */
   //@{
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
   //@}

public:
   static gadget::InputPtr create()
   {
      return gadget::InputPtr(new PlaceholderDevice);
   }
};

template<template<class> class DeviceType, typename T>
void registerBaseType()
{
   using namespace gadget;
   typedef PlaceholderDevice<DeviceType<T> > dev_type;

   // Assigning dev_type::type_id to a value here works around a linker error
   // when building on Mac OS X 10.7 with LLVM-GCC 4.2.
   const Input::type_id_type type_id = dev_type::type_id;

   BaseTypeFactory::instance()->registerCreator(type_id, dev_type::create);
}

template<template<class> class DeviceType>
struct TypeRegistrar
{
   template<typename BaseTypes>
   void operator()(const BaseTypes&)
   {
      typedef typename
         boost::mpl::inherit_linearly<
              BaseTypes
            , boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2>
         >::type
      base_type;

      registerBaseType<DeviceType, base_type>();
   }
};

}

namespace gadget
{

namespace detail
{

/** @name Compile-Time Logarithm Computation */
//@{
template<typename N, size_t Base>
struct log
   : boost::mpl::plus<
          boost::mpl::int_<1>
        , log<boost::mpl::int_<N::value / Base>, Base>
     >::type
{};

template<size_t Base>
struct log<boost::mpl::int_<1>, Base>
   : boost::mpl::int_<0>::type
{};

template<size_t Base>
struct log<boost::mpl::int_<0>, Base>
   : boost::mpl::int_<0>::type
{};

template<size_t Base>
struct log<boost::mpl::integral_c<size_t, 1>, Base>
   : boost::mpl::int_<0>::type
{};

template<size_t Base>
struct log<boost::mpl::integral_c<size_t, 0>, Base>
   : boost::mpl::int_<0>::type
{};
//@}

/** @name Metafunctions */
//@{
/**
 * Chooses a type based on a compile-time mask comparison.
 */
template<typename N, typename Mask>
struct make_base_type
{
   typedef typename boost::mpl::bitand_<N, Mask>::type index_type;
   typedef typename
      boost::mpl::eval_if<
           boost::mpl::not_equal_to<
                boost::mpl::integral_c<size_t, 0>
              , index_type
           >
         , boost::mpl::at<all_base_types, log<index_type, 2> >
         , boost::mpl::identity<boost::mpl::empty_base>
      >::type
   type;
};

/**
 * Computes the Nth unique subset of gadget::device_base_types.
 */
template<typename N, typename TypeMasks>
struct make_type_combo
{
   /**
    * A boost::mpl::vector containing the base types for an instantiation of
    * gadget::InputDevice<T>.
    */
   typedef typename
      boost::mpl::transform<
           TypeMasks
         , make_base_type<N, boost::mpl::_1>
         , boost::mpl::back_inserter<boost::mpl::vector<> >
      >::type
   type;
};
//@}

}

vprSingletonImpWithInitFunc(BaseTypeFactory, registerBaseDeviceTypes);

/**
 * Registers all the input device base types.
 */
void BaseTypeFactory::registerBaseDeviceTypes()
{
#if 1
   // While it would be nice to generate this registration code at compile
   // time, there are 127 k-combinations of the members of
   // gadget::device_base_types for gadget::InputDevice<T> and 127 more for
   // gadget::SimInputDevice<T>. That is far more than the few that we
   // actually need.
   registerBaseType<InputDevice, Digital>();
   registerBaseType<InputDevice, Analog>();
   registerBaseType<InputDevice, Position>();
   registerBaseType<InputDevice, String>();
   registerBaseType<InputDevice, Command>();
   registerBaseType<InputDevice, Glove>();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Command, Analog>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Digital, Analog>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Digital, Position>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Analog, Position>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Glove, Digital>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Glove, Digital, Analog, Position>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Digital, Analog, Position>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Digital, Analog, Rumble>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Digital, Analog, Position, Rumble>::type
   >();
   registerBaseType<SimInputDevice, Position>();
   registerBaseType<SimInputDevice, Digital>();
   registerBaseType<SimInputDevice, Analog>();
   registerBaseType<
      SimInputDevice, boost::mpl::inherit<Digital, Glove>::type
   >();
#else
   // This produces all unique subsets (k-combinations) of
   // gadget::device_base_types. This is done by counting from 1 to num_types
   // where num_types is the number of types in gadget::device_base_types.
   // A Python representation of this algorithm is as follows:
   //
   // device_base_types = [...]
   // max_size = len(device_base_types)
   // masks = [2 ** i for i in xrange(max_size)]
   // combos = []
   // for i in xrange(1, 2 ** max_size):
   //    subset = []
   //    for m in masks:
   //       index = i & m
   //       if 0 != index:
   //          index = int(math.log(index, 2))
   //          subset.append(device_base_types[index])
   //    combos.append(subset)
   // print combos
   typedef boost::mpl::size<device_base_types> num_types;
   const size_t max_combos(detail::pow<2, num_types>::value);

   // Compute the mask values for each of the types in device_base_types.
   typedef boost::mpl::range_c<size_t, 0, num_types::value> mask_range;
   typedef
      boost::mpl::transform<
           mask_range
         , type::make_id<boost::mpl::_1>
         , boost::mpl::back_inserter<boost::mpl::vector<> >
      >::type
   type_masks;

   // This bit counts from 1 to 2^max_combos and produces all the unique
   // subsets of device_base_types.
   typedef boost::mpl::range_c<size_t, 1, max_combos> type_count_range;
   typedef
      boost::mpl::transform<
           type_count_range
         , detail::make_type_combo<boost::mpl::_1, type_masks>
         , boost::mpl::back_inserter<boost::mpl::vector<> >
      >::type
   type_combos;

   // This last part ends up being the slowest to compile. The instantiation
   // of InputDevice<T> for so many different type combinations seems to be
   // painfully slow.
   {
      TypeRegistrar<InputDevice> r;
      boost::mpl::for_each<type_combos>(r);
   }

   {
      TypeRegistrar<SimInputDevice> r;
      boost::mpl::for_each<type_combos>(r);
   }
#endif

   // This is a special case. gadget::KeyboardMouse is not in
   // gadget:all_base_types.
   registerBaseType<InputDevice, KeyboardMouse>();
}

} // End of gadget namespace
