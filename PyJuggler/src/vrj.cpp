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

// Includes ====================================================================
#include <boost/python.hpp>
#include <vrj/Kernel/User.h>
#include <vrj/Kernel/App.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/CameraProjection.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Display/Display.h>
#include <pyjutil/InterpreterGuard.h>
#include <pyjutil/Debug.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


struct vrj_Display_Wrapper: vrj::Display
{
    vrj_Display_Wrapper(PyObject* self_, const vrj::Display & p0):
        vrj::Display(p0), self(self_) {}

    vrj_Display_Wrapper(PyObject* self_):
        vrj::Display(), self(self_) {}

    void config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        call_method< void >(self, "config", p0);
    }

    void default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vrj::Display::config(p0);
    }

    PyObject* self;
};

struct vrj_Projection_Wrapper: vrj::Projection
{
    vrj_Projection_Wrapper(PyObject* self_):
        vrj::Projection(), self(self_) {}

    vrj_Projection_Wrapper(PyObject* self_, const vrj::Projection & p0):
        vrj::Projection(p0), self(self_) {}

    void config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        call_method< void >(self, "config", p0);
    }

    void default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vrj::Projection::config(p0);
    }

    void calcViewMatrix(gmtl::Matrix<float,4,4> & p0, const float p1) {
        call_method< void >(self, "calcViewMatrix", p0, p1);
    }

    PyObject* self;
};

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

struct vrj_Viewport_Wrapper: vrj::Viewport
{
    vrj_Viewport_Wrapper(PyObject* self_):
        vrj::Viewport(), self(self_) {}

    vrj_Viewport_Wrapper(PyObject* self_, const vrj::Viewport & p0):
        vrj::Viewport(p0), self(self_) {}

    void config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        call_method< void >(self, "config", p0);
    }

    void default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vrj::Viewport::config(p0);
    }

    void updateProjections(const float p0) {
        call_method< void >(self, "updateProjections", p0);
    }

    PyObject* self;
};

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
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "init");
    }

    void default_init() {
        vrj::App::init();
    }

    void apiInit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::apiInit()\n",
                                 "vrj_App_Wrapper::apiInit() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "apiInit");
    }

    void default_apiInit() {
        vrj::App::apiInit();
    }

    void exit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::exit()\n",
                                 "vrj_App_Wrapper::exit() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "exit");
    }

    void default_exit() {
        vrj::App::exit();
    }

    void preFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::preFrame()\n",
                                 "vrj_App_Wrapper::preFrame() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "preFrame");
    }

    void default_preFrame() {
        vrj::App::preFrame();
    }

    void intraFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::intraFrame()\n",
                                 "vrj_App_Wrapper::intraFrame() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "intraFrame");
    }

    void default_intraFrame() {
        vrj::App::intraFrame();
    }

    void postFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::postFrame()\n",
                                 "vrj_App_Wrapper::postFrame() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "postFrame");
    }

    void default_postFrame() {
        vrj::App::postFrame();
    }

    void reset() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::reset()\n",
                                 "vrj_App_Wrapper::reset() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "reset");
    }

    void default_reset() {
        vrj::App::reset();
    }

    void focusChanged() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::focusChanged()\n",
                                 "vrj_App_Wrapper::focusChanged() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "focusChanged");
    }

    void default_focusChanged() {
        vrj::App::focusChanged();
    }

    float getDrawScaleFactor() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::getDrawScaleFactor()\n",
                                 "vrj_App_Wrapper::getDrawScaleFactor() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< float >(self, "getDrawScaleFactor");
    }

    float default_getDrawScaleFactor() {
        return vrj::App::getDrawScaleFactor();
    }

    bool configCanHandle(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configCanHandle()\n",
                                 "vrj_App_Wrapper::configCanHandle() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< bool >(self, "configCanHandle", p0);
    }

    bool default_configCanHandle(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return vrj::App::configCanHandle(p0);
    }

    bool depSatisfied() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::depSatisfied()\n",
                                 "vrj_App_Wrapper::depSatisfied() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< bool >(self, "depSatisfied");
    }

    bool default_depSatisfied() {
        return vrj::App::depSatisfied();
    }

    bool configAdd(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configAdd()\n",
                                 "vrj_App_Wrapper::configAdd() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< bool >(self, "configAdd", p0);
    }

    bool configRemove(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configRemove()\n",
                                 "vrj_App_Wrapper::configRemove() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< bool >(self, "configRemove", p0);
    }

    vrj::DrawManager * getDrawManager() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::getDrawManager()\n",
                                 "vrj_App_Wrapper::getDrawManager() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< vrj::DrawManager * >(self, "getDrawManager");
    }

    int configProcessPending() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configProcessPending()\n",
                                 "vrj_App_Wrapper::configProcessPending() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< int >(self, "configProcessPending");
    }

    int default_configProcessPending() {
        return vrj::App::configProcessPending();
    }

    PyObject* self;
};

