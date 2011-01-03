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

#include <vpr/vprConfig.h>

#include <signal.h>
#include <boost/concept_check.hpp>

#include <vpr/vprTypes.h>
#include <vpr/Util/Assert.h>
#include <vpr/Thread/Signal.h>


namespace vpr
{

// ============================================================================
// vpr::SignalSet stuff.
// ============================================================================

bool SignalSet::emptySet()
{
   bool status(true);

#ifdef HAVE_SIGEMPTYSET
   if ( sigemptyset(&mSigSet) != 0 )
   {
      status = false;
   }
#else
   mSigSet = 0;
#endif

   return status;
}

bool SignalSet::fillSet()
{
   bool status(true);

#ifdef HAVE_SIGFILLSET
   if ( sigfillset(&mSigSet) != 0 )
   {
      status = false;
   }
#else
   mSigSet = ~(sigset_t) 0;
#endif

   return status;
}

bool SignalSet::addSignal(const int sigNum)
{
   bool status(true);

   vprASSERT(sigNum >= 1 && "Invalid signal number");

#ifdef HAVE_SIGADDSET
   if ( sigaddset(&mSigSet, sigNum) != 0 )
   {
      status = false;
   }
#else
   mSigSet |= (1 << (sigNum - 1));
#endif

   return status;
}

bool SignalSet::removeSignal(const int sigNum)
{
   bool status(true);

   vprASSERT(sigNum >= 1 && "Invalid signal number");

#ifdef HAVE_SIGDELSET
   if ( sigdelset(&mSigSet, sigNum) != 0 )
   {
      status = false;
   }
#else
   mSigSet &= ~(1 << (sigNum - 1)) ;
#endif

   return status;
}

bool SignalSet::isMember(const int sigNum) const
{
   bool is_member;

   vprASSERT(sigNum >= 1 && "Invalid signal number");

#ifdef HAVE_SIGISMEMBER
   is_member = (sigismember(&mSigSet, sigNum) == 1);
#else
   is_member = ((mSigSet & (1 << (sigNum - 1))) != 0);
#endif

   return is_member;
}

// ============================================================================
// vpr::SignalAction stuff.
// ============================================================================

const vpr::SignalHandler_t SignalAction::DefaultAction = SIG_DFL;
const vpr::SignalHandler_t SignalAction::IgnoreAction  = SIG_IGN;

SignalAction::SignalAction(vpr::SignalHandler_t handler,
                           const vpr::SignalSet* sig_set, const int flags)
{
   if ( sig_set == NULL )
   {
      init(handler, NULL, flags);
   }
   else
   {
      init(handler, sig_set->getMask(), flags);
   }
}

// ============================================================================
// vpr::SigHandler stuff.
// ============================================================================

bool SigHandler::registerHandler(const int sigNum,
                                 vpr::SignalHandler_t handler,
                                 const bool restart)
{
   boost::ignore_unused_variable_warning(restart);
   vpr::SignalAction sa(handler);
   return registerHandler(sigNum, sa);
}

bool SigHandler::registerHandler(const int sigNum, vpr::SignalAction& action,
                                 const bool restart)
{
   bool status(true);

#ifdef SA_RESTART
   if ( sigNum != SIGALRM && restart )
   {
      action.mSA.sa_flags |= SA_RESTART;
   }
#endif

   if ( vpr::SigHandler::sigaction(sigNum, &action.mSA) != 0 )
   {
      status = false;
   }

   return status;
}

int SigHandler::sigaction(const int sigNum, const struct sigaction* action,
                          struct sigaction* oldAction)
{
#ifdef HAVE_SIGACTION
   return ::sigaction(sigNum, action, oldAction);
#else
   // If sigaction(2) is not available, simulate its functionality using
   // signal(2).
   struct sigaction sa;

   if ( oldAction == NULL )
   {
      oldAction = &sa;
   }

   if ( action == NULL )
   {
      oldAction->sa_handler = ::signal(sigNum, SIG_IGN);
      ::signal(sigNum, oldAction->sa_handler);
   }
   else
   {
      oldAction->sa_handler = ::signal(sigNum, action->sa_handler);
   }

   return (oldAction->sa_handler == SIG_ERR ? -1 : 0);
#endif
}

} // End of vpr namespace
