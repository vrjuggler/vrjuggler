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

#include <boost/python.hpp>

// GMTL classes
#include <gmtl/Matrix.h>
#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#include <gmtl/Math.h>

// Gadgeteer classes.
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/GestureInterface.h>
#include <gadget/Type/GloveInterface.h>

// VR Juggler classes.
#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OGL/GlApp.h>


using namespace boost::python;

namespace
{

vrj::Kernel* getKernel()
{
   return vrj::Kernel::instance();
}

struct KernelWrap
{
   vrj::Kernel* getInstance()
   {
      return vrj::Kernel::instance();
   }
};

struct AppWrap : public vrj::App
{
   AppWrap(PyObject* self) : mSelf(self)
   {
   }


   AppWrap(PyObject* self, vrj::Kernel* kernel)
      : vrj::App(kernel), mSelf(self)
   {
   }

   virtual void init()
   {
      call_method<void>(mSelf, "init");
   }

   static void default_init(AppWrap& self)
   {
      self.vrj::App::init();
   }

   virtual void apiInit()
   {
      call_method<void>(mSelf, "apiInit");
   }

   static void default_apiInit(AppWrap& self)
   {
      self.vrj::App::apiInit();
   }

   virtual void preFrame()
   {
      call_method<void>(mSelf, "preFrame");
   }

   static void default_preFrame(AppWrap& self)
   {
      self.vrj::App::preFrame();
   }

   virtual void intraFrame()
   {
      call_method<void>(mSelf, "intraFrame");
   }

   static void default_intraFrame(AppWrap& self)
   {
      self.vrj::App::intraFrame();
   }

   virtual void postFrame()
   {
      call_method<void>(mSelf, "postFrame");
   }

   static void default_postFrame(AppWrap& self)
   {
      self.vrj::App::postFrame();
   }

   virtual void exit()
   {
      call_method<void>(mSelf, "exit");
   }

   static void default_exit(vrj::App& self)
   {
      self.vrj::App::exit();
   }

   static void default_focusChanged(vrj::App& self)
   {
      self.vrj::App::focusChanged();
   }

   virtual void reset()
   {
      call_method<void>(mSelf, "reset");
   }

   static void default_reset(vrj::App& self)
   {
      self.vrj::App::reset();
   }

   virtual bool configCanHandle(jccl::ConfigChunkPtr chunk)
   {
      return call_method<bool>(mSelf, "configCanHandle", chunk);
   }

   static bool default_configCanHandle(vrj::App& self,
                                       jccl::ConfigChunkPtr chunk)
   {
      return self.vrj::App::configCanHandle(chunk);
   }

   vrj::DrawManager* getDrawManager()
   {
      call_method<vrj::DrawManager*>(mSelf, "getDrawManager");
   }

   PyObject* mSelf;
};

struct GlAppWrap : public vrj::GlApp
{
   GlAppWrap(PyObject* self) : vrj::GlApp(), mSelf(self)
   {
   }

   GlAppWrap(PyObject* self, vrj::Kernel* kernel)
      : vrj::GlApp(kernel), mSelf(self)
   {
   }

   virtual ~GlAppWrap()
   {
   }

   virtual void init()
   {
      call_method<void>(mSelf, "init");
   }

   static void default_init(vrj::GlApp& self)
   {
      self.vrj::GlApp::init();
   }

   virtual void apiInit()
   {
      call_method<void>(mSelf, "apiInit");
   }

   static void default_apiInit(vrj::GlApp& self)
   {
      self.vrj::GlApp::apiInit();
   }

   virtual void preFrame()
   {
      call_method<void>(mSelf, "preFrame");
   }

   static void default_preFrame(vrj::GlApp& self)
   {
      self.vrj::GlApp::preFrame();
   }

   virtual void intraFrame()
   {
      call_method<void>(mSelf, "intraFrame");
   }

   static void default_intraFrame(vrj::GlApp& self)
   {
      self.vrj::GlApp::intraFrame();
   }

   virtual void postFrame()
   {
      call_method<void>(mSelf, "postFrame");
   }

   static void default_postFrame(vrj::GlApp& self)
   {
      self.vrj::GlApp::postFrame();
   }

   virtual void exit()
   {
      call_method<void>(mSelf, "exit");
   }

   static void default_exit(vrj::GlApp& self)
   {
      self.vrj::GlApp::exit();
   }

