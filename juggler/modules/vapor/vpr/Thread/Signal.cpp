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

#include <vpr/vprConfig.h>

#include <signal.h>

#include <vpr/vprTypes.h>
#include <vpr/Util/Assert.h>
#include <vpr/Thread/Signal.h>


namespace vpr {

vpr::Status
SignalSet::emptySet () {
    vpr::Status status;

#ifdef HAVE_SIGEMPTYSET
    if ( sigemptyset(&m_sigset) != 0 ) {
        status.setCode(vpr::Status::Failure);
    }
#else
    m_sigset = 0;
#endif

    return status;
}

vpr::Status
SignalSet::fillSet () {
    vpr::Status status;

#ifdef HAVE_SIGFILLSET
    if ( sigfillset(&m_sigset) != 0 ) {
        status.setCode(vpr::Status::Failure);
    }
#else
    m_sigset = ~(sigset_t) 0;
#endif

    return status;
}

vpr::Status
SignalSet::addSignal (const int sig_num) {
    vpr::Status status;

    vprASSERT(sig_num >= 1 && "Invalid signal number");

#ifdef HAVE_SIGADDSET
    if ( sigaddset(&m_sigset, sig_num) != 0 ) {
        status.setCode(vpr::Status::Failure);
    }
#else
    m_sigset |= (1 << (sig_num - 1));
#endif

    return status;
}

vpr::Status
SignalSet::removeSignal (const int sig_num) {
    vpr::Status status;

    vprASSERT(sig_num >= 1 && "Invalid signal number");

#ifdef HAVE_SIGDELSET
    if ( sigdelset(&m_sigset, sig_num) != 0 ) {
        status.setCode(vpr::Status::Failure);
    }
#else
    m_sigset &= ~(1 << (sig_num - 1)) ;
#endif

    return status;
}

bool
SignalSet::isMember (const int sig_num) const {
    bool is_member;

    vprASSERT(sig_num >= 1 && "Invalid signal number");

#ifdef HAVE_SIGISMEMBER
    is_member = (sigismember(&m_sigset, sig_num) == 1);
#else
    is_member = ((m_sigset & (1 << (sig_num - 1))) != 0);
#endif

    return is_member;
}

SignalAction::SignalAction (vpr::SignalHandler_t handler,
                            const vpr::SignalSet* sig_set, const int flags)
{
    if ( sig_set == NULL ) {
        init(handler, NULL, flags);
    }
    else {
        init(handler, sig_set->getMask(), flags);
    }
}

vpr::Status
SigHandler::registerHandler (const int sig_num,
                             vpr::SignalHandler_t handler)
{
    vpr::SignalAction sa(handler);
    return registerHandler(sig_num, sa);
}

vpr::Status
SigHandler::registerHandler (const int sig_num,
                             vpr::SignalAction& action)
{
    vpr::Status status;

    if ( vpr::SigHandler::sigaction(sig_num, &action.m_sa) != 0 ) {
        status.setCode(vpr::Status::Failure);
    }

    return status;
}

int
SigHandler::sigaction (const int sig_num, const struct sigaction* action,
                       struct sigaction* old_action)
{
#ifdef HAVE_SIGACTION
    return ::sigaction(sig_num, action, old_action);
#else
    // If sigaction(2) is not available, simulate its functionality using
    // signal(2).
    struct sigaction sa;

    if ( old_action == NULL ) {
        old_action = &sa;
    }

    if ( action == NULL ) {
        old_action->sa_handler = ::signal(sig_num, SIG_IGN);
        ::signal(sig_num, old_action->sa_handler);
    }
    else {
        old_action->sa_handler = ::signal(sig_num, action->sa_handler);
    }

    return (old_action->sa_handler == SIG_ERR ? -1 : 0);
#endif
}

} // End of vpr namespace
