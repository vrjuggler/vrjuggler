/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#ifndef _VPR_SELECTOR_IMPL_SIM_H_
#define _VPR_SELECTOR_IMPL_SIM_H_

#include <vpr/vprConfig.h>

#include <vector>

#include <vpr/vpr.h>
#include <vpr/IO/SelectorBase.h>
#include <vpr/Util/Interval.h>
#include <vpr/md/SIM/IO/IOSysSIM.h>


namespace vpr
{

class VPR_CLASS_API SelectorImplSIM : public vpr::SelectorBase
{
public:
   /**
    * Adds the given handle to the selector.
    *
    * @pre handle is a valid handle.
    * @pre handle is added to the handle set, and its mask is
    *      initialized using the given value.
    *
    * @param handle The handle to be added to the selector's handle set.
    * @param mask   The mask used when checking for ready events on the given
    *               handle.  This argument is optional and defaults to 0 (no
    *               events).
    *
    * @return true is returned if the given handle is added
    *         successfully; false otherwise.
    */
   bool addHandle(vpr::IOSysSIM::Handle handle, vpr::Uint16 mask = 0);

   /**
    * Removes a handle from the selector.
    *
    * @pre handle is in the selector.
    * @post handle is removed from the set of valid handles.
    *
    * @param handle The handle to be removed from the selector's handle set.
    *
    * @return true is returned if the given handle is removed
    *         successfully; false otherwise.
    */
   bool removeHandle(vpr::IOSysSIM::Handle handle);

   /**
    * Sets the event flags going in to the select to mask.
    *
    * @pre  handle has already been registered with this selector.
    * @post handle has its mask updated to use the given value.
    *
    * @param handle The handle whose mask will be set.
    * @param mask   The mask used when checking for ready events on the given
    *               handle.
    */
   bool setIn(vpr::IOSysSIM::Handle handle, vpr::Uint16 mask);

   /**
    * Gets the current in-flag mask.
    *
    * @pre handle has already been added to the selector using
    *      addHandle.
    *
    * @param handle The handle whose "in" event flags will be returned.
    *
    * @return A bitmask value representing the "in flags" of
    *         handle.
    */
   vpr::Uint16 getIn(vpr::IOSysSIM::Handle handle);

   /**
    * Gets the current "out flag" mask after a call to select.
    * The value returned will be the bitwise OR of the "out flags".  These
    * state which of the operations named "in flags" were found to be ready.
    *
    * @pre select has been called.
    *
    * @param handle The handle whose "out" event flags will be returned.
    *
    * @return A bitmask value representing the "out flags" of
    *         handle.  These flags state which requested events
    *         were detected for handle.
    */
   vpr::Uint16 getOut(vpr::IOSysSIM::Handle handle);

   /**
    * Poll for any ready events among the registered handles using their in
    * flags.
    *
    * @param numWithEvents Upon completion, this holds the number of items that
    *                      have events.
    * @param timeout       The interval to wait for an event to be raised.
    *                      This argument is optional and defaults to
    *                      vpr::Interval::NoTimeout (wait until
    *                      an event is detected withotu timing out).  Passing
    *                      vpr::Interval::NoWait effects a poll
    *                      on the registered handles and returns immediately.
    *
    * @return vpr::ReturnStatus::Success is returned if at least one
    *         event was detected within the timeout interval.<br>
    *         vpr::ReturnStatus::Timeout is returned if no events were
    *         detected before the timeout expired or if
    *         vpr::Interval::NoWait was passed.  In this case,
    *         numWithEvents should be checked for a value
    *         greater than 0.<br>
    *         vpr::ReturnStatus::Failure is returned if the select
    *         failed.
    */
   vpr::ReturnStatus select(vpr::Uint16& numWithEvents,
                            const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * For iteration over the registered handles.
    *
    * @return An unsigned value stating how many handles have been registered.
    */
   vpr::Uint16 getNumHandles (void)
   {
      return mPollDescs.size();
   }

   /**
    * Gets the handle at the given index within the collection of registered
    * handles.  The index is determined by the order of handle addition
    * using addHandle.
    *
    * @param index The index of the desired handle.
    *
    * @return A vpr::IOSysSIM::Handle object representing the registered
    *         handle at the given index.
    */
   vpr::IOSysSIM::Handle getHandle (vpr::Uint16 index)
   {
      return mPollDescs[index].fd;
   }

   /**
    * Test if the selector contain the given handle.
    *
    * @param handle The handle of interest.
    *
    * @return true is returned if handle was
    *         previously registered with this selector.<br>
    *         false is returned if the handle has not been
    *         registered.
    */
   bool containsHandle (vpr::IOSysSIM::Handle handle)
   {
      return (getHandle(handle) != mPollDescs.end());
   }

protected:
   /// Simple type used for the vector of poll descriptors.
   struct SimPollDesc
   {
      vpr::IOSysSIM::Handle fd;
      vpr::Uint16        in_flags;
      vpr::Uint16        out_flags;
   };

   /**
    * Gets the index of the handle given.
    *
    * @return .end() is returned if the given index is not found<br>
    *         Otherwise, the index to the handle in mPollDescs is returned.
    */
   std::vector<SimPollDesc>::iterator getHandle(vpr::IOSysSIM::Handle handle);

   /// List of poll descriptors to pass to select.
   std::vector<SimPollDesc> mPollDescs;
};

} // End of vpr namespace


#endif /* _VPR_SELECTOR_IMPL_SIM_H_ */