   virtual void focusChanged()
   {
      call_method<void>(mSelf, "focusChanged");
   }

   static void default_focusChanged(vrj::GlApp& self)
   {
      self.vrj::App::focusChanged();
   }

   virtual void reset()
   {
      call_method<void>(mSelf, "reset");
   }

   static void default_reset(vrj::GlApp& self)
   {
      self.vrj::GlApp::reset();
   }

   virtual bool configCanHandle(jccl::ConfigChunkPtr chunk)
   {
      return call_method<bool>(mSelf, "configCanHandle", chunk);
   }

   static bool default_configCanHandle(vrj::GlApp& self,
                                       jccl::ConfigChunkPtr chunk)
   {
      return self.vrj::App::configCanHandle(chunk);
   }

   virtual void bufferPreDraw()
   {
      call_method<void>(mSelf, "bufferPreDraw");
   }

   static void default_bufferPreDraw(vrj::GlApp& self)
   {
      self.vrj::GlApp::bufferPreDraw();
   }

   virtual void contextInit()
   {
      call_method<void>(mSelf, "contextInit");
   }

   static void default_contextInit(vrj::GlApp& self)
   {
      self.vrj::GlApp::contextInit();
   }

   virtual void contextClose()
   {
      call_method<void>(mSelf, "contextClose");
   }

   static void default_contextClose(vrj::GlApp& self)
   {
      self.vrj::GlApp::contextClose();
   }

   virtual void contextPreDraw()
   {
      call_method<void>(mSelf, "contextPreDraw");
   }

   static void default_contextPreDraw(vrj::GlApp& self)
   {
      self.vrj::GlApp::contextPreDraw();
   }

   virtual void draw()
   {
      call_method<void>(mSelf, "draw");
   }

   virtual void contextPostDraw()
   {
      call_method<void>(mSelf, "contextPostDraw");
   }

   static void default_contextPostDraw(vrj::GlApp& self)
   {
      self.vrj::GlApp::contextPostDraw();
   }

   PyObject* mSelf;
};

struct AnalogInterfaceWrap
{
   void init(const std::string& name)
   {
      mIf.init(name);
   }

   float getData()
   {
      return mIf->getData();
   }

   gadget::AnalogInterface mIf;
};

struct DigitalInterfaceWrap
{
   void init(const std::string& name)
   {
      mIf.init(name);
   }

   int getData()
   {
      return mIf->getData();
   }

   gadget::DigitalInterface mIf;
};

struct PositionInterfaceWrap
{
   void init(const std::string& name)
   {
      mIf.init(name);
   }

   gmtl::Matrix44f getData()
   {
      return *(mIf->getData());
   }

   gadget::PositionInterface mIf;
};

struct Dummy
{
};

}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(loadConfigFile_overloads,
                                       vrj::Kernel::loadConfigFile, 1, 1)

BOOST_PYTHON_FUNCTION_OVERLOADS(deg2Rad_overloads,
                                gmtl::Math::deg2Rad, 1, 1)

