/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _VPR_SELECTOR_IMPL_BSD_H_
#define _VPR_SELECTOR_IMPL_BSD_H_

#include <vpr/vprConfig.h>

#include <vector>
#include <map>
#include <string>

#include <vpr/IO/SelectorBase.h>
#include <vpr/IO/IOSys.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Status.h>
#include <vpr/Util/Interval.h>


namespace vpr {

/**
 * BSD Implementation of ---> Cross-platform selection interface.
 *
 * A selector is used to wait on a set of Handles untils any of the
 * events occur that the user is interested in.
 *
 * Implementation site of the vpr::Selector_t bridge.
 *
 * @author Patrick Hartling
 */
class SelectorImplBSD : public SelectorBase
{
public:
   // XXX: What should we do when we are adding a handler while
   //    also iterating through the current handles.  Bad things "may" happend
   // This happens ALL the time with acceptors because they add to the reactor
   // as they are executing an handleEvent themselves

   /**
    * Adds the given handle to the selector.
    *
    * @pre <code>handle</code> is a valid handle.
    * @pre <code>handle</code> is added to the handle set, and its mask is
    *      initialized using the given value.
    *
    * @param handle The handle to be added to the selector's handle set.
    * @param mask   The mask used when checking for ready events on the given
    *               handle.  This argument is optional and defaults to 0 (no
    *               events).
    *
    * @return <code>true</code> is returned if the given handle is added
    *         successfully; <code>false</code> otherwise.
    */
   bool addHandle(vpr::IOSys::Handle handle, vpr::Uint16 mask=0);

   /**
    * Removes a handle from the selector.
    *
    * @pre <code>handle</code> is in the selector.
    * @post <code>handle</code> is removed from the set of valid handles.
    *
    * @param handle The handle to be removed from the selector's handle set.
    *
    * @return <code>true</code> is returned if the given handle is removed
    *         successfully; <code>false</code> otherwise.
    */
   bool removeHandle(vpr::IOSys::Handle handle);

   /**
    * Sets the event flags going in to the select to mask.  The flags
    * specify which events should be selected for the given handle.
    *
    * @pre  <code>handle</code> has already been registered with this selector.
    * @post <code>handle</code> has its mask updated to use the given value.
    *
    * @param handle The handle whose mask will be set.
    * @param mask   The mask used when checking for ready events on the given
    *               handle.
    */
   bool setIn(vpr::IOSys::Handle handle, vpr::Uint16 mask);

   /**
    * Gets the current in-flag mask.
    *
    * @pre <code>handle</code> has already been added to the selector using
    *      <code>addHandle</code>.
    *
    * @param handle The handle whose "in" event flags will be returned.
    *
    * @return A bitmask value representing the "in flags" of
    *         <code>handle</code>.
    */
   vpr::Uint16 getIn(vpr::IOSys::Handle handle);

   /**
    * Gets the current "out flag" mask after a call to <code>select</code>.
    * The value returned will be the bitwise OR of the "out flags".  These
    * state which of the operations named "in flags" were found to be ready.
    *
    * @pre <code>select</code> has been called.
    *
    * @param handle The handle whose "out" event flags will be returned.
    *
    * @return A bitmask value representing the "out flags" of
    *         <code>handle</code>.  These flags state which requested events
    *         were detected for <code>handle</code>.
    */
   vpr::Uint16 getOut(vpr::IOSys::Handle handle);

   /**
    * Poll for any ready events among the registered handles using their in
    * flags.
    *
    * @param numWithEvents Upon completion, this holds the number of items that
    *                      have events.
    * @param timeout       The interval to wait for an event to be raised.
    *                      This argument is optional and defaults to
    *                      <code>vpr::Interval::NoTimeout</code> (wait until
    *                      an event is detected withotu timing out).  Passing
    *                      <code>vpr::Interval::NoWait</code> effects a poll
    *                      on the registered handles and returns immediately.
    *
    * @return <code>vpr::Status::Success</code> is returned if at least one
    *         event was detected within the timeout interval.<br>
    *         <code>vpr::Status::Timeout</code> is returned if no events were
    *         detected before the timeout expired or if
    *         <code>vpr::Interval::NoWait</code> was passed.  In this case,
    *         <code>numWithEvents</code> should be checked for a value
    *         greater than 0.<br>
    *         <code>vpr::Status::Failure</code> is returned if the select
    *         failed.
    */
   vpr::Status select(vpr::Uint16& numWithEvents,
                      const vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * For iteration over the registered handles.
    *
    * @return An unsigned value stating how many handles have been registered.
    */
   vpr::Uint16 getNumHandles()
   {
      return mPollDescs.size();
   }

   /**
    * Gets the handle at the given index within the collection of registered
    * handles.  The index is determined by the order of handle addition
    * using <code>addHandle</code>.
    *
    * @param index The index of the desired handle.
    *
    * @return A <code>vpr::IOSys::Handle</code> object representing the
    *         registered handle at the given index.
    */
   vpr::IOSys::Handle getHandle(vpr::Uint16 index)
   {
      return mPollDescs[index].fd;
   }

   /**
    * Test if the selector contain the given handle.
    *
    * @param handle The handle of interest.
    *
    * @return <code>true</code> is returned if <code>handle</code> was
    *         previously registered with this selector.<br>
    *         <code>false</code> is returned if the handle has not been
    *         registered.
    */
   bool containsHandle(vpr::IOSys::Handle handle)
   {
      return (getHandle(handle) != mPollDescs.end());
   }

protected:
   /// Simple type used for the vector of poll descriptors.
   struct BSDPollDesc {
      int         fd;
      vpr::Uint16 in_flags;
      vpr::Uint16 out_flags;
   };

   /**
    * Gets the index of the handle given.
    *
    * @return .end() is returned if the given index is not found<br>
    *         Otherwise, the index to the handle in mPollDescs is returned.
    */
   std::vector<BSDPollDesc>::iterator getHandle(int handle);

   /// List of Poll Descriptions to pass to <code>select</code>(2).
   std::vector<BSDPollDesc> mPollDescs;
};

}; // End of vpr namespace


#endif  /* _VPR_SELECTOR_IMPL_BSD_H_ */