struct vrj_CameraProjection_Wrapper: vrj::CameraProjection
{
    vrj_CameraProjection_Wrapper(PyObject* self_, const vrj::CameraProjection & p0):
        vrj::CameraProjection(p0), self(self_) {}

    vrj_CameraProjection_Wrapper(PyObject* self_):
        vrj::CameraProjection(), self(self_) {}

    void config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        call_method< void >(self, "config", p0);
    }

    void default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vrj::CameraProjection::config(p0);
    }

    void calcViewMatrix(gmtl::Matrix<float,4,4> & p0, const float p1) {
        call_method< void >(self, "calcViewMatrix", p0, p1);
    }

    void default_calcViewMatrix(gmtl::Matrix<float,4,4> & p0, const float p1) {
        vrj::CameraProjection::calcViewMatrix(p0, p1);
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

struct vrj_SimViewport_Wrapper: vrj::SimViewport
{
    vrj_SimViewport_Wrapper(PyObject* self_):
        vrj::SimViewport(), self(self_) {}

    vrj_SimViewport_Wrapper(PyObject* self_, const vrj::SimViewport & p0):
        vrj::SimViewport(p0), self(self_) {}

    void config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        call_method< void >(self, "config", p0);
    }

    void default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vrj::SimViewport::config(p0);
    }

    void updateProjections(const float p0) {
        call_method< void >(self, "updateProjections", p0);
    }

    void default_updateProjections(const float p0) {
        vrj::SimViewport::updateProjections(p0);
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
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "draw");
    }

    void contextInit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextInit()\n",
                                 "vrj_GlApp_Wrapper::contextInit() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "contextInit");
    }

    void default_contextInit() {
        vrj::GlApp::contextInit();
    }

    void contextClose() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextClose()\n",
                                 "vrj_GlApp_Wrapper::contextClose() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "contextClose");
    }

    void default_contextClose() {
        vrj::GlApp::contextClose();
    }

    void contextPreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextPreDraw()\n",
                                 "vrj_GlApp_Wrapper::contextPreDraw() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "contextPreDraw");
    }

    void default_contextPreDraw() {
        vrj::GlApp::contextPreDraw();
    }

    void contextPostDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextPostDraw()\n",
                                 "vrj_GlApp_Wrapper::contextPostDraw() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "contextPostDraw");
    }

    void default_contextPostDraw() {
        vrj::GlApp::contextPostDraw();
    }

    void bufferPreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::bufferPreDraw()\n",
                                 "vrj_GlApp_Wrapper::bufferPreDraw() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "bufferPreDraw");
    }

    void default_bufferPreDraw() {
        vrj::GlApp::bufferPreDraw();
    }

    void pipePreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::pipePreDraw()\n",
                                 "vrj_GlApp_Wrapper::pipePreDraw() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "pipePreDraw");
    }

    void default_pipePreDraw() {
        vrj::GlApp::pipePreDraw();
    }

    void init() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::init()\n",
                                 "vrj_GlApp_Wrapper::init() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "init");
    }

    void default_init() {
        vrj::GlApp::init();
    }

    void apiInit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::apiInit()\n",
                                 "vrj_GlApp_Wrapper::apiInit() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "apiInit");
    }

    void default_apiInit() {
        vrj::GlApp::apiInit();
    }

    void exit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::exit()\n",
                                 "vrj_GlApp_Wrapper::exit() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "exit");
    }

    void default_exit() {
        vrj::GlApp::exit();
    }

    void preFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::preFrame()\n",
                                 "vrj_GlApp_Wrapper::preFrame() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "preFrame");
    }

    void default_preFrame() {
        vrj::GlApp::preFrame();
    }

    void intraFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::intraFrame()\n",
                                 "vrj_GlApp_Wrapper::intraFrame() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "intraFrame");
    }

    void default_intraFrame() {
        vrj::GlApp::intraFrame();
    }

    void postFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::postFrame()\n",
                                 "vrj_GlApp_Wrapper::postFrame() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "postFrame");
    }

    void default_postFrame() {
        vrj::GlApp::postFrame();
    }

    void reset() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::reset()\n",
                                 "vrj_GlApp_Wrapper::reset() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "reset");
    }

    void default_reset() {
        vrj::GlApp::reset();
    }

    void focusChanged() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::focusChanged()\n",
                                 "vrj_GlApp_Wrapper::focusChanged() done.\n");
        PyJuggler::InterpreterGuard guard;
        call_method< void >(self, "focusChanged");
    }

    void default_focusChanged() {
        vrj::GlApp::focusChanged();
    }

    float getDrawScaleFactor() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::getDrawScaleFactor()\n",
                                 "vrj_GlApp_Wrapper::getDrawScaleFactor() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< float >(self, "getDrawScaleFactor");
    }

    float default_getDrawScaleFactor() {
        return vrj::GlApp::getDrawScaleFactor();
    }

    bool configCanHandle(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configCanHandle()\n",
                                 "vrj_GlApp_Wrapper::configCanHandle() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< bool >(self, "configCanHandle", p0);
    }

    bool default_configCanHandle(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return vrj::GlApp::configCanHandle(p0);
    }

    bool depSatisfied() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::depSatisfied()\n",
                                 "vrj_GlApp_Wrapper::depSatisfied() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< bool >(self, "depSatisfied");
    }

    bool default_depSatisfied() {
        return vrj::GlApp::depSatisfied();
    }

    bool configAdd(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configAdd()\n",
                                 "vrj_GlApp_Wrapper::configAdd() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< bool >(self, "configAdd", p0);
    }

    bool configRemove(boost::shared_ptr<jccl::ConfigChunk> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configRemove()\n",
                                 "vrj_GlApp_Wrapper::configRemove() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< bool >(self, "configRemove", p0);
    }

    int configProcessPending() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configProcessPending()\n",
                                 "vrj_GlApp_Wrapper::configProcessPending() done.\n");
        PyJuggler::InterpreterGuard guard;
        return call_method< int >(self, "configProcessPending");
    }

    int default_configProcessPending() {
        return vrj::GlApp::configProcessPending();
    }

    PyObject* self;
};

