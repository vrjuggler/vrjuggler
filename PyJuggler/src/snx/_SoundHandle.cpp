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
#include <snx/SoundHandle.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct snx_SoundHandle_Adapter : snx::SoundHandle
{
    snx_SoundHandle_Adapter(PyObject* self_, const snx::SoundHandle& p0):
        snx::SoundHandle(p0), self(self_) {}

    snx_SoundHandle_Adapter(PyObject* self_):
        snx::SoundHandle(), self(self_) {}

    snx_SoundHandle_Adapter(PyObject* self_, const std::string& p0):
        snx::SoundHandle(p0), self(self_) {}

    virtual tuple getPositionWrapper()
    {
        return call_method<tuple>(self, "getPosition");
    }

    PyObject* self;
};

struct snx_SoundHandle_Wrapper: snx_SoundHandle_Adapter
{
    snx_SoundHandle_Wrapper(PyObject* self_, const snx::SoundHandle& p0):
        snx_SoundHandle_Adapter(self_, p0) {}

    snx_SoundHandle_Wrapper(PyObject* self_):
        snx_SoundHandle_Adapter(self_) {}

    snx_SoundHandle_Wrapper(PyObject* self_, const std::string& p0):
        snx_SoundHandle_Adapter(self_, p0) {}

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

    void setAmbient(const bool p0) {
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
        tuple result = getPositionWrapper();
        p0 = extract<float>(result[0]);
        p1 = extract<float>(result[1]);
        p2 = extract<float>(result[2]);
    }

    tuple default_getPositionWrapper() {
        float p0, p1, p2;
        snx::SoundHandle::getPosition(p0, p1, p2);
        return make_tuple(p0, p1, p2);
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
};


}// namespace pyj


