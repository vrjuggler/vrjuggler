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
#include <math.h>
#include <boost/python.hpp>
#include <vrj/Draw/OSG/OsgApp.h>
#include <vrj/Kernel/Kernel.h>
#include <pyjutil/InterpreterGuard.h>
#include <pyjutil/Debug.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


struct vrj_OsgApp_Wrapper: vrj::OsgApp
{
    vrj_OsgApp_Wrapper(PyObject* self_, vrj::Kernel * p0):
        vrj::OsgApp(p0), self(self_) {}

    vrj_OsgApp_Wrapper(PyObject* self_):
        vrj::OsgApp(), self(self_) {}

    virtual ~vrj_OsgApp_Wrapper()
    {
    }

    void initScene() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::initScene()\n",
                                 "vrj_OsgApp_Wrapper::initScene() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "initScene");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    osg::Group* getScene() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::getScene()\n",
                                 "vrj_OsgApp_Wrapper::getScene() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< osg::Group* >(self, "getScene");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return NULL;
    }

    void configSceneView(osgUtil::SceneView* sv) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::configSceneView()\n",
                                 "vrj_OsgApp_Wrapper::configSceneView() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "configSceneView", sv);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_configSceneView(osgUtil::SceneView* sv) {
        vrj::OsgApp::configSceneView(sv);
    }

    void draw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::draw()\n",
                                 "vrj_OsgApp_Wrapper::draw() done.\n");
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

    void default_draw() {
        vrj::OsgApp::draw();
    }

    void contextInit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::contextInit()\n",
                                 "vrj_OsgApp_Wrapper::contextInit() done.\n");
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
        vrj::OsgApp::contextInit();
    }

    void contextClose() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::contextClose()\n",
                                 "vrj_OsgApp_Wrapper::contextClose() done.\n");
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
        vrj::OsgApp::contextClose();
    }

    void contextPreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::contextPreDraw()\n",
                                 "vrj_OsgApp_Wrapper::contextPreDraw() done.\n");
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
        vrj::OsgApp::contextPreDraw();
    }

    void contextPostDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::contextPostDraw()\n",
                                 "vrj_OsgApp_Wrapper::contextPostDraw() done.\n");
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
        vrj::OsgApp::contextPostDraw();
    }

    void bufferPreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::bufferPreDraw()\n",
                                 "vrj_OsgApp_Wrapper::bufferPreDraw() done.\n");
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
        vrj::OsgApp::bufferPreDraw();
    }

    void pipePreDraw() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::pipePreDraw()\n",
                                 "vrj_OsgApp_Wrapper::pipePreDraw() done.\n");
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
        vrj::OsgApp::pipePreDraw();
    }

    void init() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::init()\n",
                                 "vrj_OsgApp_Wrapper::init() done.\n");
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
        vrj::OsgApp::init();
    }

    void apiInit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::apiInit()\n",
                                 "vrj_OsgApp_Wrapper::apiInit() done.\n");
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
        vrj::OsgApp::apiInit();
    }

    void exit() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::exit()\n",
                                 "vrj_OsgApp_Wrapper::exit() done.\n");
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
        vrj::OsgApp::exit();
    }

    void preFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::preFrame()\n",
                                 "vrj_OsgApp_Wrapper::preFrame() done.\n");
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
        vrj::OsgApp::preFrame();
    }

    void latePreFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::latePreFrame()\n",
                                 "vrj_OsgApp_Wrapper::latePreFrame() done.\n");
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
        vrj::OsgApp::latePreFrame();
    }

    void intraFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::intraFrame()\n",
                                 "vrj_OsgApp_Wrapper::intraFrame() done.\n");
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
        vrj::OsgApp::intraFrame();
    }

    void postFrame() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::postFrame()\n",
                                 "vrj_OsgApp_Wrapper::postFrame() done.\n");
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
        vrj::OsgApp::postFrame();
    }

    void reset() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::reset()\n",
                                 "vrj_OsgApp_Wrapper::reset() done.\n");
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
        vrj::OsgApp::reset();
    }

    void focusChanged() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::focusChanged()\n",
                                 "vrj_OsgApp_Wrapper::focusChanged() done.\n");
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
        vrj::OsgApp::focusChanged();
    }

    float getDrawScaleFactor() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::getDrawScaleFactor()\n",
                                 "vrj_OsgApp_Wrapper::getDrawScaleFactor() done.\n");
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
        return vrj::OsgApp::getDrawScaleFactor();
    }

    bool configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::configCanHandle()\n",
                                 "vrj_OsgApp_Wrapper::configCanHandle() done.\n");
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
        return vrj::OsgApp::configCanHandle(p0);
    }

    bool depSatisfied() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::depSatisfied()\n",
                                 "vrj_OsgApp_Wrapper::depSatisfied() done.\n");
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
        return vrj::OsgApp::depSatisfied();
    }

    bool configAdd(boost::shared_ptr<jccl::ConfigElement> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::configAdd()\n",
                                 "vrj_OsgApp_Wrapper::configAdd() done.\n");
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
        return vrj::OsgApp::configAdd(p0);
    }

    bool configRemove(boost::shared_ptr<jccl::ConfigElement> p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::configRemove()\n",
                                 "vrj_OsgApp_Wrapper::configRemove() done.\n");
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
        return vrj::OsgApp::configRemove(p0);
    }

    int configProcessPending() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_OsgApp_Wrapper::configProcessPending()\n",
                                 "vrj_OsgApp_Wrapper::configProcessPending() done.\n");
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
        return vrj::OsgApp::configProcessPending();
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
void _Export_OsgApp()
{
    class_< vrj::OsgApp, bases< vrj::GlApp >, boost::noncopyable, vrj_OsgApp_Wrapper >("OsgApp", init<  >())
        .def(init< vrj::Kernel * >())
        .def_readwrite("mKernel", &vrj::App::mKernel)
        .def_readwrite("mHaveFocus", &vrj::App::mHaveFocus)
        .def("configSceneView", &vrj::OsgApp::configSceneView, &vrj_OsgApp_Wrapper::default_configSceneView)
        .def("draw", &vrj::OsgApp::draw, &vrj_OsgApp_Wrapper::default_draw)
        .def("contextInit", &vrj::OsgApp::contextInit, &vrj_OsgApp_Wrapper::default_contextInit)
        .def("contextClose", &vrj::OsgApp::contextClose, &vrj_OsgApp_Wrapper::default_contextClose)
        .def("contextPreDraw", &vrj::OsgApp::contextPreDraw, &vrj_OsgApp_Wrapper::default_contextPreDraw)
        .def("contextPostDraw", &vrj::GlApp::contextPostDraw, &vrj_OsgApp_Wrapper::default_contextPostDraw)
        .def("bufferPreDraw", &vrj::OsgApp::bufferPreDraw, &vrj_OsgApp_Wrapper::default_bufferPreDraw)
        .def("pipePreDraw", &vrj::OsgApp::pipePreDraw, &vrj_OsgApp_Wrapper::default_pipePreDraw)
        .def("init", &vrj::OsgApp::init, &vrj_OsgApp_Wrapper::default_init)
        .def("apiInit", &vrj::App::apiInit, &vrj_OsgApp_Wrapper::default_apiInit)
        .def("exit", &vrj::App::exit, &vrj_OsgApp_Wrapper::default_exit)
        .def("preFrame", &vrj::App::preFrame, &vrj_OsgApp_Wrapper::default_preFrame)
        .def("latePreFrame", &vrj::App::latePreFrame, &vrj_OsgApp_Wrapper::default_latePreFrame)
        .def("intraFrame", &vrj::App::intraFrame, &vrj_OsgApp_Wrapper::default_intraFrame)
        .def("postFrame", &vrj::App::postFrame, &vrj_OsgApp_Wrapper::default_postFrame)
        .def("reset", &vrj::App::reset, &vrj_OsgApp_Wrapper::default_reset)
        .def("focusChanged", &vrj::App::focusChanged, &vrj_OsgApp_Wrapper::default_focusChanged)
        .def("getDrawScaleFactor", &vrj::App::getDrawScaleFactor, &vrj_OsgApp_Wrapper::default_getDrawScaleFactor)
        .def("configCanHandle", &vrj::App::configCanHandle, &vrj_OsgApp_Wrapper::default_configCanHandle)
        .def("depSatisfied", &vrj::App::depSatisfied, &vrj_OsgApp_Wrapper::default_depSatisfied)
        .def("configProcessPending", &jccl::ConfigElementHandler::configProcessPending, &vrj_OsgApp_Wrapper::default_configProcessPending)
        .def("haveFocus", &vrj::App::haveFocus)
        .def("setFocus", &vrj::App::setFocus)
    ;

}
