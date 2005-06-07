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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

/** \class Selector_t Selector_t.h vpr/IO/Selector_t.h
 *
 * Cross-platform selection interface.
 * A selector is used to wait on a set of Handles untils any of the
 * events occur that the user is interested in.  Given a platform-specific
 * wrapper class, this is typedef'd to vpr::Selector.
 *
 * @param RealSelectorImp The type that serves as the true wrapper around
 *                        the platform-specific selector implementation.
 *
 * @see vpr::SelectorNSPR, vpr::SelectorBSD
 */
template<class RealSelectorImp>
class Selector_t : public SelectorBase
{
public:
   /**
    * Adds the given handle to the selector.
    *
    * @pre \p handle is a valid handle.
    * @post \p handle is added to the handle set, and initialized to a mask of
    *       no-events.
    *
    * @param handle The handle to be added to this selector.
    * @param mask   A bitmask specifying the events of interest for this
    *               handle.  This is optional and defaults to 0 (none).
    *
    * @return \c true is returned if \p handle is added successfully;
    *         \c false otherwise.
    */
   bool addHandle(vpr::IOSys::Handle handle, vpr::Uint16 mask = 0)
   {
      return mSelectorImp.addHandle(handle, mask);
   }

   /**
    * Removes the named handle from the selector.
    *
    * @pre \p handle is in the selector.
    * @post \p handle is removed from the set of valid handles.
    *
    * @param handle The handle to be removed from the selector's handle list.
    *
    * @return \c true is returned if \p handle is removed successfully;
    *         \c false otherwise.
    */
   bool removeHandle(vpr::IOSys::Handle handle)
   {
      return mSelectorImp.removeHandle(handle);
   }

   /**
    * Sets the event flags going in to the select to mask.  The flags
    * specify which events should be selected for the given handle.
    *
    * @pre \p handle has already been added to the selector using addHandlei().
    *
    * @param handle The handle whose event flags will be updated.
    * @param mask   The mask used when checking for ready events on the given
    *               handle.
    *
    * @return \c true is returned if \p handle's event flags are set
    *         successfully; \c false otherwise.
    */
   bool setIn(vpr::IOSys::Handle handle, vpr::Uint16 mask)
   {
      return mSelectorImp.setIn(handle, mask);
   }

   /**
    * Gets the current in flag mask.
    *
    * @pre \p handle has already been added to the selector using addHandle().
    *
    * @param handle The handle whose "in" event flags will be returned.
    *
    * @return A bitmask value representing the "in flags" of \p handle.
    */
   vpr::Uint16 getIn(vpr::IOSys::Handle handle)
   {
      return mSelectorImp.getIn(handle);
   }

   /**
    * Gets the current "out flag" mask after a call to select().
    * The value returned will be the bitwise OR of the "out flags".  These
    * state which of the operations named "in flags" were found to be ready.
    *
    * @pre select() has been called.
    *
    * @param handle The handle whose "out" event flags will be returned.
    *
    * @return A bitmask value representing the "out flags" of \p handle.
    *         These flags state which requested events were detected for
    *         \p handle.
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
    *                      vpr::Interval::NoTimeout (wait until an event is
    *                      detected withotu timing out).  Passing
    *                      vpr::Interval::NoWait effects a poll on the
    *                      registered handles and returns immediately.
    *
    * @return vpr::ReturnStatus::Succeed is returned if at least one event was
    *         detected within the timeout interval.
    * @return vpr::ReturnStatus::Timeout is returned if no events were detected
    *         before the timeout expired or if vpr::Interval::NoWait was
    *         passed.  In this case, \p numWithEvents should be checked for a
    *         value greater than 0.
    * @return vpr::ReturnStatus::Fail is returned if the select failed.
    *
    * @note Socket should always be readable if it is not connected.
    */
   vpr::ReturnStatus select(vpr::Uint16& numWithEvents,
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
    * using addHandle().
    *
    * @param index The index of the desired handle.
    *
    * @return A vpr::IOSys::Handle object representing the registered handle
    *         at the given index.
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
    * @return \c true is returned if \p handle was previously registered with
    *         this selector.
    * @return \c false is returned if the handle has not been registered.
    */
   bool containsHandle(vpr::IOSys::Handle handle)
   {
      return mSelectorImp.containsHandle(handle);
   }

protected:
   RealSelectorImp mSelectorImp;     /**< Platform-specific implementation */
};

} // End of vpr namespace


#endif  /* _VPR_SELECTOR_BRIDGE_H_ */
