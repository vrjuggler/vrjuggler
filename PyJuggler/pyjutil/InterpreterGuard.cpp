/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
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
#include "InterpreterGuard.h"


namespace PyJuggler
{

vpr::TSObjectProxy<InterpreterGuard::State> InterpreterGuard::mState;

InterpreterGuard::State::State() : gilLocked(false), pyState(NULL)
{
   vprDEBUG(pyjDBG_CXX, vprDBG_HVERB_LVL)
      << "InterpreterGuard::State constructor -- this->pyState: "
      << std::hex << this->pyState << std::dec << std::endl
      << vprDEBUG_FLUSH;
}

InterpreterGuard::State::~State()
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

InterpreterGuard::InterpreterGuard() : mMyLock(false)
{
   if ( NULL == mState->pyState )
   {
      vprDEBUG(pyjDBG_CXX, vprDBG_VERB_LVL)
         << "Getting new thread state data\n" << vprDEBUG_FLUSH;

      // Use an existing interpreter object for creating the new thread
      // state to get proper sharing of information between threads.
      // XXX: PyInterpreterState_Head() is documented as only for use by
      // advanced debuggers.  There may be a better way to do this.
      mState->pyState = PyThreadState_New(PyInterpreterState_Head());
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

InterpreterGuard::~InterpreterGuard()
{
   if ( mMyLock )
   {
      vprASSERT(mState->gilLocked && "GIL is not held, but I thought it was.");
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
