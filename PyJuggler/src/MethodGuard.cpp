/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling
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

#include <stdlib.h>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Debug.h>

#include "Debug.h"
#include "MethodGuard.h"


namespace PyJuggler
{

// XXX: This really should be in MethodGuard.cpp.
vpr::TSObjectProxy<MethodGuard::State> MethodGuard::mState;

MethodGuard::State::State() : gilLocked(false), pyState(NULL)
{
   vprDEBUG(pyjDBG_CXX, vprDBG_HVERB_LVL)
      << "MethodGuard::State constructor -- this->pyState: "
      << std::hex << this->pyState << std::dec << std::endl
      << vprDEBUG_FLUSH;
}

MethodGuard::State::~State()
{
   if ( NULL != pyState )
   {
      PyEval_AcquireThread(pyState);
      PyInterpreterState_Clear(pyState->interp);
      PyEval_ReleaseThread(pyState);
      PyInterpreterState_Delete(pyState->interp);
      PyThreadState_Delete(pyState);
      pyState = NULL;
   }
}

MethodGuard::MethodGuard() : mMyLock(false)
{
   if ( NULL == mState->pyState )
   {
      vprDEBUG(pyjDBG_CXX, vprDBG_VERB_LVL)
         << "Getting new thread state data\n" << vprDEBUG_FLUSH;
      mState->pyState = PyThreadState_New(PyInterpreterState_New());
   }

   vprDEBUG(pyjDBG_CXX, vprDBG_HVERB_LVL)
      << "mState->pyState: " << std::hex << mState->pyState << std::dec
      << std::endl << vprDEBUG_FLUSH;

   if ( ! mState->gilLocked )
   {
      vprDEBUG(pyjDBG_CXX, vprDBG_VERB_LVL)
         << std::hex << this << std::dec << " locking\n" << vprDEBUG_FLUSH;

      // Lock the GIL.
      PyEval_AcquireThread(mState->pyState);

      vprDEBUG(pyjDBG_CXX, vprDBG_VERB_LVL)
         << std::hex << this << std::dec << " locked\n" << vprDEBUG_FLUSH;

      mState->gilLocked = true;
      mMyLock           = true;
   }
}

MethodGuard::~MethodGuard()
{
   if ( mMyLock && mState->gilLocked )
   {
      vprDEBUG(pyjDBG_CXX, vprDBG_VERB_LVL)
         << std::hex << this << std::dec << " unlocking\n" << vprDEBUG_FLUSH;

      // Unlock the GIL.
      PyEval_ReleaseThread(mState->pyState);

      vprDEBUG(pyjDBG_CXX, vprDBG_VERB_LVL)
         << std::hex << this << std::dec << " unlocked\n" << vprDEBUG_FLUSH;

      mState->gilLocked = false;
   }
}

} // End of PyJuggler namespace
