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

// Includes ====================================================================
#include <boost/python.hpp>
#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Kernel/Kernel.h>
#include <pyjutil/InterpreterGuard.h>
#include <pyjutil/Debug.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


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

        try
        {
            call_method< void >(self, "draw");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void contextInit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextInit()\n",
                                 "vrj_GlApp_Wrapper::contextInit() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "contextInit");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_contextInit() {
        vrj::GlApp::contextInit();
    }

    void contextClose() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextClose()\n",
                                 "vrj_GlApp_Wrapper::contextClose() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "contextClose");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_contextClose() {
        vrj::GlApp::contextClose();
    }

    void contextPreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextPreDraw()\n",
                                 "vrj_GlApp_Wrapper::contextPreDraw() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "contextPreDraw");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_contextPreDraw() {
        vrj::GlApp::contextPreDraw();
    }

    void contextPostDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::contextPostDraw()\n",
                                 "vrj_GlApp_Wrapper::contextPostDraw() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "contextPostDraw");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_contextPostDraw() {
        vrj::GlApp::contextPostDraw();
    }

    void bufferPreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::bufferPreDraw()\n",
                                 "vrj_GlApp_Wrapper::bufferPreDraw() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "bufferPreDraw");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_bufferPreDraw() {
        vrj::GlApp::bufferPreDraw();
    }

    void pipePreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::pipePreDraw()\n",
                                 "vrj_GlApp_Wrapper::pipePreDraw() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "pipePreDraw");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_pipePreDraw() {
        vrj::GlApp::pipePreDraw();
    }

    void init() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::init()\n",
                                 "vrj_GlApp_Wrapper::init() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "init");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_init() {
        vrj::GlApp::init();
    }

    void apiInit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::apiInit()\n",
                                 "vrj_GlApp_Wrapper::apiInit() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "apiInit");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_apiInit() {
        vrj::GlApp::apiInit();
    }

    void exit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::exit()\n",
                                 "vrj_GlApp_Wrapper::exit() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "exit");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_exit() {
        vrj::GlApp::exit();
    }

    void preFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::preFrame()\n",
                                 "vrj_GlApp_Wrapper::preFrame() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "preFrame");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_preFrame() {
        vrj::GlApp::preFrame();
    }

    void latePreFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::latePreFrame()\n",
                                 "vrj_GlApp_Wrapper::latePreFrame() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "latePreFrame");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_latePreFrame() {
        vrj::GlApp::latePreFrame();
    }

    void intraFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::postFrame()\n",
                                 "vrj_GlApp_Wrapper::postFrame() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "postFrame");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_intraFrame() {
        vrj::GlApp::intraFrame();
    }

    void postFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::postFrame()\n",
                                 "vrj_GlApp_Wrapper::postFrame() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "postFrame");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_postFrame() {
        vrj::GlApp::postFrame();
    }

    void reset() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::reset()\n",
                                 "vrj_GlApp_Wrapper::reset() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "reset");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_reset() {
        vrj::GlApp::reset();
    }

    void focusChanged() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::focusChanged()\n",
                                 "vrj_GlApp_Wrapper::focusChanged() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "focusChanged");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_focusChanged() {
        vrj::GlApp::focusChanged();
    }

    float getDrawScaleFactor() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::getDrawScaleFactor()\n",
                                 "vrj_GlApp_Wrapper::getDrawScaleFactor() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< float >(self, "getDrawScaleFactor");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return 1.0f;
    }

    float default_getDrawScaleFactor() {
        return vrj::GlApp::getDrawScaleFactor();
    }

    bool configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configCanHandle()\n",
                                 "vrj_GlApp_Wrapper::configCanHandle() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< bool >(self, "configCanHandle", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0) {
        return vrj::GlApp::configCanHandle(p0);
    }

    bool depSatisfied() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::depSatisfied()\n",
                                 "vrj_GlApp_Wrapper::depSatisfied() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< bool >(self, "depSatisfied");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return true;
    }

    bool default_depSatisfied() {
        return vrj::GlApp::depSatisfied();
    }

    bool configAdd(boost::shared_ptr<jccl::ConfigElement> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configAdd()\n",
                                 "vrj_GlApp_Wrapper::configAdd() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< bool >(self, "configAdd", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_configAdd(boost::shared_ptr<jccl::ConfigElement> p0) {
        return vrj::GlApp::configAdd(p0);
    }

    bool configRemove(boost::shared_ptr<jccl::ConfigElement> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configRemove()\n",
                                 "vrj_GlApp_Wrapper::configRemove() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< bool >(self, "configRemove", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_configRemove(boost::shared_ptr<jccl::ConfigElement> p0) {
        return vrj::GlApp::configRemove(p0);
    }

    int configProcessPending() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_GlApp_Wrapper::configProcessPending()\n",
                                 "vrj_GlApp_Wrapper::configProcessPending() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< int >(self, "configProcessPending");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return 0;
    }

    int default_configProcessPending() {
        return vrj::GlApp::configProcessPending();
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
void _Export_GlApp()
{
    class_< vrj::GlApp, bases< vrj::App >, boost::noncopyable, vrj_GlApp_Wrapper >("GlApp", init<  >())
        .def(init< vrj::Kernel * >())
        .def_readwrite("mKernel", &vrj::App::mKernel)
        .def_readwrite("mHaveFocus", &vrj::App::mHaveFocus)
        .def("contextInit", &vrj::GlApp::contextInit, &vrj_GlApp_Wrapper::default_contextInit)
        .def("contextClose", &vrj::GlApp::contextClose, &vrj_GlApp_Wrapper::default_contextClose)
        .def("contextPreDraw", &vrj::GlApp::contextPreDraw, &vrj_GlApp_Wrapper::default_contextPreDraw)
        .def("contextPostDraw", &vrj::GlApp::contextPostDraw, &vrj_GlApp_Wrapper::default_contextPostDraw)
        .def("bufferPreDraw", &vrj::GlApp::bufferPreDraw, &vrj_GlApp_Wrapper::default_bufferPreDraw)
        .def("pipePreDraw", &vrj::GlApp::pipePreDraw, &vrj_GlApp_Wrapper::default_pipePreDraw)
        .def("init", &vrj::App::init, &vrj_GlApp_Wrapper::default_init)
        .def("apiInit", &vrj::App::apiInit, &vrj_GlApp_Wrapper::default_apiInit)
        .def("exit", &vrj::App::exit, &vrj_GlApp_Wrapper::default_exit)
        .def("preFrame", &vrj::App::preFrame, &vrj_GlApp_Wrapper::default_preFrame)
        .def("latePreFrame", &vrj::App::latePreFrame, &vrj_GlApp_Wrapper::default_latePreFrame)
        .def("intraFrame", &vrj::App::intraFrame, &vrj_GlApp_Wrapper::default_intraFrame)
        .def("postFrame", &vrj::App::postFrame, &vrj_GlApp_Wrapper::default_postFrame)
        .def("reset", &vrj::App::reset, &vrj_GlApp_Wrapper::default_reset)
        .def("focusChanged", &vrj::App::focusChanged, &vrj_GlApp_Wrapper::default_focusChanged)
        .def("getDrawScaleFactor", &vrj::App::getDrawScaleFactor, &vrj_GlApp_Wrapper::default_getDrawScaleFactor)
        .def("configCanHandle", &vrj::App::configCanHandle, &vrj_GlApp_Wrapper::default_configCanHandle)
        .def("depSatisfied", &vrj::App::depSatisfied, &vrj_GlApp_Wrapper::default_depSatisfied)
        .def("configProcessPending", &jccl::ConfigElementHandler::configProcessPending, &vrj_GlApp_Wrapper::default_configProcessPending)
        .def("haveFocus", &vrj::App::haveFocus)
        .def("setFocus", &vrj::App::setFocus)
    ;

}
