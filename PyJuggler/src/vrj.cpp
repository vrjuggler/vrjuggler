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
#include <iomanip>

// Includes ====================================================================
#include <boost/python.hpp>
#include <vpr/Util/Debug.h>
#include <vrj/Kernel/User.h>
#include <vrj/Kernel/App.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OGL/GlApp.h>
#include "MethodGuard.h"
#include "Debug.h"

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace  {


struct vrj_User_Wrapper: vrj::User
{
    vrj_User_Wrapper(PyObject* self_):
        vrj::User(), self(self_) {}

    bool config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return vrj::User::config(p0);
    }

    PyObject* self;
};

struct vrj_Kernel_Wrapper: vrj::Kernel
{
    bool configCanHandle(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return call_method< bool >(self, "configCanHandle", p0);
    }

    bool configAdd(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return call_method< bool >(self, "configAdd", p0);
    }

    bool configRemove(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return call_method< bool >(self, "configRemove", p0);
    }

    int configProcessPending() {
        return call_method< int >(self, "configProcessPending");
    }

    int default_configProcessPending() {
        return vrj::Kernel::configProcessPending();
    }

    PyObject* self;
};

void vrj_Kernel_waitForKernelStop(vrj::Kernel* kernel)
{
   // A VR Juggler application written entirely in Python will call
   // vrj.Kernel.waitForKernelStop() and block until the kernel shuts down.
   // This is definitely a blocking I/O kind of case, so we need to release
   // the global interpreter lock before we block on the kernel shutdown
   // condition.
   Py_BEGIN_ALLOW_THREADS;
      kernel->waitForKernelStop();
   Py_END_ALLOW_THREADS;
}

struct vrj_App_Wrapper: vrj::App
{
    vrj_App_Wrapper(PyObject* self_, const vrj::App & p0):
        vrj::App(p0), self(self_) {}

    vrj_App_Wrapper(PyObject* self_, vrj::Kernel * p0):
        vrj::App(p0), self(self_) {}

    vrj_App_Wrapper(PyObject* self_):
        vrj::App(), self(self_) {}

    virtual ~vrj_App_Wrapper()
    {
    }

    void init() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::init()\n",
                                 "vrj_App_Wrapper::init() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "init");
    }

    void default_init() {
        vrj::App::init();
    }

    void apiInit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::apiInit()\n",
                                 "vrj_App_Wrapper::apiInit() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "apiInit");
    }

    void default_apiInit() {
        vrj::App::apiInit();
    }

    void exit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::exit()\n",
                                 "vrj_App_Wrapper::exit() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "exit");
    }

    void default_exit() {
        vrj::App::exit();
    }

    void preFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::preFrame()\n",
                                 "vrj_App_Wrapper::preFrame() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "preFrame");
    }

    void default_preFrame() {
        vrj::App::preFrame();
    }

    void intraFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::intraFrame()\n",
                                 "vrj_App_Wrapper::intraFrame() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "intraFrame");
    }

    void default_intraFrame() {
        vrj::App::intraFrame();
    }

    void postFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::postFrame()\n",
                                 "vrj_App_Wrapper::postFrame() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "postFrame");
    }

    void default_postFrame() {
        vrj::App::postFrame();
    }

    void reset() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::reset()\n",
                                 "vrj_App_Wrapper::reset() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "reset");
    }

    void default_reset() {
        vrj::App::reset();
    }

    void focusChanged() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::focusChanged()\n",
                                 "vrj_App_Wrapper::focusChanged() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "focusChanged");
    }

    void default_focusChanged() {
        vrj::App::focusChanged();
    }

    float getDrawScaleFactor() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::getDrawScaleFactor()\n",
                                 "vrj_App_Wrapper::getDrawScaleFactor() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< float >(self, "getDrawScaleFactor");
    }

    float default_getDrawScaleFactor() {
        return vrj::App::getDrawScaleFactor();
    }

    bool configCanHandle(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configCanHandle()\n",
                                 "vrj_App_Wrapper::configCanHandle() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< bool >(self, "configCanHandle", p0);
    }

    bool default_configCanHandle(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return vrj::App::configCanHandle(p0);
    }

    bool depSatisfied() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::depSatisfied()\n",
                                 "vrj_App_Wrapper::depSatisfied() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< bool >(self, "depSatisfied");
    }

    bool default_depSatisfied() {
        return vrj::App::depSatisfied();
    }

    bool configAdd(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configAdd()\n",
                                 "vrj_App_Wrapper::configAdd() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< bool >(self, "configAdd", p0);
    }

    bool configRemove(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configRemove()\n",
                                 "vrj_App_Wrapper::configRemove() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< bool >(self, "configRemove", p0);
    }

    vrj::DrawManager * getDrawManager() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::getDrawManager()\n",
                                 "vrj_App_Wrapper::getDrawManager() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< vrj::DrawManager * >(self, "getDrawManager");
    }

    int configProcessPending() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configProcessPending()\n",
                                 "vrj_App_Wrapper::configProcessPending() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< int >(self, "configProcessPending");
    }

    int default_configProcessPending() {
        return vrj::App::configProcessPending();
    }

    PyObject* self;
};