BOOST_PYTHON_MODULE(PyJuggler)
{
   // XXX: This kind of sucks.
   class_<KernelWrap>("KernelWrap")
      .def("getInstance", &KernelWrap::getInstance,
           return_internal_reference<>())
   ;
//   def("getKernel", getKernel, return_internal_reference<>());

   class_<vrj::Kernel, boost::noncopyable>("Kernel", no_init)
      .def("start", &vrj::Kernel::start)
      .def("setApplication", &vrj::Kernel::setApplication)
      .def("loadConfigFile", (void(vrj::Kernel::*) (const char*)) 0,
           loadConfigFile_overloads())
      .def("loadConfigFile", (void(vrj::Kernel::*) (std::string)) 0,
           loadConfigFile_overloads())
      .def("loadChunkDescFile", &vrj::Kernel::loadChunkDescFile)
      .def("waitForKernelStop", &vrj::Kernel::waitForKernelStop)
      .def("getUser", &vrj::Kernel::getUser, return_internal_reference<>())
   ;

   class_<vrj::App, AppWrap, boost::noncopyable>("App", init<>())
      .def(init<vrj::Kernel*>())
      .def("init", &AppWrap::default_init)
      .def("apiInit", &AppWrap::default_apiInit)
      .def("preFrame", &AppWrap::default_preFrame)
      .def("intraFrame", &AppWrap::default_intraFrame)
      .def("postFrame", &AppWrap::default_postFrame)
      .def("exit", &AppWrap::default_exit)
      .def("haveFocus", &vrj::App::haveFocus)
      .def("focusChanged", &AppWrap::default_focusChanged)
      .def("reset", &AppWrap::default_reset)
      .def("configCanHandle", &AppWrap::default_configCanHandle)
      .def_readonly("mKernel", &vrj::App::mKernel)
      .def_readonly("mHaveFocus", &vrj::App::mHaveFocus)
   ;

   class_<vrj::GlApp, bases<vrj::App>, GlAppWrap, boost::noncopyable>("GlApp", init<>())
      .def(init<vrj::Kernel*>())
      .def("init", &GlAppWrap::default_init)
      .def("apiInit", &GlAppWrap::default_apiInit)
      .def("preFrame", &GlAppWrap::default_preFrame)
      .def("intraFrame", &GlAppWrap::default_intraFrame)
      .def("postFrame", &GlAppWrap::default_postFrame)
      .def("exit", &GlAppWrap::default_exit)
      .def("haveFocus", &vrj::GlApp::haveFocus)
      .def("focusChanged", &GlAppWrap::default_focusChanged)
      .def("reset", &GlAppWrap::default_reset)
      .def("configCanHandle", &GlAppWrap::default_configCanHandle)
      .def("contextInit", &GlAppWrap::default_contextInit)
      .def("contextClose", &GlAppWrap::default_contextClose)
      .def("contextPreDraw", &GlAppWrap::default_contextPreDraw)
      .def("contextPostDraw", &GlAppWrap::default_contextPostDraw)
      .def("bufferPreDraw", &GlAppWrap::default_bufferPreDraw)
   ;

// Gadgeteer classes ==========================================================
   class_<AnalogInterfaceWrap, boost::noncopyable>("AnalogInterface")
      .def("init", &AnalogInterfaceWrap::init)
      .def("getData", &AnalogInterfaceWrap::getData)
   ;

   class_<DigitalInterfaceWrap, boost::noncopyable>("DigitalInterface")
      .def("init", &DigitalInterfaceWrap::init)
      .def("getData", &DigitalInterfaceWrap::getData)
   ;

   enum_<gadget::Digital::State>("DigitalState")
      .value("OFF", gadget::Digital::OFF)
      .value("ON", gadget::Digital::ON)
      .value("TOGGLE_ON", gadget::Digital::TOGGLE_ON)
      .value("TOGGLE_OFF", gadget::Digital::TOGGLE_OFF)
   ;

   class_<PositionInterfaceWrap, boost::noncopyable>("PositionInterface")
      .def("init", &PositionInterfaceWrap::init)
      .def("getData", &PositionInterfaceWrap::getData)
   ;

   class_<gadget::GloveInterface, boost::noncopyable>("GloveInterface")
      .def("init", &gadget::GloveInterface::init)
   ;

   class_<gadget::GestureInterface, boost::noncopyable>("GestureInterface")
      .def("init", &gadget::GestureInterface::init)
   ;
// ========================================================== Gadgeteer classes

// JCCL classes ===============================================================
   class_<jccl::ConfigChunk, jccl::ConfigChunkPtr>("ConfigChunk", no_init)
      .def("getName", &jccl::ConfigChunk::getName)
      .def("getNum", &jccl::ConfigChunk::getNum)
      .def("getFullName", &jccl::ConfigChunk::getFullName)
      .def("getDescToken", &jccl::ConfigChunk::getDescToken)
   ;
// =============================================================== JCCL classes

// GMTL classes ===============================================================
   class_<gmtl::Matrix44f>("Matrix44f")
//      .def("getData", &gmtl::Matrix44f::getData, return_internal_reference<>())
      .def_readonly("mData", &gmtl::Matrix44f::mData)
   ;

   class_<gmtl::Point3f>("Point3f")
   ;

   class_<gmtl::Vec3f>("Vec3f")
   ;

   // PyJuggler.Math scope.
   {
//   scope gmtl_Math = class_<Dummy>("Math", no_init)
//   ;

//   def("deg2Rad", (float(*) (float)) 0, gmtl::Math::deg2Rad);
//   def("rad2Deg", (float(*) (float)) 0, gmtl::Math::rad2Deg);
   }
// =============================================================== GMTL classes
}
