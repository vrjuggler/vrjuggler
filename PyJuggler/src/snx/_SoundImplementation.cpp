// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <snx/SoundImplementation.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct snx_SoundImplementation_Wrapper: snx::SoundImplementation
{
    snx_SoundImplementation_Wrapper(PyObject* self_, const snx::SoundImplementation& p0):
        snx::SoundImplementation(p0), self(self_) {}

    snx_SoundImplementation_Wrapper(PyObject* self_):
        snx::SoundImplementation(), self(self_) {}

    void clone(snx::ISoundImplementation*& p0) {
        call_method< void >(self, "clone", p0);
    }

    void trigger(const std::string& p0, const int& p1) {
        call_method< void >(self, "trigger", p0, p1);
    }

    void default_trigger_1(const std::string& p0) {
        snx::SoundImplementation::trigger(p0);
    }

    void default_trigger_2(const std::string& p0, const int& p1) {
        snx::SoundImplementation::trigger(p0, p1);
    }

    bool isPlaying(const std::string& p0) {
        return call_method< bool >(self, "isPlaying", p0);
    }

    bool default_isPlaying(const std::string& p0) {
        return snx::SoundImplementation::isPlaying(p0);
    }

    void setRetriggerable(const std::string& p0, bool p1) {
        call_method< void >(self, "setRetriggerable", p0, p1);
    }

    void default_setRetriggerable(const std::string& p0, bool p1) {
        snx::SoundImplementation::setRetriggerable(p0, p1);
    }

    bool isRetriggerable(const std::string& p0) {
        return call_method< bool >(self, "isRetriggerable", p0);
    }

    bool default_isRetriggerable(const std::string& p0) {
        return snx::SoundImplementation::isRetriggerable(p0);
    }

    void stop(const std::string& p0) {
        call_method< void >(self, "stop", p0);
    }

    void default_stop(const std::string& p0) {
        snx::SoundImplementation::stop(p0);
    }

    void pause(const std::string& p0) {
        call_method< void >(self, "pause", p0);
    }

    void default_pause(const std::string& p0) {
        snx::SoundImplementation::pause(p0);
    }

    void unpause(const std::string& p0) {
        call_method< void >(self, "unpause", p0);
    }

    void default_unpause(const std::string& p0) {
        snx::SoundImplementation::unpause(p0);
    }

    bool isPaused(const std::string& p0) {
        return call_method< bool >(self, "isPaused", p0);
    }

    bool default_isPaused(const std::string& p0) {
        return snx::SoundImplementation::isPaused(p0);
    }

    void setAmbient(const std::string& p0, bool p1) {
        call_method< void >(self, "setAmbient", p0, p1);
    }

    void default_setAmbient_1(const std::string& p0) {
        snx::SoundImplementation::setAmbient(p0);
    }

    void default_setAmbient_2(const std::string& p0, bool p1) {
        snx::SoundImplementation::setAmbient(p0, p1);
    }

    bool isAmbient(const std::string& p0) {
        return call_method< bool >(self, "isAmbient", p0);
    }

    bool default_isAmbient(const std::string& p0) {
        return snx::SoundImplementation::isAmbient(p0);
    }

    void setPitchBend(const std::string& p0, float p1) {
        call_method< void >(self, "setPitchBend", p0, p1);
    }

    void default_setPitchBend(const std::string& p0, float p1) {
        snx::SoundImplementation::setPitchBend(p0, p1);
    }

    void setVolume(const std::string& p0, float p1) {
        call_method< void >(self, "setVolume", p0, p1);
    }

    void default_setVolume(const std::string& p0, float p1) {
        snx::SoundImplementation::setVolume(p0, p1);
    }

    void setCutoff(const std::string& p0, float p1) {
        call_method< void >(self, "setCutoff", p0, p1);
    }

    void default_setCutoff(const std::string& p0, float p1) {
        snx::SoundImplementation::setCutoff(p0, p1);
    }

    void setPosition(const std::string& p0, float p1, float p2, float p3) {
        call_method< void >(self, "setPosition", p0, p1, p2, p3);
    }

    void default_setPosition(const std::string& p0, float p1, float p2, float p3) {
        snx::SoundImplementation::setPosition(p0, p1, p2, p3);
    }

    void getPosition(const std::string& p0, float& p1, float& p2, float& p3) {
        call_method< void >(self, "getPosition", p0, p1, p2, p3);
    }

    void default_getPosition(const std::string& p0, float& p1, float& p2, float& p3) {
        snx::SoundImplementation::getPosition(p0, p1, p2, p3);
    }

    void setListenerPosition(const gmtl::Matrix44f& p0) {
        call_method< void >(self, "setListenerPosition", p0);
    }

    void default_setListenerPosition(const gmtl::Matrix44f& p0) {
        snx::SoundImplementation::setListenerPosition(p0);
    }

    void getListenerPosition(gmtl::Matrix44f& p0) {
        call_method< void >(self, "getListenerPosition", p0);
    }

    void default_getListenerPosition(gmtl::Matrix44f& p0) {
        snx::SoundImplementation::getListenerPosition(p0);
    }

    int startAPI() {
        return call_method< int >(self, "startAPI");
    }

    bool isStarted() const {
        return call_method< bool >(self, "isStarted");
    }

    void shutdownAPI() {
        call_method< void >(self, "shutdownAPI");
    }

    void default_shutdownAPI() {
        snx::SoundImplementation::shutdownAPI();
    }

    void configure(const snx::SoundAPIInfo& p0) {
        call_method< void >(self, "configure", p0);
    }

    void default_configure(const snx::SoundAPIInfo& p0) {
        snx::SoundImplementation::configure(p0);
    }

    void configure(const std::string& p0, const snx::SoundInfo& p1) {
        call_method< void >(self, "configure", p0, p1);
    }

    void default_configure(const std::string& p0, const snx::SoundInfo& p1) {
        snx::SoundImplementation::configure(p0, p1);
    }

    void remove(const std::string& p0) {
        call_method< void >(self, "remove", p0);
    }

    void default_remove(const std::string& p0) {
        snx::SoundImplementation::remove(p0);
    }

    void step(const float& p0) {
        call_method< void >(self, "step", p0);
    }

    void default_step(const float& p0) {
        snx::SoundImplementation::step(p0);
    }

    void clear() {
        call_method< void >(self, "clear");
    }

    void default_clear() {
        snx::SoundImplementation::clear();
    }

    void bindAll() {
        call_method< void >(self, "bindAll");
    }

    void default_bindAll() {
        snx::SoundImplementation::bindAll();
    }

    void unbindAll() {
        call_method< void >(self, "unbindAll");
    }

    void default_unbindAll() {
        snx::SoundImplementation::unbindAll();
    }

    void bind(const std::string& p0) {
        call_method< void >(self, "bind", p0);
    }

    void unbind(const std::string& p0) {
        call_method< void >(self, "unbind", p0);
    }

    snx::SoundInfo& lookup(const std::string& p0) {
        return call_method< snx::SoundInfo& >(self, "lookup", p0);
    }

    snx::SoundInfo& default_lookup(const std::string& p0) {
        return snx::SoundImplementation::lookup(p0);
    }

    void setName(const std::string& p0) {
        call_method< void >(self, "setName", p0);
    }

    void default_setName(const std::string& p0) {
        snx::SoundImplementation::setName(p0);
    }

    std::string& name() {
        return call_method< std::string& >(self, "name");
    }

    std::string& default_name() {
        return snx::SoundImplementation::name();
    }

    void destroy() {
        call_method< void >(self, "destroy");
    }

    PyObject* self;
};


}// namespace pyj


