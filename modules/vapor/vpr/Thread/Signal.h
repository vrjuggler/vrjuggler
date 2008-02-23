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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _VPR_SIGNAL_HANDLER_H_
#define _VPR_SIGNAL_HANDLER_H_

#include <vpr/vprConfig.h>

#include <cstdlib>
#include <cstring>

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#else
#error "I don't know what to do without signal.h!"
#endif


namespace vpr
{

// XXX: Use a functor for the real handler function?
#ifdef VPR_OS_Windows
typedef RETSIGTYPE (__cdecl *SignalHandler_t)(int);
#else
#if defined(VPR_OS_IRIX) && defined(__GNUC__) && \
    __GNUC__ == 3 && __GNUC_MINOR__ < 1
typedef RETSIGTYPE (*SignalHandler_t)();
#else
typedef RETSIGTYPE (*SignalHandler_t)(int);
#endif
#endif

}

#ifndef HAVE_SIGACTION
/** \struct sigaction Signal.h vpr/Thread/Signal.h
 *
 * This data structure is defined only on platforms that do not provide a
 * sigaction struct.
 */
struct sigaction
{
   int                  sa_flags;      /**< see signal options below */
   vpr::SignalHandler_t sa_handler;    /**< signal handler */
   sigset_t             sa_mask;       /**< signal mask to apply */
};
#endif

namespace vpr
{

/** \class SignalSet Signal.h vpr/Thread/Signal.h
 *
 * Wrapper class for a signal set.  A signal set contains the set of system
 * signals to be masked when used with a vpr::SignalAction.
 */
class VPR_CLASS_API SignalSet
{
public:
   /**
    * Initializes \c mSigSet with \p sigset.  If \p sigset == 0 then fill the
    * set.
    *
    * @param sigset The signal set used for initializing this object.
    */
   SignalSet(const sigset_t* const sigset)
   {
      if ( sigset == 0 )
      {
         fillSet();
      }
      else
      {
         mSigSet = *sigset;
      }
   }

   /**
    * If \p fill is \c false then initialize the \c mSigSet to be empty.
    * Otherwise, \c mSigSet is initialized to be full.
    *
    * @param fill A boolean flag stating whether to fill this object's
    *             signal set or make it empty.
    */
   SignalSet(bool fill = false)
   {
      if ( fill )
      {
         fillSet();
      }
      else
      {
         emptySet();
      }
   }

   /**
    * Creates a set that includes no signals defined by the system.
    *
    * @return \c true is returned if this object's signal set is emptied.
    *         \c false is returned otherwise.
    */
   bool emptySet();

   /**
    * Creates a set that includes all signals defined by the system.
    *
    * @return \c true is returned if this object's signal set is filled.
    *         \c false is returned otherwise.
    */
   bool fillSet();

   /**
    * Adds the individual signal specified by \p sigNum to the set.
    *
    * @param sigNum The signal number to be added.
    *
    * @return \c true is returned if the given signal is added to this
    *         object's signal set. \c false is returned otherwise.
    */
   bool addSignal(const int sigNum);

   /**
    * Deletes the individual signal specified by \p sigNum from the set.
    *
    * @param sigNum The signal number to be removed.
    *
    * @return \c true is returned if the given signal is removed from this
    *         object's signal set.  \c false is returned otherwise.
    */
   bool removeSignal(const int sigNum);

   /**
    * Checks whether the signal specified by \p sigNum is in the set.
    *
    * @param sigNum The signal number being checked.
    *
    * @return \c true is returned if \p sigNum is in this object's set.
    *         \c false is returned otherwise.
    */
   bool isMember(const int sigNum) const;

   /**
    * Returns a constant pointer to the underlying sigset_t variable.
    *
    * @return A constant pointer to this object's sigset_t variable.
    */
   const sigset_t* getMask() const
   {
      return &mSigSet;
   }

private:
   sigset_t mSigSet;
};

/** \class SignalAction Signal.h vpr/Thread/Signal.h
 *
 * Wrapper class for a signal action.  A signal action is used to encapsulate
 * the handler callback, the set of signals to be masked, and flags specifying
 * optional behavior when handling the signal.
 */
class VPR_CLASS_API SignalAction
{
public:
   /**
    * Constructs an object that will hold the given handler, the given mask,
    * and any optional flags set.
    *
    * @post The encapsulated mSA variable is fully initialized.
    *
    * @param handler The actual handler callback.  Set to SIG_IGN to ignore
    *                the signal with which this action object will be
    *                associated.
    * @param sig_set The signal set mask used to mask out signals we are not
    *                interested in.  This is an optional parameter and
    *                defaults to NULL (which implies that no signals will be
    *                masked).
    * @param flags   Options defining the behavior for handling the signal.
    *                This is an optional parameter and defaults to 0.
    */
   SignalAction(vpr::SignalHandler_t handler,
                const vpr::SignalSet* sig_set = NULL, const int flags = 0);

