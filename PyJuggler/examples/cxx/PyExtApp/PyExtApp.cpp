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

#include <boost/python/extract.hpp>
#include <boost/python/converter/registry.hpp>

#include <vpr/Util/Debug.h>
#include <gmtl/Vec.h>
#include <gmtl/Generate.h>

#include <PyExtApp.h>


namespace python = boost::python;

PyExtApp::~PyExtApp()
{
}

void PyExtApp::init()
{
   mButton0.init("VJButton0");
   mButton1.init("VJButton1");
   mButton2.init("VJButton2");
   mWand.init("VJWand");

   // We put initPython() in a try/catch block just to be safe.  We do not
   // want to crash VR Juggler after all.
   try
   {
      initPython();
   }
   catch(...)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutNORM(clrYELLOW, "WARNING")
         << ": Caught exception trying to initialize Python extension\n"
         << vprDEBUG_FLUSH;
   }
}

void PyExtApp::contextInit()
{
   initGLState();
}

void PyExtApp::preFrame()
{
   if ( NULL != mPythonFunc )
   {
      gmtl::Vec3f wand_pos(gmtl::makeTrans<gmtl::Vec3f>(mWand->getData()));

      try
      {
         python::call<void, gmtl::Vec3f>(mPythonFunc.get(), wand_pos);
      }
      catch(...)
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "ERROR")
            << ": Caught exception trying to call mPythonFunc\n"
            << vprDEBUG_FLUSH;
      }
   }
}

void PyExtApp::bufferPreDraw()
{
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT);
}

void PyExtApp::draw()
{
   glClear(GL_DEPTH_BUFFER_BIT);

   // Render something interesting ...
}

void PyExtApp::initPython()
{
   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_STATE_LVL,
                        "PyExtApp::initPython() entered\n",
                        "PyExtApp::initPython() done.\n");

   try
   {
      // First, we need to get the module name in a form that Python/C likes.
      python::handle<> py_module_name(PyString_FromString(mModuleName.c_str()));

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "Loading module named '" << mModuleName << "'\n" << vprDEBUG_FLUSH;

      // Next, we import the Python module into the Python interpreter.
      mModule =
         python::handle<>(python::borrowed(PyImport_Import(py_module_name.get())));

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "Getting dictionary\n"
                                                << vprDEBUG_FLUSH;

      // Next, we get the module's dictionary so we can look up functions
      // and such.
      mModuleDict =
         python::handle<>(python::borrowed(PyModule_GetDict(mModule.get())));

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "Looking up myFunc in dictionary\n" << vprDEBUG_FLUSH;

      // Now we find the entry point function (arbitrarily) called "myFunc."
      mPythonFunc =
         python::handle<>(python::borrowed(PyDict_GetItemString(mModuleDict.get(),
                                                                "myFunc")));

      // Test the results of the dictionary lookup.  If the dictionary
      // lookup failed, then mPythonFunc is not usable.
      if ( ! PyCallable_Check(mPythonFunc.get()) )
      {
         PyErr_Print();
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "ERROR")
            << ": Did not find function myFunc\n" << vprDEBUG_FLUSH;
      }
   }
   catch(python::error_already_set)
   {
      PyErr_Print();
   }
}

void PyExtApp::initGLState()
{
   GLfloat light0_ambient[]   = { 0.2f, 0.2f, 0.2f, 1.0f };
   GLfloat light0_diffuse[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat light0_specular[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat light0_position[]  = { 2000.0f, 1000.0f, 100.0f, 1.0f };
   GLfloat light0_direction[] = { -100.0f, -100.0f, -100.0f };

   GLfloat mat_ambient[]   = { 0.7f, 0.7f, 0.7f, 1.0f };
   GLfloat mat_diffuse[]   = { 1.0f, 0.5f, 0.8f, 1.0f };
   GLfloat mat_specular[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat mat_shininess[] = { 50.0f };
   GLfloat mat_emission[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat no_mat[]        = { 0.0f, 0.0f, 0.0f, 1.0f };

   glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);

   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
//   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   glMatrixMode(GL_MODELVIEW);
}
