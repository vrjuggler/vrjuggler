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
   AppHolder(vrj::App* app, python::object srcObj, python::handle<> module,
             python::handle<> initFunc)
      : mApp(app), mSrcObj(srcObj), mModule(module), mInitFunc(initFunc)
   {
   }

   ~AppHolder()
   {
   }

   vrj::App* getApp()
   {
      return mApp;
   }

private:
   vrj::App*        mApp;
   python::object   mSrcObj;
   python::handle<> mModule;
   python::handle<> mInitFunc;
};


int main(int argc, char* argv[])
{
//- INITIALIZATION ------------------------------------------------------//

   // Initialize the interpreter.
   Py_Initialize();

   extern char* optarg;
   extern int optind;
   int ch;
   std::vector<std::string> module_names;

   while ( (ch = getopt(argc, argv, "c:m:")) != -1 )
   {
      switch (ch)
      {
         case 'c':
            vrj::Kernel::instance()->loadConfigFile(optarg);
            break;
         case 'm':
            module_names.push_back(std::string(optarg));
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

   for ( std::vector<std::string>::iterator i = module_names.begin();
         i != module_names.end();
         ++i )
   {
      std::cout << "Working on module named '" << *i << "'" << std::endl;

      try
      {
         python::handle<> cur_name(PyString_FromString((*i).c_str()));

         std::cout << "--- Attempting to import module" << std::endl;
         python::handle<> cur_module(PyImport_Import(cur_name.get()));

         std::cout << "--- Getting dictionary" << std::endl;
         python::handle<> dict(python::borrowed(PyModule_GetDict(cur_module.get())));

         std::cout << "--- Looking up vrjInit in dictionary" << std::endl;
         python::handle<> init_func(python::borrowed(PyDict_GetItemString(dict.get(), "vrjInit")));

         if ( PyCallable_Check(init_func.get()) )
         {
            std::cout << "--- Calling vrjInit" << std::endl;

            try
            {
               python::object app_obj =
                  python::call<python::object>(init_func.get());
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
               }
            }
            catch (...)
            {
               PyErr_Print();
               std::cerr << "ERROR: Caught exception trying to execute vrjInit()\n";
            }
         }
         else
         {
            PyErr_Print();
            std::cerr << "ERROR: Did not find function vrjInit\n";
         }
      }
      catch(python::error_already_set)
      {
         PyErr_Print();
      }
   }

   for ( unsigned int i = 0; i < apps.size() * 5; ++i )
   {
      vrj::Kernel::instance()->setApplication(apps[i].getApp());
      Py_BEGIN_ALLOW_THREADS;
         vpr::System::sleep(10);
      Py_END_ALLOW_THREADS;
   }

/*
   Py_BEGIN_ALLOW_THREADS;
      vrj::Kernel::instance()->waitForKernelStop();
   Py_END_ALLOW_THREADS;
*/

//- CLEAN UP ------------------------------------------------------------//

   apps.clear();

   // Release the Python interpreter
   Py_Finalize();

   return 0;
}
