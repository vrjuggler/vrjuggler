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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SELECTOR_BRIDGE_H_
#define _VPR_SELECTOR_BRIDGE_H_
// NOTE: this is the bridge class for use with Selector.h

#include <vpr/vprConfig.h>

#include <string>
#include <vpr/IO/IOSys.h>
#include <vpr/IO/SelectorBase.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/Util/Interval.h>


namespace vpr
{

/**
 * Cross-platform selection interface.
 * A selector is used to wait on a set of Handles untils any of the
 * events occur that the user is interested in.
 */
template<class RealSelectorImp>
class Selector_t : public SelectorBase
{
public:
   /**
    * Adds the given handle to the selector.
    *
    * @pre <code>handle</code> is a valid handle.
    * @post <code>handle</code> is added to the handle set, and initialized to
    *       a mask of no-events.
    *
    * @param handle The handle to be added to this selector.
    * @param mask   A bitmask specifying the events of interest for this
    *               handle.  This is optional and defaults to 0 (none).
    *
    * @return <code>true</code> is returned if <code>handle</code> is added
    *         successfully; <code>false</code> otherwise.
    */
   bool addHandle(vpr::IOSys::Handle handle, vpr::Uint16 mask=0)
   {
      return mSelectorImp.addHandle(handle, mask);
   }

   /**
    * Removes the named handle from the selector.
    *
    * @pre <code>handle</code> is in the selector.
    * @post <code>handle</code> is removed from the set of valid handles.
    *
    * @param handle The handle to be removed from the selector's handle list.
    *
    * @return <code>true</code> is returned if <code>handle</code> is removed
    *         successfully; <code>false</code> otherwise.
    */
   bool removeHandle(vpr::IOSys::Handle handle)
   {
      return mSelectorImp.removeHandle(handle);
   }

   /**
    * Sets the event flags going in to the select to mask.  The flags
    * specify which events should be selected for the given handle.
    *
    * @pre <code>handle</code> has already been added to the selector using
    *      <code>addHandle</code>.
    *
    * @param handle The handle whose event flags will be updated.
    * @param mask   The mask used when checking for ready events on the given
    *               handle.
    *
    * @return <code>true</code> is returned if <code>handle</code>'s event
    *         flags ar3e set successfully; <code>false</code> otherwise.
    */
   bool setIn(vpr::IOSys::Handle handle, vpr::Uint16 mask)
   {
      return mSelectorImp.setIn(handle, mask);
   }

   /**
    * Gets the current in flag mask.
    *
    * @pre <code>handle</code> has already been added to the selector using
    *      <code>addHandle</code>.
    *
    * @param handle The handle whose "in" event flags will be returned.
    *
    * @return A bitmask value representing the "in flags" of
    *         <code>handle</code>.
    */
   vpr::Uint16 getIn(vpr::IOSys::Handle handle)
   {
      return mSelectorImp.getIn(handle);
   }

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
   vpr::Uint16 getOut(vpr::IOSys::Handle handle)
   {
      return mSelectorImp.getOut(handle);
   }

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
    * @return <code>vpr::ReturnStatus::Succeed</code> is returned if at least one
    *         event was detected within the timeout interval.<br>
    *         <code>vpr::ReturnStatus::Timeout</code> is returned if no events were
    *         detected before the timeout expired or if
    *         <code>vpr::Interval::NoWait</code> was passed.  In this case,
    *         <code>numWithEvents</code> should be checked for a value
    *         greater than 0.<br>
    *         <code>vpr::ReturnStatus::Fail</code> is returned if the select
    *         failed.
    * @note Socket should always be readable if it is not connected.
    */
   const ReturnStatus select(vpr::Uint16& numWithEvents,
                 const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      return mSelectorImp.select(numWithEvents, timeout);
   }

   /**
    * For iteration over the registered handles.
    *
    * @return An unsigned value stating how many handles have been registered.
    */
   vpr::Uint16 getNumHandles()
   {
      return mSelectorImp.getNumHandles();
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
      return mSelectorImp.getHandle(index);
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
      return mSelectorImp.containsHandle(handle);
   }


protected:
   RealSelectorImp mSelectorImp;     /**< Platform-specific implementation */
};

}; // End of vpr namespace


#endif  /* _VPR_SELECTOR_BRIDGE_H_ */
