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
#include <snx/sonix.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct sonix_Wrapper: sonix
{
    sonix_Wrapper(PyObject* self_, const sonix& p0):
        sonix(p0), self(self_) {}

    void trigger(const std::string& p0, const int& p1) {
        call_method< void >(self, "trigger", p0, p1);
    }

    void default_trigger_1(const std::string& p0) {
        sonix::trigger(p0);
    }

    void default_trigger_2(const std::string& p0, const int& p1) {
        sonix::trigger(p0, p1);
    }

    bool isPlaying(const std::string& p0) {
        return call_method< bool >(self, "isPlaying", p0);
    }

    bool default_isPlaying(const std::string& p0) {
        return sonix::isPlaying(p0);
    }

    void setRetriggerable(const std::string& p0, bool p1) {
        call_method< void >(self, "setRetriggerable", p0, p1);
    }

    void default_setRetriggerable(const std::string& p0, bool p1) {
        sonix::setRetriggerable(p0, p1);
    }

    bool isRetriggerable(const std::string& p0) {
        return call_method< bool >(self, "isRetriggerable", p0);
    }

    bool default_isRetriggerable(const std::string& p0) {
        return sonix::isRetriggerable(p0);
    }

    void stop(const std::string& p0) {
        call_method< void >(self, "stop", p0);
    }

    void default_stop(const std::string& p0) {
        sonix::stop(p0);
    }

    void pause(const std::string& p0) {
        call_method< void >(self, "pause", p0);
    }

    void default_pause(const std::string& p0) {
        sonix::pause(p0);
    }

    void unpause(const std::string& p0) {
        call_method< void >(self, "unpause", p0);
    }

    void default_unpause(const std::string& p0) {
        sonix::unpause(p0);
    }

    bool isPaused(const std::string& p0) {
        return call_method< bool >(self, "isPaused", p0);
    }

    bool default_isPaused(const std::string& p0) {
        return sonix::isPaused(p0);
    }

    void setAmbient(const std::string& p0, bool p1) {
        call_method< void >(self, "setAmbient", p0, p1);
    }

    void default_setAmbient_1(const std::string& p0) {
        sonix::setAmbient(p0);
    }

    void default_setAmbient_2(const std::string& p0, bool p1) {
        sonix::setAmbient(p0, p1);
    }

    bool isAmbient(const std::string& p0) {
        return call_method< bool >(self, "isAmbient", p0);
    }

    bool default_isAmbient(const std::string& p0) {
        return sonix::isAmbient(p0);
    }

    void setPitchBend(const std::string& p0, float p1) {
        call_method< void >(self, "setPitchBend", p0, p1);
    }

    void default_setPitchBend(const std::string& p0, float p1) {
        sonix::setPitchBend(p0, p1);
    }

    void setVolume(const std::string& p0, float p1) {
        call_method< void >(self, "setVolume", p0, p1);
    }

    void default_setVolume(const std::string& p0, float p1) {
        sonix::setVolume(p0, p1);
    }

    void setCutoff(const std::string& p0, float p1) {
        call_method< void >(self, "setCutoff", p0, p1);
    }

    void default_setCutoff(const std::string& p0, float p1) {
        sonix::setCutoff(p0, p1);
    }

    void setPosition(const std::string& p0, const float& p1, const float& p2, const float& p3) {
        call_method< void >(self, "setPosition", p0, p1, p2, p3);
    }

    void default_setPosition(const std::string& p0, const float& p1, const float& p2, const float& p3) {
        sonix::setPosition(p0, p1, p2, p3);
    }

    void getPosition(const std::string& p0, float& p1, float& p2, float& p3) {
        call_method< void >(self, "getPosition", p0, p1, p2, p3);
    }

    void default_getPosition(const std::string& p0, float& p1, float& p2, float& p3) {
        sonix::getPosition(p0, p1, p2, p3);
    }

    void setListenerPosition(const gmtl::Matrix44f& p0) {
        call_method< void >(self, "setListenerPosition", p0);
    }

    void default_setListenerPosition(const gmtl::Matrix44f& p0) {
        sonix::setListenerPosition(p0);
    }

    void getListenerPosition(gmtl::Matrix44f& p0) {
        call_method< void >(self, "getListenerPosition", p0);
    }

    void default_getListenerPosition(gmtl::Matrix44f& p0) {
        sonix::getListenerPosition(p0);
    }

    void changeAPI(const std::string& p0) {
        call_method< void >(self, "changeAPI", p0);
    }

    void default_changeAPI(const std::string& p0) {
        sonix::changeAPI(p0);
    }

    void configure(const snx::SoundAPIInfo& p0) {
        call_method< void >(self, "configure", p0);
    }

    void default_configure(const snx::SoundAPIInfo& p0) {
        sonix::configure(p0);
    }

    void configure(const std::string& p0, const snx::SoundInfo& p1) {
        call_method< void >(self, "configure", p0, p1);
    }

    void default_configure(const std::string& p0, const snx::SoundInfo& p1) {
        sonix::configure(p0, p1);
    }

    void remove(const std::string p0) {
        call_method< void >(self, "remove", p0);
    }

    void default_remove(const std::string p0) {
        sonix::remove(p0);
    }

    void step(const float& p0) {
        call_method< void >(self, "step", p0);
    }

    void default_step(const float& p0) {
        sonix::step(p0);
    }

    PyObject* self;
};


}// namespace pyj


