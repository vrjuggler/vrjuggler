// PyJuggler is (C) Copyright 2002-2005 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <vrj/Kernel/App.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/DrawManager.h>
#include <pyjutil/InterpreterGuard.h>
#include <pyjutil/Debug.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vrj_App_Wrapper: vrj::App
{
    vrj_App_Wrapper(PyObject* self_, const vrj::App& p0):
        vrj::App(p0), self(self_) {}

    vrj_App_Wrapper(PyObject* self_, vrj::Kernel* p0 = NULL):
        vrj::App(p0), self(self_) {}

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

    bool configCanHandle(jccl::ConfigElementPtr p0) {
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

    bool default_configCanHandle(jccl::ConfigElementPtr p0) {
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

    bool configAdd(jccl::ConfigElementPtr p0) {
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

    bool default_configAdd(jccl::ConfigElementPtr p0) {
        return vrj::App::configAdd(p0);
    }

    bool configRemove(jccl::ConfigElementPtr p0) {
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

    bool default_configRemove(jccl::ConfigElementPtr p0) {
        return vrj::App::configRemove(p0);
    }

    vrj::DrawManager* getDrawManager() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vrj_App_Wrapper::getDrawManager()\n",
                                 "vrj_App_Wrapper::getDrawManager() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vrj::DrawManager* >(self, "getDrawManager");
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
        return jccl::ConfigElementHandler::configProcessPending();
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_App()
{
    class_< vrj::App, boost::noncopyable, pyj::vrj_App_Wrapper >("App",
         "Encapsulates the actual application.\n\n"
         "This defines the base class for all API-specific application\n"
         "object types.  The interface given is what the VR Juggler kernel\n"
         "expects in order to communicate with the application.  Most of\n"
         "the application's interface will be defined in the derived\n"
         "API-specific classes.\n\n"
         "Users should write their application objects as subclasses of the\n"
         "API-specific classes.  Overriding the methods in this class and in\n"
         "the API-specific subclasses is the method by which the application\n"
         "programmer interfaces with VR Juggler.\n\n"
         "The VR Juggler kernel control loop will look similar to this:\n\n"
         "while drawing:\n"
         "   app_obj.preFrame()\n"
         "   app_obj.latePreFrame()\n"
         "   draw()\n"
         "   app_obj.intraFrame()\n"
         "   sync()\n"
         "   app_obj.postFrame()\n\n"
         "   updateAllDeices()\n\n"
         "One time through this loop is a \"Juggler Frame.\"\n\n"
         "See also: vrj.Kernel"
         ,
         init< optional< vrj::Kernel* > >(
            "__init__()\n"
            "Default constructor.  Uses vrj.Kernel.instance().\n\n"
            "__init__(kernel)\n"
            "Arguments:\n"
            "kernel -- The vrj.Kernel instance that is active (so the\n"
            "          application has easy access to the kernel)."
         )
        )
        .def_readwrite("mKernel", &vrj::App::mKernel)
        .def_readwrite("mHaveFocus", &vrj::App::mHaveFocus)
        .def("init", &vrj::App::init, &pyj::vrj_App_Wrapper::default_init,
             "init()\n"
             "Application initialization function.  Execute any\n"
             "initialization needed before the graphics API is started.\n\n"
             "Note:\n"
             "Derived classes MUST call the base class version of this\n"
             "method."
         )
        .def("apiInit", &vrj::App::apiInit,
             &pyj::vrj_App_Wrapper::default_apiInit,
             "apiInit()\n"
             "Application graphics API initialization function.  Execute any\n"
             "initialization needed after the graphics API is started but\n"
             "before the Draw Manager starts the rendering loop(s)."
         )
        .def("exit", &vrj::App::exit, &pyj::vrj_App_Wrapper::default_exit,
             "exit()\n"
             "Executes any final clean-up needed for the application before\n"
             "exiting."
         )
        .def("preFrame", &vrj::App::preFrame,
             &pyj::vrj_App_Wrapper::default_preFrame,
             "preFrame()\n"
             "Executes any code needed to set up the application object\n"
             "state prior to rendering the scene.  This is invoked by the\n"
             "kernel once per pass through the Juggler frame loop.  This is\n"
             "called after input device updates but before the start of\n"
             "rendering a new frame of the scene."
         )
        .def("latePreFrame", &vrj::App::latePreFrame,
             &pyj::vrj_App_Wrapper::default_latePreFrame,
             "latePreFrame()\n"
             "Function called after preFrame() and application-specific data\n"
             "synchronization (in a cluster conifguration) but before the\n"
             "start of a new frame.\n\n"
             "Note:\n"
             "This is required because we cannot update data during the\n"
             "rendering process since it might be using multiple threads."
         )
        .def("intraFrame", &vrj::App::intraFrame,
             &pyj::vrj_App_Wrapper::default_intraFrame,
             "intraFrame()\n"
             "Function called <b>during</b> this application's drawing time.\n"
             "This can be used for \"free\" parallel processing, but it\n"
             "introduces a critical section to the code.  If the rendering\n"
             "process is reading state data, this method should not be\n"
             "modifying any of that data.  Instead, a double- or\n"
             "triple-buffering scheme must be used to allow parallel\n"
             "reading and writing of all application state used for rendering."
         )
        .def("postFrame", &vrj::App::postFrame,
             &pyj::vrj_App_Wrapper::default_postFrame,
             "postFrame()\n"
             "Function called before updating input devices but after the\n"
             "frame rendering is complete."
         )
        .def("reset", &vrj::App::reset, &pyj::vrj_App_Wrapper::default_reset,
             "reset()\n"
             "Resets the application.  This is used when the kernel (or\n"
             "applications) would like this application object to reset to\n"
             "its initial state.  This is one of the only methods that can\n"
             "be safe for cross-calls from other application object\n"
             "interface methodss.  That is, of course, as long as the\n"
             "override of this method is implemented in a safe manner.  The\n"
             "override should be implemented such that it can be invoked at\n"
             "any time during the Juggler frame loop by the kernel or from\n"
             "a method such as preFrame() or postFrame() by the application\n"
             "object itself."
         )
        .def("focusChanged", &vrj::App::focusChanged,
             &pyj::vrj_App_Wrapper::default_focusChanged,
             "focusChanged()\n"
             "Called by the kernel when the focus state changes."
         )
        .def("getDrawScaleFactor", &vrj::App::getDrawScaleFactor,
             &pyj::vrj_App_Wrapper::default_getDrawScaleFactor,
             "getDrawScaleFactor() -> float\n"
             "Returns the scale factor to convert from Juggler units\n"
             "(meters) to application units.  Internally, VR Juggler stores\n"
             "and processes all position values in meterrs.  The scale\n"
             "factor returned by this method is used by VR Juggler to scale\n"
             "the rendering state from meters to whatever units this\n"
             "application wants to use.  For example, to render in feet,\n"
             "return 3.28 (gadget.PositionUnitConversion.ConvertToFeet)."
         )
        .def("configCanHandle",
             (bool (vrj::App::*)(jccl::ConfigElementPtr) )&vrj::App::configCanHandle,
             &pyj::vrj_App_Wrapper::default_configCanHandle,
             "configCanHandle(element) -> Boolean\n"
             "Defaults to handling nothing.\n\n"
             "Arguments:\n"
             "element -- An instance of jccl.ConfigElement."
         )
        .def("depSatisfied", &vrj::App::depSatisfied,
             &pyj::vrj_App_Wrapper::default_depSatisfied,
             "depSatisfied() -> Boolean\n"
             "Are any application dependencies satisified?  If this\n"
             "application requires anything special of the system for\n"
             "successful initialization, check it here.  If the return value\n"
             "is False, then this application will not be started yet.  If\n"
             "the return value is True, this application will be allowed to\n"
             "enter the system."
         )
//        .def("getDrawManager", pure_virtual(&vrj::App::getDrawManager))
        .def("configProcessPending",
             (int (jccl::ConfigElementHandler::*)() )&jccl::ConfigElementHandler::configProcessPending,
             (int (pyj::vrj_App_Wrapper::*)())&pyj::vrj_App_Wrapper::default_configProcessPending,
             "configProcessPending() -> int\n"
             "Inherited from jccl.ConfigElementHandler and not overridden."
         )
        .def("haveFocus", &vrj::App::haveFocus,
             "haveFocus() -> Boolean\n"
             "Does this application currently have focus?  If an application\n"
             "has focus, the user may be attempting to interact with it, so\n"
             "the application should process input.  If not, the user is\n"
             "not interacting with it, so ignore all input.  However, the\n"
             "user may still be viewing the application, so render, update\n"
             "animations, etc.  This is akin to the way that a user can only\n"
             "interact with a GUI window that has focus."
         )
        .def("setFocus", &vrj::App::setFocus,
             "setFocus(newState)\n"
             "Sets the focus state.\n\n"
             "Post-condition:\n"
             "If the focus state has changed, then focusChanged() is\n"
             "called.\n\n"
             "Arguments:\n"
             "newState -- A Boolean value indicating whether this application\n"
             "            now has focus."
         )
    ;

}
