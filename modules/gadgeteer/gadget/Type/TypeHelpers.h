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

#ifndef _GADGET_TYPE_HELPERS_H_
#define _GADGET_TYPE_HELPERS_H_

#include <gadget/gadgetConfig.h>

#include <boost/mpl/times.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bitor.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/fold.hpp>

#include <gadget/Type/AllBaseTypes.h>


namespace gadget
{

namespace detail
{

/** @name Compile-Time Power-of Computation */
//@{
template<size_t Value, typename Exponent>
struct pow
   : boost::mpl::times<
          boost::mpl::int_<Value>
        , pow<Value, boost::mpl::int_<Exponent::value - 1> >
     >::type
{};

// specialization for 0
template<size_t Value>
struct pow<Value, boost::mpl::int_<0> >
   : boost::mpl::int_<1>::type
{};
//@}

}

namespace type
{

/** @name Metafunctions */
//@{
template<typename N>
struct make_id
{
   typedef detail::pow<2, boost::mpl::int_<N::value> > type;
};

template<typename MaskType, typename Type>
struct compute_id
{
   typedef typename boost::mpl::find<all_base_types, Type>::type::pos pos_type;
   typedef typename
      boost::mpl::bitor_<
           MaskType
         , typename make_id<pos_type>::type
      >::type
   type;
};

template<typename TypeList>
struct compose_id
{
   typedef typename
      boost::mpl::fold<
           TypeList
         , boost::mpl::int_<0>
         , compute_id<boost::mpl::_1, boost::mpl::_2>
      >::type
   type;
};
//@}

}

}


#endif /* _GADGET_TYPE_HELPERS_H_ */
