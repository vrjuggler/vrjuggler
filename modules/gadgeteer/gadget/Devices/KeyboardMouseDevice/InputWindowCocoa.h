/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _GADGET_INPUT_WINDOW_COCOA_H_
#define _GADGET_INPUT_WINDOW_COCOA_H_

#include <gadget/gadgetConfig.h>

#include <string>

#include <gadget/Type/Input.h>
#include <gadget/Devices/KeyboardMouseDevice/InputAreaCocoa.h>


namespace gadget
{

/**
 * A specialization of gadget::InputAreaCocoa that opens an NSWindow to
 * receive keyboard and mouse events as input from the user.
 *
 * @since 1.1.25
 */
class InputWindowCocoa
   : public gadget::InputAreaCocoa
   , public gadget::Input
{
public:
   InputWindowCocoa();

   virtual ~InputWindowCocoa();

   static std::string getElementType();

   virtual bool config(jccl::ConfigElementPtr e);

   /**
    * Does nothing.
    *
    * @return \c true is always returned.
    */
   virtual bool sample();

   /**
    * @pre An autorelease pool has been created for this thread.
    */
   virtual bool startSampling();

   /**
    * Closes the window and decrements the reference counts for
    * \c mCocoaWindow and \c mMainView. If \c mThread is not NULL, the object
    * to which it points is deleted, and the variable is set to NULL.
    *
    * @post \c mCocoaWindow and \c mMainView are nil. \c mThread is NULL.
    *
    * @return \c true is always returned.
    */
   virtual bool stopSampling();

   /**
    * Does nothing.
    */
   virtual void updateData();

   /**
    * Informs this object that the NSWindow object that we created previously
    * has been closed by an external entity and hence deallocated. By invoking
    * this method with the \p isOpen parameter set to false, the caller is
    * telling this object that it no longer has an NSWindow object and that it
    * is basically defunct.
    *
    * @post \c mWindowOpen is set to the value of \c isOpen.
    */
   void setWindowOpen(const bool isOpen);

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   void destroy()
   {
      delete this;
   }

private:
   /**
    * Waits until it is safe to open a window and then does so. As soon as
    * the window is open, the method exits.
    *
    * @note This blocks on the static member \c sWinOpenLock from
    *       gadget::InputAreaCocoa.
    */
   void waitAndOpen();

   /**
    * Opens the window.
    *
    * @post \c mWindowOpen is set to \c true.
    */
   void finishOpen();

   unsigned int mScreen;
   bool mWindowOpen;    /**< Keeps track of NSWindow open/closed state */
};

}


#endif /* _GADGET_INPUT_WINDOW_COCOA_H_ */