// Module ======================================================================
void _Export_SoundImplementation()
{
    class_< snx::SoundImplementation, boost::noncopyable, pyj::snx_SoundImplementation_Wrapper >("SoundImplementation", init<  >())
        .def("clone", pure_virtual((void (snx::SoundImplementation::*)(snx::ISoundImplementation*&) )&snx::SoundImplementation::clone))
        .def("trigger", (void (snx::SoundImplementation::*)(const std::string&, const int&) )&snx::SoundImplementation::trigger, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, const int&))&pyj::snx_SoundImplementation_Wrapper::default_trigger_2)
        .def("trigger", (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_trigger_1)
        .def("isPlaying", (bool (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::isPlaying, (bool (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_isPlaying)
        .def("setRetriggerable", (void (snx::SoundImplementation::*)(const std::string&, bool) )&snx::SoundImplementation::setRetriggerable, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, bool))&pyj::snx_SoundImplementation_Wrapper::default_setRetriggerable)
        .def("isRetriggerable", (bool (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::isRetriggerable, (bool (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_isRetriggerable)
        .def("stop", (void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::stop, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_stop)
        .def("pause", (void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::pause, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_pause)
        .def("unpause", (void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::unpause, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_unpause)
        .def("isPaused", (bool (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::isPaused, (bool (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_isPaused)
        .def("setAmbient", (void (snx::SoundImplementation::*)(const std::string&, bool) )&snx::SoundImplementation::setAmbient, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, bool))&pyj::snx_SoundImplementation_Wrapper::default_setAmbient_2)
        .def("setAmbient", (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_setAmbient_1)
        .def("isAmbient", (bool (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::isAmbient, (bool (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_isAmbient)
        .def("setPitchBend", (void (snx::SoundImplementation::*)(const std::string&, float) )&snx::SoundImplementation::setPitchBend, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, float))&pyj::snx_SoundImplementation_Wrapper::default_setPitchBend)
        .def("setVolume", (void (snx::SoundImplementation::*)(const std::string&, float) )&snx::SoundImplementation::setVolume, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, float))&pyj::snx_SoundImplementation_Wrapper::default_setVolume)
        .def("setCutoff", (void (snx::SoundImplementation::*)(const std::string&, float) )&snx::SoundImplementation::setCutoff, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, float))&pyj::snx_SoundImplementation_Wrapper::default_setCutoff)
        .def("setPosition", (void (snx::SoundImplementation::*)(const std::string&, float, float, float) )&snx::SoundImplementation::setPosition, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, float, float, float))&pyj::snx_SoundImplementation_Wrapper::default_setPosition)
        .def("getPosition", (void (snx::SoundImplementation::*)(const std::string&, float&, float&, float&) )&snx::SoundImplementation::getPosition, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, float&, float&, float&))&pyj::snx_SoundImplementation_Wrapper::default_getPosition)
        .def("setListenerPosition", (void (snx::SoundImplementation::*)(const gmtl::Matrix44f&) )&snx::SoundImplementation::setListenerPosition, (void (pyj::snx_SoundImplementation_Wrapper::*)(const gmtl::Matrix44f&))&pyj::snx_SoundImplementation_Wrapper::default_setListenerPosition)
        .def("getListenerPosition", (void (snx::SoundImplementation::*)(gmtl::Matrix44f&) )&snx::SoundImplementation::getListenerPosition, (void (pyj::snx_SoundImplementation_Wrapper::*)(gmtl::Matrix44f&))&pyj::snx_SoundImplementation_Wrapper::default_getListenerPosition)
        .def("startAPI", pure_virtual((int (snx::SoundImplementation::*)() )&snx::SoundImplementation::startAPI))
        .def("isStarted", pure_virtual((bool (snx::SoundImplementation::*)() const)&snx::SoundImplementation::isStarted))
        .def("shutdownAPI", (void (snx::SoundImplementation::*)() )&snx::SoundImplementation::shutdownAPI, (void (pyj::snx_SoundImplementation_Wrapper::*)())&pyj::snx_SoundImplementation_Wrapper::default_shutdownAPI)
        .def("configure", (void (snx::SoundImplementation::*)(const snx::SoundAPIInfo&) )&snx::SoundImplementation::configure, (void (pyj::snx_SoundImplementation_Wrapper::*)(const snx::SoundAPIInfo&))&pyj::snx_SoundImplementation_Wrapper::default_configure)
        .def("configure", (void (snx::SoundImplementation::*)(const std::string&, const snx::SoundInfo&) )&snx::SoundImplementation::configure, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, const snx::SoundInfo&))&pyj::snx_SoundImplementation_Wrapper::default_configure)
        .def("remove", (void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::remove, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_remove)
        .def("step", (void (snx::SoundImplementation::*)(const float&) )&snx::SoundImplementation::step, (void (pyj::snx_SoundImplementation_Wrapper::*)(const float&))&pyj::snx_SoundImplementation_Wrapper::default_step)
        .def("clear", (void (snx::SoundImplementation::*)() )&snx::SoundImplementation::clear, (void (pyj::snx_SoundImplementation_Wrapper::*)())&pyj::snx_SoundImplementation_Wrapper::default_clear)
        .def("bindAll", (void (snx::SoundImplementation::*)() )&snx::SoundImplementation::bindAll, (void (pyj::snx_SoundImplementation_Wrapper::*)())&pyj::snx_SoundImplementation_Wrapper::default_bindAll)
        .def("unbindAll", (void (snx::SoundImplementation::*)() )&snx::SoundImplementation::unbindAll, (void (pyj::snx_SoundImplementation_Wrapper::*)())&pyj::snx_SoundImplementation_Wrapper::default_unbindAll)
        .def("bind", pure_virtual((void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::bind))
        .def("unbind", pure_virtual((void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::unbind))
        .def("lookup", (snx::SoundInfo& (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::lookup, (snx::SoundInfo& (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_lookup, return_internal_reference< 1 >())
        .def("setName", (void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::setName, (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_setName)
        .def("name", (std::string& (snx::SoundImplementation::*)() )&snx::SoundImplementation::name, (std::string& (pyj::snx_SoundImplementation_Wrapper::*)())&pyj::snx_SoundImplementation_Wrapper::default_name, return_internal_reference< 1 >())
        .def("copy", &snx::SoundImplementation::copy)
    ;

}
