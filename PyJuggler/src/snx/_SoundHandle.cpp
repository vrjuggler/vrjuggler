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
#include <snx/SoundHandle.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct snx_SoundHandle_Wrapper: snx::SoundHandle
{
    snx_SoundHandle_Wrapper(PyObject* self_, const snx::SoundHandle& p0):
        snx::SoundHandle(p0), self(self_) {}

    snx_SoundHandle_Wrapper(PyObject* self_):
        snx::SoundHandle(), self(self_) {}

    snx_SoundHandle_Wrapper(PyObject* self_, const std::string& p0):
        snx::SoundHandle(p0), self(self_) {}

    void trigger(const int& p0) {
        call_method< void >(self, "trigger", p0);
    }

    void default_trigger_0() {
        snx::SoundHandle::trigger();
    }

    void default_trigger_1(const int& p0) {
        snx::SoundHandle::trigger(p0);
    }

    bool isPlaying() {
        return call_method< bool >(self, "isPlaying");
    }

    bool default_isPlaying() {
        return snx::SoundHandle::isPlaying();
    }

    void setRetriggerable(bool p0) {
        call_method< void >(self, "setRetriggerable", p0);
    }

    void default_setRetriggerable(bool p0) {
        snx::SoundHandle::setRetriggerable(p0);
    }

    bool isRetriggerable() {
        return call_method< bool >(self, "isRetriggerable");
    }

    bool default_isRetriggerable() {
        return snx::SoundHandle::isRetriggerable();
    }

    void stop() {
        call_method< void >(self, "stop");
    }

    void default_stop() {
        snx::SoundHandle::stop();
    }

    void pause() {
        call_method< void >(self, "pause");
    }

    void default_pause() {
        snx::SoundHandle::pause();
    }

    void unpause() {
        call_method< void >(self, "unpause");
    }

    void default_unpause() {
        snx::SoundHandle::unpause();
    }

    bool isPaused() {
        return call_method< bool >(self, "isPaused");
    }

    bool default_isPaused() {
        return snx::SoundHandle::isPaused();
    }

    void setAmbient(bool p0) {
        call_method< void >(self, "setAmbient", p0);
    }

    void default_setAmbient_0() {
        snx::SoundHandle::setAmbient();
    }

    void default_setAmbient_1(bool p0) {
        snx::SoundHandle::setAmbient(p0);
    }

    bool isAmbient() {
        return call_method< bool >(self, "isAmbient");
    }

    bool default_isAmbient() {
        return snx::SoundHandle::isAmbient();
    }

    void setPitchBend(float p0) {
        call_method< void >(self, "setPitchBend", p0);
    }

    void default_setPitchBend(float p0) {
        snx::SoundHandle::setPitchBend(p0);
    }

    void setVolume(float p0) {
        call_method< void >(self, "setVolume", p0);
    }

    void default_setVolume(float p0) {
        snx::SoundHandle::setVolume(p0);
    }

    void setCutoff(float p0) {
        call_method< void >(self, "setCutoff", p0);
    }

    void default_setCutoff(float p0) {
        snx::SoundHandle::setCutoff(p0);
    }

    void setPosition(const float& p0, const float& p1, const float& p2) {
        call_method< void >(self, "setPosition", p0, p1, p2);
    }

    void default_setPosition(const float& p0, const float& p1, const float& p2) {
        snx::SoundHandle::setPosition(p0, p1, p2);
    }

    void getPosition(float& p0, float& p1, float& p2) {
        call_method< void >(self, "getPosition", p0, p1, p2);
    }

    void default_getPosition(float& p0, float& p1, float& p2) {
        snx::SoundHandle::getPosition(p0, p1, p2);
    }

    void setListenerPosition(const gmtl::Matrix44f& p0) {
        call_method< void >(self, "setListenerPosition", p0);
    }

    void default_setListenerPosition(const gmtl::Matrix44f& p0) {
        snx::SoundHandle::setListenerPosition(p0);
    }

    void getListenerPosition(gmtl::Matrix44f& p0) {
        call_method< void >(self, "getListenerPosition", p0);
    }

    void default_getListenerPosition(gmtl::Matrix44f& p0) {
        snx::SoundHandle::getListenerPosition(p0);
    }

    void configure(const snx::SoundInfo& p0) {
        call_method< void >(self, "configure", p0);
    }

    void default_configure(const snx::SoundInfo& p0) {
        snx::SoundHandle::configure(p0);
    }

    void remove() {
        call_method< void >(self, "remove");
    }

    void default_remove() {
        snx::SoundHandle::remove();
    }

    PyObject* self;
};


}// namespace pyj


