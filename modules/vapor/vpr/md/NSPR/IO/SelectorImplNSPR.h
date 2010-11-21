/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _VPR_SELECTOR_IMPL_NSPR_H_
#define _VPR_SELECTOR_IMPL_NSPR_H_

#include <vpr/vprConfig.h>

#include <vector>
#include <map>
#include <string>
#include <prio.h>

#include <vpr/IO/SelectorBase.h>
#include <vpr/IO/IOSys.h>
#include <vpr/Util/Debug.h>


namespace vpr
{

/** \class SelectorImplNSPR SelectorImplNSPR.h vpr/IO/Selector.h
 *
 * NSPR Implementation of cross-platform selection interface.
 * A selector is used to wait on a set of Handles untils any of the
 * events occur that the user is interested in.
 *
 * This is an implementation site of the vpr::Selector_t bridge.
 */
class VPR_CLASS_API SelectorImplNSPR : public SelectorBase
{
public:
   // XXX: What should we do when we are adding a handler while
   //    also iterating through the current handles.  Bad things "may" happen.
   // This happens ALL the time with acceptors because they add to the reactor
   // as they are executing an handleEvent themselves.

   /**
    * Adds the given handle to the selector.
    *
    * @pre \p handle is a valid handle.
    * @post \p handle is added to the handle set and initialized to a mask of
    *       no-events.
    */
   bool addHandle(const IOSys::Handle handle, const vpr::Uint16 mask = 0);

   /**
    * Removes a handle from the selector.
    *
    * @pre \p handle is in the selector.
    * @post \p handle is removed from the set of valid handles.
    */
   bool removeHandle(const IOSys::Handle handle);

   /**
    * Sets the event flags going in to the select to mask.
    */
   bool setIn(const IOSys::Handle handle, const vpr::Uint16 mask);

   /**
    * Gets the current in flag mask.
    */
   vpr::Uint16 getIn(const IOSys::Handle handle) const;

   /**
    * Gets the current out flag mask.
    */
   vpr::Uint16 getOut(const IOSys::Handle handle) const;

   /**
    * Select.
    *
    * @param numWithEvents Upon completion, this holds the number of items
    *                      that have events.
    * @param timeout       The number of milliseconds to select for
    *                      (passing vpr::Interval::NoWait means don't wait).
    *
    *
    * @throws vpr::TimeoutException If no events were detected before the
    *                               timeout expired or if
    *                               vpr::Interval::NoWait was passed. In
    *                               this case, \p numWithEvents should be
    *                               checked for a value greater than 0.
    * @throws vpr::IOException      If the select failed.
    */
   void select(vpr::Uint16& numWithEvents,
               const vpr::Interval& timeout = vpr::Interval::NoTimeout);

   /**
    * Gets the number of handles.
    */
   vpr::Uint16 getNumHandles() const
   {
      return mPollDescs.size();
   }

   IOSys::Handle getHandle(const vpr::Uint16 index) const
   {
      return mPollDescs[index].fd;
   }

   bool containsHandle(const IOSys::Handle handle) const
   {
      return (getHandle(handle) != mPollDescs.end());
   }

protected:

   /**
    * Gets the index of the handle given.
    *
    * @return .end() if not found, else the index to the handle in mPollDescs.
    */
   std::vector<PRPollDesc>::iterator getHandle(PRFileDesc const* handle);

   /**
    * Gets the index of the handle given (const version).
    *
    * @return .end() if not found, else the index to the handle in mPollDescs.
    */
   std::vector<PRPollDesc>::const_iterator getHandle(PRFileDesc const* handle)
      const;

   PRUint16 convertMaskVprToNspr(const vpr::Uint16 mask) const;
   vpr::Uint16 convertMaskNsprToVpr(const PRUint16 mask) const;

protected:

   std::vector<PRPollDesc> mPollDescs;    /**< List of Poll Descriptions to pass to PR_Poll() */

/*
   struct PRPollDesc
   {
      PRFileDesc* fd;
      PRInt16 in_flags;
      PRInt16 out_flags;
   };
*/
};

} // End of vpr namespace


#endif  /* _VPR_SELECTOR_IMP_NSPR_H_ */