// Module ======================================================================
void _Export_SoundHandle()
{
    class_< snx::SoundHandle, pyj::snx_SoundHandle_Wrapper >("SoundHandle",
         "A handle to a Sonix sound.  This is a convenient handle to a\n"
         "sound, and it is the interface to using individual sounds in\n"
         "Sonix.  Use configure() and remove() to manage the memory of a\n"
         "sound handle.  Memory leaks could occur if remove() is not called.\n"
         "This is because there may be more than one handle to a sound.\n\n"
         "Note:\n"
         "You may have to call snx.sonix.instance().changeAPI() to switch to\n"
         "the sound API you want to use (such as OpenAL or AudioWorks)."
         ,
         init<  >(
            "__init__()\n"
            "Default constructor.  Call init() after constructing to name\n"
            "this handle.\n\n"
            "Post-conditions:\n"
            "This sound handle is not associated with any nae.  init() must\n"
            "be called to assign a name to this handle.\n\n"
            "__init__(name)\n"
            "Name constructor.  This assigns the given name to this sound\n"
            "handle.  This name is what is used in all snx.sonix class\n"
            "calls.  (snx.SoundHandle is a wrapper for snx.sonix.)\n\n"
            "Post-conditions:\n"
            "This sound handle is associated with the given name, and there\n"
            "is no need to call init()."
         )
        )
        .def(init< const snx::SoundHandle& >())
        .def(init< const std::string& >())
        .def("trigger", &snx::SoundHandle::trigger,
             &pyj::snx_SoundHandle_Wrapper::default_trigger_1,
             "trigger(repeat)\n"
             "Triggers a sound.\n\n"
             "Pre-condition:\n"
             "This object does not have to be a sound.\n\n"
             "Post-condition:\n"
             "If this object is a sound, then the loaded sound is triggered.\n"
             "If this object is not a sound, then nothing happens.\n\n"
             "Arguments:\n"
             "repeat -- Number of times to play.  Use -1 to repeat forever.\n\n"
             "trigger()\n"
             "Triggers a sound once."
         )
        .def("trigger", &pyj::snx_SoundHandle_Wrapper::default_trigger_0)
        .def("isPlaying", &snx::SoundHandle::isPlaying,
             &pyj::snx_SoundHandle_Wrapper::default_isPlaying,
             "isPlaying() -> Boolean\n"
             "Is the sound currently playing?"
         )
        .def("setRetriggerable", &snx::SoundHandle::setRetriggerable,
             &pyj::snx_SoundHandle_Wrapper::default_setRetriggerable,
             "setRetriggerable(onOff)\n"
             "Specifies whether the sound retriggers from the beginning when\n"
             "triggered while playing.  In other words, when the sound is\n"
             "already playing and trigger() is called, does the sound\n"
             "restart from the beginning?\n\n"
             "Arguments:\n"
             "onOff -- A Boolean value enabling or disabling retriggering."
         )
        .def("isRetriggerable", &snx::SoundHandle::isRetriggerable,
             &pyj::snx_SoundHandle_Wrapper::default_isRetriggerable,
             "isRetriggerable() -> Boolean\n"
             "Is the sound retriggerable?"
         )
        .def("stop", &snx::SoundHandle::stop,
             &pyj::snx_SoundHandle_Wrapper::default_stop,
             "stop()\n"
             "Stops the sound."
         )
        .def("pause", &snx::SoundHandle::pause,
             &pyj::snx_SoundHandle_Wrapper::default_pause,
             "pause()\n"
             "Pauses the sound.  Use unpause() to return playback from where\n"
             "the sound was paused."
         )
        .def("unpause", &snx::SoundHandle::unpause,
             &pyj::snx_SoundHandle_Wrapper::default_unpause,
             "unpause()\n"
             "Resumes playback from a paused state.  This does nothing if the\n"
             "sound was not paused."
         )
        .def("isPaused", &snx::SoundHandle::isPaused,
             &pyj::snx_SoundHandle_Wrapper::default_isPaused,
             "isPaused() -> Boolean\n"
             "If the sound is paused, then return True."
         )
        .def("setAmbient", &snx::SoundHandle::setAmbient,
             &pyj::snx_SoundHandle_Wrapper::default_setAmbient_1,
             "setAmbient(ambient)\n"
             "Sets the sound as either ambient or positional depending on the\n"
             "value of the given argument.  If the sound is ambient, it is\n"
             "attached to the listener, and its volume does not change when\n"
             "the listener moves.  If the sound is positional, the volume\n"
             "changes when the listener moves.\n\n"
             "Arguments:\n"
             "ambient -- A Boolean flag identifying whether this sound is\n"
             "           ambient (True) or positional (False).\n\n"
             "setAmbient()\n"
             "Sets the sound as being positional."
         )
        .def("setAmbient", &pyj::snx_SoundHandle_Wrapper::default_setAmbient_0)
        .def("isAmbient", &snx::SoundHandle::isAmbient,
             &pyj::snx_SoundHandle_Wrapper::default_isAmbient,
             "isAmbient() -> Boolean\n"
             "Is the sound ambient?"
         )
        .def("setPitchBend", &snx::SoundHandle::setPitchBend,
             &pyj::snx_SoundHandle_Wrapper::default_setPitchBend,
             "setPitchBend(amount)\n"
             "Alters the frequency of the sample.\n\n"
             "Arguments:\n"
             "amount -- A floating-point value that determines the pitch\n"
             "          bend.  1.0 means that there is no change.  A value\n"
             "          less than 1.0 is low; a value greather than 1.0 is\n"
             "          high."
         )
        .def("setVolume", &snx::SoundHandle::setVolume,
             &pyj::snx_SoundHandle_Wrapper::default_setVolume,
             "setVolume(amount)\n"
             "Sets the effect volume.  The value must be in the range [0,1].\n\n"
             "Argument:\n"
             "amount -- A floating-point value that determines the volume.\n"
             "          It must be between 0.0 and 1.0 inclusive."
         )
        .def("setCutoff", &snx::SoundHandle::setCutoff,
             &pyj::snx_SoundHandle_Wrapper::default_setCutoff,
             "setCutoff(amount)\n"
             "Sets the effect cutoff.  Set to a value in the range [0,1].\n\n"
             "Arguments:\n"
             "amount -- A floating-point value that determines the cutoff.\n"
             "          The value must be between 0.0 and 1.0 inclusive.\n"
             "          1.0 means no change; 0.0 is total cutoff."
         )
        .def("setPosition", &snx::SoundHandle::setPosition,
             &pyj::snx_SoundHandle_Wrapper::default_setPosition,
             "setPosition(x, y, z)\n"
             "Sets the sound's three-dimensional position.\n\n"
             "Arguments:\n"
             "x -- The X coordinate of the sound in 3D OpenGL coordinates.\n"
             "y -- The Y coordinate of the sound in 3D OpenGL coordinates.\n"
             "z -- The Z coordinate of the sound in 3D OpenGL coordinates."
         )
        .def("getPosition", &pyj::snx_SoundHandle_Adapter::getPositionWrapper,
             &pyj::snx_SoundHandle_Wrapper::default_getPositionWrapper,
             "getPosition() -> tuple\n"
             "Gets the sound's 3D position.\n\n"
             "Returns:\n"
             "A tuple of floating-point values providing the (x, y, z)\n"
             "position of the sound."
         )
        .def("setListenerPosition", &snx::SoundHandle::setListenerPosition,
             &pyj::snx_SoundHandle_Wrapper::default_setListenerPosition,
             "setListenerPosition(matrix)\n"
             "Sets the position of the listener.\n\n"
             "Arguments:\n"
             "matrix -- A gmtl.Matrix44f object representing the position\n"
             "          of the listener."
         )
        .def("getListenerPosition", &snx::SoundHandle::getListenerPosition,
             &pyj::snx_SoundHandle_Wrapper::default_getListenerPosition,
             "getListenerPosition(matrix)\n"
             "Gets the listeners's 3D position.\n\n"
             "Arguments:\n"
             "matrix -- A gmtl.Matrix44f object used to store the position\n"
             "          of the listener."
         )
        .def("configure", &snx::SoundHandle::configure,
             &pyj::snx_SoundHandle_Wrapper::default_configure,
             "configure(description)\n"
             "Configures/reconfigures the sound by associating sound data\n"
             "with this handle.  Afterwards, this sound handle can be used\n"
             "to operate on this sound data.\n\n"
             "Configure: associate a name (alias) with the description if\n"
             "not already done.\n"
             "Reconfigure: change properties of the sound to the description\n"
             "provided.\n\n"
             "Pre-condition:\n"
             "Provide a snx.SoundInfo that describes the sound.\n\n"
             "Post-condition:\n"
             "This handle will point to loaded sound data.\n\n"
             "Arguments:\n"
             "description -- A snx.SoundInfo object that describes the sound\n"
             "               for which this object will be a handle."
         )
        .def("remove", &snx::SoundHandle::remove,
             &pyj::snx_SoundHandle_Wrapper::default_remove,
             "remove()\n"
             "Removes a configured sound.  Any future reference to the alias\n"
             "will not cause an error, but it will not result in a rendered\n"
             "sound."
         )
        .def("init", &snx::SoundHandle::init,
             "init(uniqueName)\n"
             "Initializes the name of this sound with the given name.  If\n"
             "this name has not been configured before, then configure() must\n"
             "be invoked on this handle after calling init()."
         )
        .def("getName", &snx::SoundHandle::getName,
            "getName() -> string object.\n"
            "Gets the name of this handle."
         )
    ;

}