// Module ======================================================================
void _Export_sonix()
{
    class_< sonix, pyj::sonix_Wrapper, boost::noncopyable >("sonix", no_init)
        .def("trigger", &sonix::trigger, &pyj::sonix_Wrapper::default_trigger_2)
        .def("trigger", &pyj::sonix_Wrapper::default_trigger_1)
        .def("isPlaying", &sonix::isPlaying, &pyj::sonix_Wrapper::default_isPlaying)
        .def("setRetriggerable", &sonix::setRetriggerable, &pyj::sonix_Wrapper::default_setRetriggerable)
        .def("isRetriggerable", &sonix::isRetriggerable, &pyj::sonix_Wrapper::default_isRetriggerable)
        .def("stop", &sonix::stop, &pyj::sonix_Wrapper::default_stop)
        .def("pause", &sonix::pause, &pyj::sonix_Wrapper::default_pause)
        .def("unpause", &sonix::unpause, &pyj::sonix_Wrapper::default_unpause)
        .def("isPaused", &sonix::isPaused, &pyj::sonix_Wrapper::default_isPaused)
        .def("setAmbient", &sonix::setAmbient, &pyj::sonix_Wrapper::default_setAmbient_2)
        .def("setAmbient", &pyj::sonix_Wrapper::default_setAmbient_1)
        .def("isAmbient", &sonix::isAmbient, &pyj::sonix_Wrapper::default_isAmbient)
        .def("setPitchBend", &sonix::setPitchBend, &pyj::sonix_Wrapper::default_setPitchBend)
        .def("setVolume", &sonix::setVolume, &pyj::sonix_Wrapper::default_setVolume)
        .def("setCutoff", &sonix::setCutoff, &pyj::sonix_Wrapper::default_setCutoff)
        .def("setPosition", &sonix::setPosition, &pyj::sonix_Wrapper::default_setPosition)
        .def("getPosition", &sonix::getPosition, &pyj::sonix_Wrapper::default_getPosition)
        .def("setListenerPosition", &sonix::setListenerPosition, &pyj::sonix_Wrapper::default_setListenerPosition)
        .def("getListenerPosition", &sonix::getListenerPosition, &pyj::sonix_Wrapper::default_getListenerPosition)
        .def("changeAPI", &sonix::changeAPI, &pyj::sonix_Wrapper::default_changeAPI)
        .def("configure", (void (sonix::*)(const snx::SoundAPIInfo&) )&sonix::configure, (void (pyj::sonix_Wrapper::*)(const snx::SoundAPIInfo&))&pyj::sonix_Wrapper::default_configure)
        .def("configure", (void (sonix::*)(const std::string&, const snx::SoundInfo&) )&sonix::configure, (void (pyj::sonix_Wrapper::*)(const std::string&, const snx::SoundInfo&))&pyj::sonix_Wrapper::default_configure)
        .def("remove", &sonix::remove, &pyj::sonix_Wrapper::default_remove)
        .def("step", &sonix::step, &pyj::sonix_Wrapper::default_step)
        .def("instance", &sonix::instance, return_value_policy< reference_existing_object >())
        .staticmethod("instance")
    ;

}
