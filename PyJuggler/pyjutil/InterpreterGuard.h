// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _PYJUGGLER_INTERPRETER_GUARD_H_
#define _PYJUGGLER_INTERPRETER_GUARD_H_

#include <Python.h>

#include <vpr/vpr.h>
#include <vpr/Thread/TSObjectProxy.h>


namespace PyJuggler
{

/**
 * Helper class for dealing with the ins and outs of multi-threaded C++ code
 * calling into the Python interpreter.  This class is designed for a very
 * specific purpose: an instance of PyJuggler::InterpreterGuard should exist
 * on the stack prior to any calls into the Python interpreter via Python/C
 * (or Boost.Python) when the calling thread is different from the thread
 * that started the interpreter.  Recursive locks (of a fashion) are allowed
 * by this class.  More specifically, it will not allow the same thread to
 * acquire the Global Interpreter Lock twice and in so doing prevents
 * deadlock.
 */
class InterpreterGuard
{
private:
   /**
    * One instance of this class should exist for each thread that may call
    * through to the Python interpreter.  A thread-specific data mechanism is
    * needed to manage which instance is used for a given thread.
    */
   struct State
   {
      State();

      ~State();

      bool           gilLocked; /**< Indicates that the GIL is held. */
      PyThreadState* pyState;   /**< The Python state info for this thread. */
   };

public:
   /**
    * Acquires the Python Global Interpreter Lock (GIL) for the invoking
    * thread.  If that thread already holds the GIL, then no action is taken.
    */
   InterpreterGuard();

   /**
    * Releases the Python Global Interpreter Lock (GIL) for the invoking thread
    * iff that thread holds the GIL and the constructor acquired the GIL.
    */
   ~InterpreterGuard();

private:
   /**
    * Thread-specific data structure for handling the Python thread data
    * information.  This is used to ensure that a single thread never locks
    * the GIL twice.
    */
   static vpr::TSObjectProxy<State> mState;

   /** Prevent copying. */
   InterpreterGuard(const InterpreterGuard& o)
   {
      /* Do nothing. */ ;
   }

   void operator=(const InterpreterGuard& o)
   {
      /* Do nothing. */ ;
   }

   /** Indicates that this object was the one that acquired the GIL. */
   bool mMyLock;
};

} // End of PyJuggler namespace


#endif /* _PYJUGGLER_INTERPRETER_GUARD_H_ */
