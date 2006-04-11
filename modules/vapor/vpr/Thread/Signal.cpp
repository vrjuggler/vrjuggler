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

#include <vpr/vprConfig.h>

#include <signal.h>

#include <vpr/vprTypes.h>
#include <vpr/Util/Assert.h>
#include <vpr/Thread/Signal.h>


namespace vpr
{

// ============================================================================
// vpr::SignalSet stuff.
// ============================================================================

vpr::ReturnStatus SignalSet::emptySet ()
{
   vpr::ReturnStatus status;

#ifdef HAVE_SIGEMPTYSET
   if ( sigemptyset(&mSigSet) != 0 )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }
#else
   mSigSet = 0;
#endif

   return status;
}

vpr::ReturnStatus SignalSet::fillSet ()
{
   vpr::ReturnStatus status;

#ifdef HAVE_SIGFILLSET
   if ( sigfillset(&mSigSet) != 0 ) {
      status.setCode(vpr::ReturnStatus::Fail);
   }
#else
   mSigSet = ~(sigset_t) 0;
#endif

   return status;
}

vpr::ReturnStatus SignalSet::addSignal (const int sig_num)
{
   vpr::ReturnStatus status;

   vprASSERT(sig_num >= 1 && "Invalid signal number");

#ifdef HAVE_SIGADDSET
   if ( sigaddset(&mSigSet, sig_num) != 0 )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }
#else
   mSigSet |= (1 << (sig_num - 1));
#endif

   return status;
}

vpr::ReturnStatus SignalSet::removeSignal (const int sig_num)
{
   vpr::ReturnStatus status;

   vprASSERT(sig_num >= 1 && "Invalid signal number");

#ifdef HAVE_SIGDELSET
   if ( sigdelset(&mSigSet, sig_num) != 0 ) {
      status.setCode(vpr::ReturnStatus::Fail);
   }
#else
   mSigSet &= ~(1 << (sig_num - 1)) ;
#endif

   return status;
}

bool SignalSet::isMember (const int sig_num) const
{
   bool is_member;

   vprASSERT(sig_num >= 1 && "Invalid signal number");

#ifdef HAVE_SIGISMEMBER
   is_member = (sigismember(&mSigSet, sig_num) == 1);
#else
   is_member = ((mSigSet & (1 << (sig_num - 1))) != 0);
#endif

   return is_member;
}

// ============================================================================
// vpr::SignalAction stuff.
// ============================================================================

const vpr::SignalHandler_t SignalAction::DefaultAction = SIG_DFL;
const vpr::SignalHandler_t SignalAction::IgnoreAction  = SIG_IGN;

SignalAction::SignalAction (vpr::SignalHandler_t handler,
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

vpr::ReturnStatus SigHandler::registerHandler (const int sig_num,
                                               vpr::SignalHandler_t handler,
                                               const bool restart)
{
   vpr::SignalAction sa(handler);
   return registerHandler(sig_num, sa);
}

vpr::ReturnStatus SigHandler::registerHandler (const int sig_num,
                                               vpr::SignalAction& action,
                                               const bool restart)
{
   vpr::ReturnStatus status;

#ifdef SA_RESTART
   if ( sig_num != SIGALRM && restart )
   {
      action.mSA.sa_flags |= SA_RESTART;
   }
#endif

   if ( vpr::SigHandler::sigaction(sig_num, &action.mSA) != 0 )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

int SigHandler::sigaction (const int sig_num, const struct sigaction* action,
                           struct sigaction* old_action)
{
#ifdef HAVE_SIGACTION
   return ::sigaction(sig_num, action, old_action);
#else
   // If sigaction(2) is not available, simulate its functionality using
   // signal(2).
   struct sigaction sa;

   if ( old_action == NULL )
   {
      old_action = &sa;
   }

   if ( action == NULL )
   {
      old_action->sa_handler = ::signal(sig_num, SIG_IGN);
      ::signal(sig_num, old_action->sa_handler);
   }
   else
   {
      old_action->sa_handler = ::signal(sig_num, action->sa_handler);
   }

   return (old_action->sa_handler == SIG_ERR ? -1 : 0);
#endif
}

} // End of vpr namespace