struct vrj_GlApp_Wrapper: vrj::GlApp
{
    vrj_GlApp_Wrapper(PyObject* self_, const vrj::GlApp & p0):
        vrj::GlApp(p0), self(self_) {}

    vrj_GlApp_Wrapper(PyObject* self_, vrj::Kernel * p0):
        vrj::GlApp(p0), self(self_) {}

    vrj_GlApp_Wrapper(PyObject* self_):
        vrj::GlApp(), self(self_) {}

    virtual ~vrj_GlApp_Wrapper()
    {
    }

    void draw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::draw()\n",
                                 "vrj_GlApp_Wrapper::draw() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "draw");
    }

    void contextInit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextInit()\n",
                                 "vrj_GlApp_Wrapper::contextInit() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "contextInit");
    }

    void default_contextInit() {
        vrj::GlApp::contextInit();
    }

    void contextClose() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextClose()\n",
                                 "vrj_GlApp_Wrapper::contextClose() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "contextClose");
    }

    void default_contextClose() {
        vrj::GlApp::contextClose();
    }

    void contextPreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextPreDraw()\n",
                                 "vrj_GlApp_Wrapper::contextPreDraw() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "contextPreDraw");
    }

    void default_contextPreDraw() {
        vrj::GlApp::contextPreDraw();
    }

    void contextPostDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextPostDraw()\n",
                                 "vrj_GlApp_Wrapper::contextPostDraw() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "contextPostDraw");
    }

    void default_contextPostDraw() {
        vrj::GlApp::contextPostDraw();
    }

    void bufferPreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::bufferPreDraw()\n",
                                 "vrj_GlApp_Wrapper::bufferPreDraw() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "bufferPreDraw");
    }

    void default_bufferPreDraw() {
        vrj::GlApp::bufferPreDraw();
    }

    void pipePreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::pipePreDraw()\n",
                                 "vrj_GlApp_Wrapper::pipePreDraw() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "pipePreDraw");
    }

    void default_pipePreDraw() {
        vrj::GlApp::pipePreDraw();
    }

    void init() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::init()\n",
                                 "vrj_GlApp_Wrapper::init() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "init");
    }

    void default_init() {
        vrj::GlApp::init();
    }

    void apiInit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::apiInit()\n",
                                 "vrj_GlApp_Wrapper::apiInit() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "apiInit");
    }

    void default_apiInit() {
        vrj::GlApp::apiInit();
    }

    void exit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::exit()\n",
                                 "vrj_GlApp_Wrapper::exit() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "exit");
    }

    void default_exit() {
        vrj::GlApp::exit();
    }

    void preFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::preFrame()\n",
                                 "vrj_GlApp_Wrapper::preFrame() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "preFrame");
    }

    void default_preFrame() {
        vrj::GlApp::preFrame();
    }

    void intraFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::intraFrame()\n",
                                 "vrj_GlApp_Wrapper::intraFrame() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "intraFrame");
    }

    void default_intraFrame() {
        vrj::GlApp::intraFrame();
    }

    void postFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::postFrame()\n",
                                 "vrj_GlApp_Wrapper::postFrame() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "postFrame");
    }

    void default_postFrame() {
        vrj::GlApp::postFrame();
    }

    void reset() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::reset()\n",
                                 "vrj_GlApp_Wrapper::reset() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "reset");
    }

    void default_reset() {
        vrj::GlApp::reset();
    }

    void focusChanged() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::focusChanged()\n",
                                 "vrj_GlApp_Wrapper::focusChanged() done.\n");
        PyJuggler::MethodGuard guard;
        call_method< void >(self, "focusChanged");
    }

    void default_focusChanged() {
        vrj::GlApp::focusChanged();
    }

    float getDrawScaleFactor() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::getDrawScaleFactor()\n",
                                 "vrj_GlApp_Wrapper::getDrawScaleFactor() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< float >(self, "getDrawScaleFactor");
    }

    float default_getDrawScaleFactor() {
        return vrj::GlApp::getDrawScaleFactor();
    }

    bool configCanHandle(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configCanHandle()\n",
                                 "vrj_GlApp_Wrapper::configCanHandle() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< bool >(self, "configCanHandle", p0);
    }

    bool default_configCanHandle(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return vrj::GlApp::configCanHandle(p0);
    }

    bool depSatisfied() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::depSatisfied()\n",
                                 "vrj_GlApp_Wrapper::depSatisfied() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< bool >(self, "depSatisfied");
    }

    bool default_depSatisfied() {
        return vrj::GlApp::depSatisfied();
    }

    bool configAdd(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configAdd()\n",
                                 "vrj_GlApp_Wrapper::configAdd() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< bool >(self, "configAdd", p0);
    }

    bool configRemove(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configRemove()\n",
                                 "vrj_GlApp_Wrapper::configRemove() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< bool >(self, "configRemove", p0);
    }

    int configProcessPending() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configProcessPending()\n",
                                 "vrj_GlApp_Wrapper::configProcessPending() done.\n");
        PyJuggler::MethodGuard guard;
        return call_method< int >(self, "configProcessPending");
    }

    int default_configProcessPending() {
        return vrj::GlApp::configProcessPending();
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(vrj)
{
    // Create and acquire the global interpreter lock.
    PyEval_InitThreads();

    class_< vrj::User, boost::noncopyable, vrj_User_Wrapper >("User", init<  >())
        .def("getId", &vrj::User::getId)
        .def("getName", &vrj::User::getName)
        .def("getHeadUpdateTime", &vrj::User::getHeadUpdateTime)
        .def("getInterocularDistance", &vrj::User::getInterocularDistance)
        .def("config", &vrj::User::config, &vrj_User_Wrapper::default_config)
    ;

    class_< vrj::Kernel, boost::noncopyable, vrj_Kernel_Wrapper >("Kernel", no_init)
        .def("start", &vrj::Kernel::start)
        .def("stop", &vrj::Kernel::stop)
        .def("isRunning", &vrj::Kernel::isRunning)
        .def("waitForKernelStop", vrj_Kernel_waitForKernelStop)
        .def("initConfig", &vrj::Kernel::initConfig)
        .def("controlLoop", &vrj::Kernel::controlLoop)
        .def("setApplication", &vrj::Kernel::setApplication)
        .def("loadConfigFile", (void (vrj::Kernel::*)(const char *) )&vrj::Kernel::loadConfigFile)
        .def("loadConfigFile", (void (vrj::Kernel::*)(std::basic_string<char,std::char_traits<char>,std::allocator<char> >) )&vrj::Kernel::loadConfigFile)
        .def("loadChunkDescFile", &vrj::Kernel::loadChunkDescFile)
        .def("getUser", &vrj::Kernel::getUser, return_internal_reference< 1 >())
        .def("getUsers", &vrj::Kernel::getUsers)
        .def("instance", &vrj::Kernel::instance, return_value_policy< reference_existing_object >())
        .staticmethod("instance")
        .def("configProcessPending", &vrj::Kernel::configProcessPending, &vrj_Kernel_Wrapper::default_configProcessPending)
    ;

    class_< vrj::App, boost::noncopyable, vrj_App_Wrapper >("App", init<  >())
        .def(init< vrj::Kernel * >())
        .def_readwrite("mKernel", &vrj::App::mKernel)
        .def_readwrite("mHaveFocus", &vrj::App::mHaveFocus)
        .def("haveFocus", &vrj::App::haveFocus)
        .def("setFocus", &vrj::App::setFocus)
        .def("init", &vrj::App::init, &vrj_App_Wrapper::default_init)
        .def("apiInit", &vrj::App::apiInit, &vrj_App_Wrapper::default_apiInit)
        .def("exit", &vrj::App::exit, &vrj_App_Wrapper::default_exit)
        .def("preFrame", &vrj::App::preFrame, &vrj_App_Wrapper::default_preFrame)
        .def("intraFrame", &vrj::App::intraFrame, &vrj_App_Wrapper::default_intraFrame)
        .def("postFrame", &vrj::App::postFrame, &vrj_App_Wrapper::default_postFrame)
        .def("reset", &vrj::App::reset, &vrj_App_Wrapper::default_reset)
        .def("focusChanged", &vrj::App::focusChanged, &vrj_App_Wrapper::default_focusChanged)
        .def("getDrawScaleFactor", &vrj::App::getDrawScaleFactor, &vrj_App_Wrapper::default_getDrawScaleFactor)
        .def("configCanHandle", &vrj::App::configCanHandle, &vrj_App_Wrapper::default_configCanHandle)
        .def("depSatisfied", &vrj::App::depSatisfied, &vrj_App_Wrapper::default_depSatisfied)
        .def("configProcessPending", &vrj::App::configProcessPending, &vrj_App_Wrapper::default_configProcessPending)
    ;

    class_< vrj::GlApp, bases< vrj::App > , boost::noncopyable, vrj_GlApp_Wrapper >("GlApp", init<  >())
        .def(init< vrj::Kernel * >())
        .def("contextInit", &vrj::GlApp::contextInit, &vrj_GlApp_Wrapper::default_contextInit)
        .def("contextClose", &vrj::GlApp::contextClose, &vrj_GlApp_Wrapper::default_contextClose)
        .def("contextPreDraw", &vrj::GlApp::contextPreDraw, &vrj_GlApp_Wrapper::default_contextPreDraw)
        .def("contextPostDraw", &vrj::GlApp::contextPostDraw, &vrj_GlApp_Wrapper::default_contextPostDraw)
        .def("bufferPreDraw", &vrj::GlApp::bufferPreDraw, &vrj_GlApp_Wrapper::default_bufferPreDraw)
        .def("pipePreDraw", &vrj::GlApp::pipePreDraw, &vrj_GlApp_Wrapper::default_pipePreDraw)
        .def("init", &vrj::GlApp::init, &vrj_GlApp_Wrapper::default_init)
        .def("apiInit", &vrj::GlApp::apiInit, &vrj_GlApp_Wrapper::default_apiInit)
        .def("exit", &vrj::GlApp::exit, &vrj_GlApp_Wrapper::default_exit)
        .def("preFrame", &vrj::GlApp::preFrame, &vrj_GlApp_Wrapper::default_preFrame)
        .def("intraFrame", &vrj::GlApp::intraFrame, &vrj_GlApp_Wrapper::default_intraFrame)
        .def("postFrame", &vrj::GlApp::postFrame, &vrj_GlApp_Wrapper::default_postFrame)
        .def("reset", &vrj::GlApp::reset, &vrj_GlApp_Wrapper::default_reset)
        .def("focusChanged", &vrj::GlApp::focusChanged, &vrj_GlApp_Wrapper::default_focusChanged)
        .def("getDrawScaleFactor", &vrj::GlApp::getDrawScaleFactor, &vrj_GlApp_Wrapper::default_getDrawScaleFactor)
        .def("configCanHandle", &vrj::GlApp::configCanHandle, &vrj_GlApp_Wrapper::default_configCanHandle)
        .def("depSatisfied", &vrj::GlApp::depSatisfied, &vrj_GlApp_Wrapper::default_depSatisfied)
        .def("configProcessPending", &vrj::GlApp::configProcessPending, &vrj_GlApp_Wrapper::default_configProcessPending)
    ;

}
