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
#include <vrj/Kernel/App.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/DrawManager.h>
#include <pyjutil/InterpreterGuard.h>
#include <pyjutil/Debug.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


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
        vrj::App::init();
    }

    void apiInit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::apiInit()\n",
                                 "vrj_App_Wrapper::apiInit() done.\n");
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
        vrj::App::apiInit();
    }

    void exit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::exit()\n",
                                 "vrj_App_Wrapper::exit() done.\n");
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
        vrj::App::exit();
    }

    void preFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::preFrame()\n",
                                 "vrj_App_Wrapper::preFrame() done.\n");
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
        vrj::App::preFrame();
    }

    void latePreFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::latePreFrame()\n",
                                 "vrj_App_Wrapper::latePreFrame() done.\n");
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
        vrj::App::latePreFrame();
    }

    void intraFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::intraFrame()\n",
                                 "vrj_App_Wrapper::intraFrame() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "intraFrame");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_intraFrame() {
        vrj::App::intraFrame();
    }

    void postFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::postFrame()\n",
                                 "vrj_App_Wrapper::postFrame() done.\n");
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
        vrj::App::postFrame();
    }

    void reset() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::reset()\n",
                                 "vrj_App_Wrapper::reset() done.\n");
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
        vrj::App::reset();
    }

    void focusChanged() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::focusChanged()\n",
                                 "vrj_App_Wrapper::focusChanged() done.\n");
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
        vrj::App::focusChanged();
    }

    float getDrawScaleFactor() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::getDrawScaleFactor()\n",
                                 "vrj_App_Wrapper::getDrawScaleFactor() done.\n");
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
        return vrj::App::getDrawScaleFactor();
    }

    bool configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configCanHandle()\n",
                                 "vrj_App_Wrapper::configCanHandle() done.\n");
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
        return vrj::App::configCanHandle(p0);
    }

    bool depSatisfied() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::depSatisfied()\n",
                                 "vrj_App_Wrapper::depSatisfied() done.\n");
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
        return vrj::App::depSatisfied();
    }

    bool configAdd(boost::shared_ptr<jccl::ConfigElement> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configAdd()\n",
                                 "vrj_App_Wrapper::configAdd() done.\n");
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
        return vrj::App::configAdd(p0);
    }

    bool configRemove(boost::shared_ptr<jccl::ConfigElement> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configRemove()\n",
                                 "vrj_App_Wrapper::configRemove() done.\n");
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
        return vrj::App::configRemove(p0);
    }

    vrj::DrawManager * getDrawManager() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::getDrawManager()\n",
                                 "vrj_App_Wrapper::getDrawManager() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vrj::DrawManager * >(self, "getDrawManager");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return NULL;
    }

    int configProcessPending() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::configProcessPending()\n",
                                 "vrj_App_Wrapper::configProcessPending() done.\n");
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
        return vrj::App::configProcessPending();
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
void _Export_App()
{
    class_< vrj::App, boost::noncopyable, vrj_App_Wrapper >("App", init<  >())
        .def(init< vrj::Kernel * >())
        .def_readwrite("mKernel", &vrj::App::mKernel)
        .def_readwrite("mHaveFocus", &vrj::App::mHaveFocus)
        .def("init", &vrj::App::init, &vrj_App_Wrapper::default_init)
        .def("apiInit", &vrj::App::apiInit, &vrj_App_Wrapper::default_apiInit)
        .def("exit", &vrj::App::exit, &vrj_App_Wrapper::default_exit)
        .def("preFrame", &vrj::App::preFrame, &vrj_App_Wrapper::default_preFrame)
        .def("latePreFrame", &vrj::App::latePreFrame, &vrj_App_Wrapper::default_latePreFrame)
        .def("intraFrame", &vrj::App::intraFrame, &vrj_App_Wrapper::default_intraFrame)
        .def("postFrame", &vrj::App::postFrame, &vrj_App_Wrapper::default_postFrame)
        .def("reset", &vrj::App::reset, &vrj_App_Wrapper::default_reset)
        .def("focusChanged", &vrj::App::focusChanged, &vrj_App_Wrapper::default_focusChanged)
        .def("getDrawScaleFactor", &vrj::App::getDrawScaleFactor, &vrj_App_Wrapper::default_getDrawScaleFactor)
        .def("configCanHandle", &vrj::App::configCanHandle, &vrj_App_Wrapper::default_configCanHandle)
        .def("depSatisfied", &vrj::App::depSatisfied, &vrj_App_Wrapper::default_depSatisfied)
        .def("configProcessPending", &jccl::ConfigElementHandler::configProcessPending, &vrj_App_Wrapper::default_configProcessPending)
        .def("haveFocus", &vrj::App::haveFocus)
        .def("setFocus", &vrj::App::setFocus)
    ;

}