   static const vpr::SignalHandler_t DefaultAction; /**< Use default action */
   static const vpr::SignalHandler_t IgnoreAction;  /**< Ignore the signal */

private:
   /**
    * Default constructor.  We do not want this to be called.
    */
   SignalAction()
   {
      /* Do nothing. */ ;
   }

   /**
    * Initializes this object.
    *
    * @post The encapsulated mSA variable is fully initialized.
    *
    * @param handler The actual handler callback.  Set to SIG_IGN to ignore
    *                the signal with which this action object will be
    *                associated.
    * @param sig_set The signal set mask used to mask out signals we are not
    *                interested in.  If the mask is NULL, no mask is applied.
    * @param flags   Options defining the behavior for handling the signal.
    */
   void init(vpr::SignalHandler_t handler, const sigset_t* mask,
             const int flags)
   {
      std::memset(&mSA, '\0', sizeof(mSA));
      mSA.sa_handler = handler;
      mSA.sa_flags   = flags;

      if ( mask != NULL )
      {
         std::memcpy(&mSA.sa_mask, mask, sizeof(sigset_t));
      }
   }

public:
   struct sigaction mSA;
};

/** \class SigHandler Signal.h vpr/Thread/Signal.h
 *
 * This class wraps the calls needed to register signal handlers with the
 * operating system.  It currently has only static member functions and thus
 * is not really much of a class at all.
 */
class VPR_CLASS_API SigHandler
{
public:
   /**
    * Reigsters the given callback as a handler for the named signal.  No
    * mask is applied, and no option flags are set.  To get a mask and/or
    * option flags, use a vpr::SignalAction object.
    *
    * @post If \p sigNum is a valid signal number, handler is registered
    *       as a handler for that signal.
    *
    * @param sigNum  The signal to be handled.
    * @param handler The callback that does the signal handling.
    * @param restart Restart a system call interrupted by the named signal.
    *                This argument is optional and defaults to true.
    *                However, if \p sigNum is \c SIGALARM, the restart
    *                parameter is ignored and set to false.
    *
    * @return \c true is returned if the given handler is registered
    *         successfully; \c false otherwise.
    */
   static bool registerHandler(const int sigNum, vpr::SignalHandler_t handler,
                               const bool restart = true);

   /**
    * Reigsters the given callback as a handler for the named signal.  No
    * mask is applied, and no option flags are set.  To get a mask and/or
    * option flags, use a vpr::SignalAction object.
    *
    * @pre  action is a reference to a valid vpr::SignalAction object.
    * @post If \p sigNum is a valid signal number, action is registered
    *       as the action to be taken when that signal is raised.
    *
    * @param sigNum  The signal to be handled.
    * @param action  A vpr::SignalAction object encapsulating all the
    *                information needed to handle the named signal.
    * @param restart Restart a system call interrupted by the named signal.
    *                This argument is optional and defaults to true.
    *                However, if \p sigNum is \c SIGALRM, the restart parameter
    *                is ignored and set to false.
    *
    * @return \c true is returned if the given action is registered
    *         successfully; \c false otherwise.
    */
   static bool registerHandler(const int sigNum, vpr::SignalAction& action,
                               const bool restart = true);

protected:
   /**
    * Default constructor.  It is protected because there is no need for this
    * class to be instantiated at this point.
    */
   SigHandler()
   {
      /* Do nothing. */ ;
   }

   /**
    * Wrapper around the system call sigaction(2).
    *
    * @pre  action points to a valid sigaction struct or is NULL.
    * @post If \p sigNum is a valid signal number, action is registered as
    *       the action to be taken when that signal is raised.
    *
    * @param sigNum The signal to be handled.
    * @param action A vpr::SignalAction object encapsulating all the
    *               information needed to handle the named signal.  If
    *               action is NULL, the signal will be ignored.
    * @param oldAct Storage for the previous action registered for the given
    *               signal number.  If it is NULL, the previous action is
    *               discarded.
    *
    * @return 0 is returned on successful action registration; -1 is returned
    *         otherwise.
    */
   static int sigaction(const int sigNum, const struct sigaction* action,
                        struct sigaction* oldAct = NULL);
};

} // End of vpr namespace


#endif /* _VPR_SIGNAL_HANDLER_H_ */
