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

#ifndef _GADGET_EVENT_DATA_EXAMINER_H_
#define _GADGET_EVENT_DATA_EXAMINER_H_

#include <gadget/gadgetConfig.h>

#include <cassert>
#include <boost/function.hpp>


namespace gadget
{

namespace event
{

/** \class BaseExaminer DataExaminer.h gadget/Event/DataExaminer.h
 *
 * This class captures the details that are common to all DataExaminer
 * instanations.
 *
 * @tparam DataType The type of a sample received from a device proxy.
 *
 * @since 2.1.16
 */
template<typename DataType>
class BaseExaminer
{
public:
   typedef boost::function<void (const DataType&)> callback_type;

   /**
    * Stores the given functor as the callback for this examiner.
    *
    * @pre If this examiner is going to be used, then \p cb is not empty.
    */
   void setEventCallback(const callback_type& cb)
   {
      mCallback = cb;
   }

protected:
   /**
    * Invokes the callback associated with this data examiner.
    *
    * @pre \c mCallback is not empty.
    * @post \p consumed is true.
    *
    * @param d        The data for the event.
    * @param consumed A reference to a boolean flag indicating whether the
    *                 event data has been consumed. Invocation of this method
    *                 results in this variable being set to true.
    */
   void addEvent(const DataType& d, bool& consumed)
   {
      consumed = true;
      assert(! mCallback.empty());
      mCallback(d);
   }

private:
   callback_type mCallback;
};

/** \class DataExaminer DataExaminer.h gadget/Event/DataExaminer.h
 *
 * A "Data Examiner" is a concept whose purpose is to process samples from
 * input devices and determine what constitutes an event. The interface to
 * implement is simple:
 *
 * \code
 * void examine(const DataType&, bool&);
 * \endcode
 *
 * This class can be specialized for different combinations of event tags and
 * data types. This default implementation stores every data sample received
 * from a device proxy to be emitted later as an event.
 *
 * It is up to individual implementations to determine how to handle the
 * \c consumed variable. This default implementation effectively ignores it.
 *
 * @tparam EventTag The tag identifying the kind of event. This is used to
 *                  provide a mechanism for specializing this type based on
 *                  specific needs.
 * @tparam DataType The type of a sample received from a device proxy.
 *
 * @since 2.1.16
 */
template<typename EventTag, typename DataType>
class DataExaminer
   : public BaseExaminer<DataType>
{
public:
   /**
    * Stores every data sample received from a device proxy to be emitted
    * later as an event.
    */
   void examine(const DataType& d, bool& consumed)
   {
      addEvent(d, consumed);
   }
};

}

}


#endif /* _GADGET_EVENT_DATA_EXAMINER_H_ */