inline tuple vrj_Display_getOriginAndSize_wrapper(vrj::Display* disp)
{
   int xo, yo, xs, ys;
   disp->getOriginAndSize(xo, yo, xs, ys);
   return make_tuple(xo, yo, xs, ys);
}

inline tuple vrj_Viewport_getOriginAndSize_wrapper(vrj::Viewport* vp)
{
   float xo, yo, xs, ys;
   vp->getOriginAndSize(xo, yo, xs, ys);
   return make_tuple(xo, yo, xs, ys);
}


}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(vrj)
{
    // Create and acquire the global interpreter lock.
    PyEval_InitThreads();

    class_< vrj::Display, vrj_Display_Wrapper >("Display", init<  >())
        .def(init< const vrj::Display & >())
        .def("configDisplayWindow", &vrj::Display::configDisplayWindow)
        .def("configViewports", &vrj::Display::configViewports)
        .def("updateProjections", &vrj::Display::updateProjections)
        .def("isActive", &vrj::Display::isActive)
        .def("setName", &vrj::Display::setName)
        .def("getName", &vrj::Display::getName)
        .def("shouldDrawBorder", &vrj::Display::shouldDrawBorder)
        .def("setOriginAndSize", &vrj::Display::setOriginAndSize)
        .def("getOriginAndSize", vrj_Display_getOriginAndSize_wrapper)
        .def("setPipe", &vrj::Display::setPipe)
        .def("getPipe", &vrj::Display::getPipe)
        .def("inStereo", &vrj::Display::inStereo)
        .def("getConfigChunk", &vrj::Display::getConfigChunk)
        .def("getGlFrameBufferConfig", &vrj::Display::getGlFrameBufferConfig)
        .def("getNumViewports", &vrj::Display::getNumViewports)
        .def("getViewport", &vrj::Display::getViewport, return_internal_reference< 1 >())
        .def("config", &vrj::Display::config, &vrj_Display_Wrapper::default_config)
//        .def(self_ns::str(self))
    ;

    scope* vrj_Projection_scope = new scope(
    class_< vrj::Projection, boost::noncopyable, vrj_Projection_Wrapper >("Projection", init<  >())
        .def("setEye", &vrj::Projection::setEye)
        .def("getEye", &vrj::Projection::getEye)
        .def("setViewport", &vrj::Projection::setViewport)
        .def("getViewport", &vrj::Projection::getViewport, return_internal_reference< 1 >())
        .def("getFrustumApexAndCorners", &vrj::Projection::getFrustumApexAndCorners)
        .def("getViewMatrix", &vrj::Projection::getViewMatrix, return_value_policy< copy_const_reference >())
        .def("getFrustum", &vrj::Projection::getFrustum)
        .def("setNearFar", &vrj::Projection::setNearFar)
        .staticmethod("setNearFar")
        .def("config", &vrj::Projection::config, &vrj_Projection_Wrapper::default_config)
//        .def(self_ns::str(self))
    );

    enum_< vrj::Projection::Eye >("Eye")
        .value("RIGHT", vrj::Projection::RIGHT)
        .value("LEFT", vrj::Projection::LEFT)
    ;

    delete vrj_Projection_scope;

    class_< vrj::User, boost::noncopyable, vrj_User_Wrapper >("User", init<  >())
        .def("getId", &vrj::User::getId)
        .def("getName", &vrj::User::getName)
        .def("getHeadUpdateTime", &vrj::User::getHeadUpdateTime)
        .def("getInterocularDistance", &vrj::User::getInterocularDistance)
        .def("config", &vrj::User::config, &vrj_User_Wrapper::default_config)
    ;

    scope* vrj_Viewport_scope = new scope(
    class_< vrj::Viewport, boost::noncopyable, vrj_Viewport_Wrapper >("Viewport", init<  >())
        .def("recordLatency", &vrj::Viewport::recordLatency)
        .def("getType", &vrj::Viewport::getType)
        .def("isSimulator", &vrj::Viewport::isSimulator)
        .def("isSurface", &vrj::Viewport::isSurface)
        .def("isActive", &vrj::Viewport::isActive)
        .def("setName", &vrj::Viewport::setName)
        .def("getName", &vrj::Viewport::getName)
        .def("inStereo", &vrj::Viewport::inStereo)
        .def("getView", &vrj::Viewport::getView)
        .def("setOriginAndSize", &vrj::Viewport::setOriginAndSize)
        .def("getOriginAndSize", vrj_Viewport_getOriginAndSize_wrapper)
        .def("getConfigChunk", &vrj::Viewport::getConfigChunk)
        .def("getUser", &vrj::Viewport::getUser, return_internal_reference< 1 >())
        .def("setDisplay", &vrj::Viewport::setDisplay)
        .def("getDisplay", &vrj::Viewport::getDisplay, return_internal_reference< 1 >())
        .def("getLeftProj", &vrj::Viewport::getLeftProj, return_internal_reference< 1 >())
        .def("getRightProj", &vrj::Viewport::getRightProj, return_internal_reference< 1 >())
        .def("config", &vrj::Viewport::config, &vrj_Viewport_Wrapper::default_config)
//        .def(self_ns::str(self))
    );

    enum_< vrj::Viewport::Type >("Type")
        .value("SIM", vrj::Viewport::SIM)
        .value("UNDEFINED", vrj::Viewport::UNDEFINED)
        .value("SURFACE", vrj::Viewport::SURFACE)
    ;


    enum_< vrj::Viewport::View >("View")
        .value("LEFT_EYE", vrj::Viewport::LEFT_EYE)
        .value("NONE", vrj::Viewport::NONE)
        .value("STEREO", vrj::Viewport::STEREO)
        .value("RIGHT_EYE", vrj::Viewport::RIGHT_EYE)
    ;

    delete vrj_Viewport_scope;

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

    class_< vrj::CameraProjection, bases< vrj::Projection > , vrj_CameraProjection_Wrapper >("CameraProjection", init<  >())
        .def(init< const vrj::CameraProjection & >())
        .def_readwrite("mVertFOV", &vrj::CameraProjection::mVertFOV)
        .def("setVerticalFOV", &vrj::CameraProjection::setVerticalFOV)
        .def("config", &vrj::CameraProjection::config, &vrj_CameraProjection_Wrapper::default_config)
        .def("calcViewMatrix", &vrj::CameraProjection::calcViewMatrix, &vrj_CameraProjection_Wrapper::default_calcViewMatrix)
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

    class_< vrj::SimViewport, bases< vrj::Viewport > , vrj_SimViewport_Wrapper >("SimViewport", init<  >())
        .def("getDrawSimInterface", &vrj::SimViewport::getDrawSimInterface, return_internal_reference< 1 >())
        .def("config", &vrj::SimViewport::config, &vrj_SimViewport_Wrapper::default_config)
        .def("updateProjections", &vrj::SimViewport::updateProjections, &vrj_SimViewport_Wrapper::default_updateProjections)
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