// Module ======================================================================
void _Export_SoundHandle()
{
    class_< snx::SoundHandle, pyj::snx_SoundHandle_Wrapper >("SoundHandle", init<  >())
        .def(init< const snx::SoundHandle& >())
        .def(init< const std::string& >())
        .def("trigger", &snx::SoundHandle::trigger, &pyj::snx_SoundHandle_Wrapper::default_trigger_1)
        .def("trigger", &pyj::snx_SoundHandle_Wrapper::default_trigger_0)
        .def("isPlaying", &snx::SoundHandle::isPlaying, &pyj::snx_SoundHandle_Wrapper::default_isPlaying)
        .def("setRetriggerable", &snx::SoundHandle::setRetriggerable, &pyj::snx_SoundHandle_Wrapper::default_setRetriggerable)
        .def("isRetriggerable", &snx::SoundHandle::isRetriggerable, &pyj::snx_SoundHandle_Wrapper::default_isRetriggerable)
        .def("stop", &snx::SoundHandle::stop, &pyj::snx_SoundHandle_Wrapper::default_stop)
        .def("pause", &snx::SoundHandle::pause, &pyj::snx_SoundHandle_Wrapper::default_pause)
        .def("unpause", &snx::SoundHandle::unpause, &pyj::snx_SoundHandle_Wrapper::default_unpause)
        .def("isPaused", &snx::SoundHandle::isPaused, &pyj::snx_SoundHandle_Wrapper::default_isPaused)
        .def("setAmbient", &snx::SoundHandle::setAmbient, &pyj::snx_SoundHandle_Wrapper::default_setAmbient_1)
        .def("setAmbient", &pyj::snx_SoundHandle_Wrapper::default_setAmbient_0)
        .def("isAmbient", &snx::SoundHandle::isAmbient, &pyj::snx_SoundHandle_Wrapper::default_isAmbient)
        .def("setPitchBend", &snx::SoundHandle::setPitchBend, &pyj::snx_SoundHandle_Wrapper::default_setPitchBend)
        .def("setVolume", &snx::SoundHandle::setVolume, &pyj::snx_SoundHandle_Wrapper::default_setVolume)
        .def("setCutoff", &snx::SoundHandle::setCutoff, &pyj::snx_SoundHandle_Wrapper::default_setCutoff)
        .def("setPosition", &snx::SoundHandle::setPosition, &pyj::snx_SoundHandle_Wrapper::default_setPosition)
        .def("getPosition", &snx::SoundHandle::getPosition, &pyj::snx_SoundHandle_Wrapper::default_getPosition)
        .def("setListenerPosition", &snx::SoundHandle::setListenerPosition, &pyj::snx_SoundHandle_Wrapper::default_setListenerPosition)
        .def("getListenerPosition", &snx::SoundHandle::getListenerPosition, &pyj::snx_SoundHandle_Wrapper::default_getListenerPosition)
        .def("configure", &snx::SoundHandle::configure, &pyj::snx_SoundHandle_Wrapper::default_configure)
        .def("remove", &snx::SoundHandle::remove, &pyj::snx_SoundHandle_Wrapper::default_remove)
        .def("init", &snx::SoundHandle::init)
        .def("getName", &snx::SoundHandle::getName)
    ;

}
