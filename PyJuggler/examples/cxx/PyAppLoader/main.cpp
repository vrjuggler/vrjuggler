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

#include <iostream>
#include <stdlib.h>

#ifdef WIN32
#  include <string.h>
#else
#  include <unistd.h>
#endif

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

// This class is important for keeping hold of all the reference-counted
// memory associated with a given Python application object.
class AppHolder
{
public:
   AppHolder(vrj::App* app, python::object srcObj, python::handle<> module,
             python::object appClassk)
      : mApp(app), mSrcObj(srcObj), mModule(module), mAppClass(appClassk)
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
   python::object   mAppClass;
};


static void usage(const char* appName)
{
   std::cerr << "Usage:\n   "
             << appName << " -c <VR Juggler config file> -m <Python module>\n";
   std::cerr << "\tThere may be one or more VR Juggler config files and one\n"
             << "\tor more Python modules given by repeating the options.\n";
}

int main(int argc, char* argv[])
{
//- INITIALIZATION ------------------------------------------------------//

   // Initialize the interpreter.
   Py_Initialize();

   std::vector<std::string> module_names;
   bool cfg_file_given(false), module_given(false);

#ifdef WIN32
   int i, optind;

   for ( i = 1; i < argc; ++i )
   {
      if ( strcmp(argv[i], "-c") == 0 )
      {
         vrj::Kernel::instance()->loadConfigFile(argv[i + i]);
         cfg_file_given = true;
         ++i;
      }
      else if ( strcmp(argv[i], "-h") == 0 )
      {
         usage(argv[0]);
      }
      else if ( strcmp(argv[i], "-m") == 0 )
      {
         module_names.push_back(std::string(argv[i + i]));
         module_given = true;
         ++i;
      }
   }

   optind = i;
#else
   extern char* optarg;
   extern int optind;
   int ch;

   while ( (ch = getopt(argc, argv, "c:hm:")) != -1 )
   {
      switch (ch)
      {
         case 'c':
            vrj::Kernel::instance()->loadConfigFile(optarg);
            cfg_file_given = true;
            break;
         case 'm':
            module_names.push_back(std::string(optarg));
            module_given = true;
            break;
         case 'h':
         default:
            usage(argv[0]);
            break;
      }
   }
#endif

   if ( argc < 5 || ! (cfg_file_given && module_given) )
   {
      usage(argv[0]);
      exit(1);
   }

   argc -= optind;
   argv += optind;

   vrj::Kernel::instance()->start();

   std::vector<AppHolder> apps;

   for ( std::vector<std::string>::iterator i = module_names.begin();
         i != module_names.end();
         ++i )
   {
      std::string::size_type sep((*i).rfind("."));
      const std::string mod_name((*i).substr(0, sep));
      const std::string class_name((*i).substr(sep + 1, (*i).length()));
      
      std::cout << "Working on module named '" << mod_name << "'" << std::endl;

      try
      {
         python::handle<> cur_name(PyString_FromString(mod_name.c_str()));

         std::cout << "--- Attempting to import module" << std::endl;
         python::handle<> cur_module(PyImport_Import(cur_name.get()));

         std::cout << "--- Getting dictionary" << std::endl;
         python::object dict(python::borrowed(PyModule_GetDict(cur_module.get())));

         std::cout << "--- Looking up '" << class_name << "' in dictionary"
                   << std::endl;

         // Look up the class name in the dictionary, get an instance of the
         // class, and convert it to a vrj::App*.
         // Isn't this cool?!?  The answer is: yes.  :)
         python::object app_class = dict[class_name];
         python::object app_obj = app_class();
         vrj::App* cur_app = python::extract<vrj::App*>(app_obj);

         if ( NULL != cur_app )
         {
            std::cout << "--- Storing object reference" << std::endl;
            apps.push_back(AppHolder(cur_app, app_obj, cur_module,
                                     app_class));
         }
         else
         {
            PyErr_Print();
            std::cerr << "ERROR: Could not get application reference\n";
         }
      }
      catch(python::error_already_set)
      {
         PyErr_Print();
      }
   }

   for ( unsigned int count = 0; count < apps.size() * 5; ++count )
   {
      for ( unsigned int i = 0; i < apps.size(); ++i )
      {
         vrj::Kernel::instance()->setApplication(apps[i].getApp());
         Py_BEGIN_ALLOW_THREADS;
            vpr::System::sleep(10);
         Py_END_ALLOW_THREADS;
      }
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
