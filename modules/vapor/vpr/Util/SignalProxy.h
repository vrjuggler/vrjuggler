// vrkit is (C) Copyright 2005-2008
//    by Allen Bierbaum, Aron Bierbuam, Patrick Hartling, and Daniel Shipton
//
// This file is part of vrkit.
//
// vrkit is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// vrkit is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _VPR_SIGNAL_PROXY_H_
#define _VPR_SIGNAL_PROXY_H_

#include <boost/signals/connection.hpp>


namespace vpr
{

/** \class SignalProxy SignalProxy.h vpr/Util/SignalProxy.h
 *
 * Simple proxy for \c boost::signal<> instances that exposes only the
 * connect() member function. This allows for a (somewhat) cleaner interface
 * to accessing a \c boost::signal<> instance that is a class data member
 * without making that data member public. It is intended to be used for cases
 * when external classes need to connect a slot to a signal but the emitting
 * of the signal can only be performed by the holding class implementation.
 *
 * The idea for this class comes from the following article about signals and
 * slots:
 *
 * http://www.scottcollins.net/articles/a-deeper-look-at-signals-and-slots.html
 *
 * @note This class was renamed from vrkit::SignalProxy and moved into the
 *       vrkit::signal namespace in version 0.47.
 *
 * @since 2.3.2
 */
template<typename SignalT>
class SignalProxy
{
public:
   typedef SignalT signal_t;

   SignalProxy(signal_t& signal)
      : mSignal(signal)
   {
      /* Do nothing. */ ;
   }

   SignalProxy(const SignalProxy& rhs)
      : mSignal(rhs.mSignal)
   {
      /* Do nothing. */ ;
   }

   boost::signals::connection connect(typename signal_t::slot_type slot)
   {
      return mSignal.connect(slot);
   }

   /**
    * Connects the given slot to the contained signal using the identified
    * group.
    *
    * @param group The group in which the given slot will be connected.
    * @param slot  The slot to connect to this signal.
    *
    * @since 0.26.1
    */
   boost::signals::connection
   connect(const typename signal_t::group_type& group,
           typename signal_t::slot_type slot)
   {
      return mSignal.connect(group, slot);
   }

private:
   signal_t& mSignal;
};

}


#endif /* _VPR_SIGNAL_PROXY_H_ */
