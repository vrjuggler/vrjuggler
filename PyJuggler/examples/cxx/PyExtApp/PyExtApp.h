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

#ifndef _PY_EXT_APP_H_
#define _PY_EXT_APP_H_

#include <stdlib.h>
#include <string>
#include <boost/python.hpp>

#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/PositionInterface.h>
#include <vrj/Draw/OGL/GlApp.h>


// Simplify (visually) calls to Boost.Python.
namespace python = boost::python;

class PyExtApp : public vrj::GlApp
{
public:
   PyExtApp(const std::string& moduleName)
      : vrj::GlApp(), mModuleName(moduleName), mModule(),
        mModuleDict(), mPythonFunc()
   {
      /* Do nothing. */ ;
   }

   virtual ~PyExtApp();

   /**
    * Initializes the Gadgeteer device interfaces and the Python extension.
    */
   virtual void init();

   virtual void preFrame();

   virtual void contextInit();

   virtual void bufferPreDraw();

   /** Renders the scene. */
   virtual void draw();

private:
   /**
    * Initializes the Python extension to this application.
    *
    * @post If a Python module is available, mModule will point to it, and
    *       mPythonFunc will be a handle to the function contained therein
    *       named myFunc.
    */
   void initPython();

   void initGLState();

   std::string      mModuleName; /**< The name of the Python module we can use. */
   python::handle<> mModule;     /**< The handle to the loaded module. */
   python::handle<> mModuleDict; /**< The handle to the module's dictionary. */
   python::handle<> mPythonFunc; /**< The handle to the function myFunc. */

   gadget::DigitalInterface  mButton0; /**< Button 0 interface. */
   gadget::DigitalInterface  mButton1; /**< Button 1 interface. */
   gadget::DigitalInterface  mButton2; /**< Button 2 interface. */
   gadget::PositionInterface mWand;    /**< Wand interface. */
};


#endif /* _PY_EXT_APP_H_ */
