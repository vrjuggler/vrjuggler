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

#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <boost/python.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/converter/registry.hpp>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Kernel/App.h>


namespace python = boost::python;

class AppHolder
{
public:
   AppHolder(vrj::App* app, python::object srcObj, PyObject* module,
             PyObject* initFunc)
      : mApp(app), mSrcObj(srcObj), mModule(module), mInitFunc(initFunc)
   {
   }

   ~AppHolder()
   {
      Py_DECREF(mInitFunc);
      Py_DECREF(mModule);
   }

   vrj::App* getApp()
   {
      return mApp;
   }

private:
   vrj::App*      mApp;
   python::object mSrcObj;
   PyObject*      mModule;
   PyObject*      mInitFunc;
};


int main(int argc, char* argv[])
{
//- INITIALIZATION ------------------------------------------------------//

   // Initialize the interpreter.
   Py_Initialize();

   extern char* optarg;
   extern int optind;
   int ch;
   std::vector<std::string> script_names;

   while ( (ch = getopt(argc, argv, "c:s:")) != -1 )
   {
      switch (ch)
      {
         case 'c':
            vrj::Kernel::instance()->loadConfigFile(optarg);
            break;
         case 's':
            script_names.push_back(std::string(optarg));
            break;
         default:
//            usage();
            break;
      }
   }

   argc -= optind;
   argv += optind;

   vrj::Kernel::instance()->start();

   std::vector<AppHolder> apps;

   for ( std::vector<std::string>::iterator i = script_names.begin();
         i != script_names.end();
         ++i )
   {
      std::cout << "Working on module named '" << *i << "'" << std::endl;

      PyObject* cur_name = PyString_FromString((*i).c_str());

      if ( NULL != cur_name )
      {
         std::cout << "--- Attempting to import module" << std::endl;

         PyObject* cur_module = PyImport_Import(cur_name);

         if ( NULL != cur_module )
         {
            std::cout << "--- Getting dictionary" << std::endl;

            PyObject* dict = PyModule_GetDict(cur_module);

            std::cout << "--- Looking up vrjInit in dictionary" << std::endl;

            PyObject* init_func = PyDict_GetItemString(dict, "vrjInit");

            if ( NULL != init_func && PyCallable_Check(init_func) )
            {
               std::cout << "--- Calling vrjInit" << std::endl;

               try
               {
                  python::object app_obj =
                     python::call<python::object>(init_func);
                  vrj::App* cur_app = python::extract<vrj::App*>(app_obj);

                  if ( NULL != cur_app )
                  {
                     std::cout << "--- Storing object reference" << std::endl;

                     apps.push_back(AppHolder(cur_app, app_obj, cur_module,
                                              init_func));
                  }
                  else
                  {
                     PyErr_Print();
                     std::cerr << "ERROR: Could not get application reference\n";
                     Py_DECREF(init_func);
                     Py_DECREF(cur_module);
                  }
               }
               catch (...)
               {
                  PyErr_Print();
                  std::cerr << "ERROR: Caught exception trying to execute vrjInit()\n";
                  Py_DECREF(init_func);
                  Py_DECREF(cur_module);
               }
            }
            else
            {
               PyErr_Print();
               std::cerr << "ERROR: Did not find function vrjInit\n";
               Py_DECREF(cur_module);
            }
         }
         else
         {
            PyErr_Print();
            std::cerr << "WARNING: Could not import module named '" << *i
                      << "'\n";
         }

         Py_DECREF(cur_name);
      }
      else
      {
         PyErr_Print();
         std::cerr << "Danger!\n";
      }
   }

   for ( unsigned int i = 0; i < apps.size() * 5; ++i )
   {
      vrj::Kernel::instance()->setApplication(apps[i].getApp());
      vpr::System::sleep(10);
   }
//   vrj::Kernel::instance()->waitForKernelStop();

//- CLEAN UP ------------------------------------------------------------//

   apps.clear();

   // Release the Python interpreter
   Py_Finalize();

   return 0;
}
