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

struct snx_SoundImplementation_Adapter: snx::SoundImplementation
{
    snx_SoundImplementation_Adapter(PyObject* self_, const snx::SoundImplementation& p0):
        snx::SoundImplementation(p0), self(self_) {}

    snx_SoundImplementation_Adapter(PyObject* self_):
        snx::SoundImplementation(), self(self_) {}

   virtual tuple getPositionWrapper(const std::string& alias)
   {
      float p0, p1, p2;
      getPosition(alias, p0, p1, p2);
      return make_tuple(p0, p1, p2);
   }

    PyObject* self;
};

struct snx_SoundImplementation_Wrapper: snx_SoundImplementation_Adapter
{
    snx_SoundImplementation_Wrapper(PyObject* self_, const snx::SoundImplementation& p0):
        snx_SoundImplementation_Adapter(self_, p0) {}

    snx_SoundImplementation_Wrapper(PyObject* self_):
        snx_SoundImplementation_Adapter(self_) {}

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
        tuple result = getPositionWrapper(p0);
        p1 = extract<float>(result[0]);
        p2 = extract<float>(result[1]);
        p3 = extract<float>(result[2]);
    }

    tuple default_getPositionWrapper(const std::string& p0) {
        float p1, p2, p3;
        snx::SoundImplementation::getPosition(p0, p1, p2, p3);
        return make_tuple(p1, p2, p3);
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
};


}// namespace pyj


// Module ======================================================================
void _Export_SoundImplementation()
{
    class_< snx::SoundImplementation, boost::noncopyable, pyj::snx_SoundImplementation_Wrapper >("SoundImplementation",
         init<  >(
            "__init__()\n"
            "Default constructor."
         )
        )
        .def("clone",
             pure_virtual((void (snx::SoundImplementation::*)(snx::ISoundImplementation*&) )&snx::SoundImplementation::clone),
             "clone(newCopy)\n"
             "Every implementation can return a new copy of itself."
         )
        .def("trigger",
             (void (snx::SoundImplementation::*)(const std::string&, const int&) )&snx::SoundImplementation::trigger,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, const int&))&pyj::snx_SoundImplementation_Wrapper::default_trigger_2,
             "trigger(alias, repeat)\n"
             "Triggers a sound.\n\n"
             "Pre-condition:\n"
             "alias does not have to be associated with a loaded sound.\n\n"
             "Post-condition:\n"
             "If allias is associated with a loaded sound, then the loaded\n"
             "sound is triggered.  If not, nothing happens.\n\n"
             "Arguments:\n"
             "alias  -- Alias of the sound to trigger.\n"
             "repeat -- The number of times to play.  Use -1 to repeat\n\n"
             "          forever.\n\n"
             "trigger(alias)\n"
             "Triggers a sound once."
         )
        .def("trigger",
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_trigger_1
         )
        .def("isPlaying",
             (bool (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::isPlaying,
             (bool (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_isPlaying,
             "isPlaying(alias) -> Boolean\n"
             "Is the named sound currently playing?\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound to query."
         )
        .def("setRetriggerable",
             (void (snx::SoundImplementation::*)(const std::string&, bool) )&snx::SoundImplementation::setRetriggerable,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, bool))&pyj::snx_SoundImplementation_Wrapper::default_setRetriggerable,
             "setRetriggerable(alias, onOff)\n"
             "Specifies whether the named sound retriggers from the beginning\n"
             "when triggered while playing.  In other words, when the named\n"
             "sound is already playing and trigger() is called, does the\n"
             "sound restart from the beginning?\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound to change.\n"
             "onOff -- A Boolean value enabling or disabling retriggering."
         )
        .def("isRetriggerable",
             (bool (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::isRetriggerable,
             (bool (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_isRetriggerable,
             "isRetriggerable(alais) -> Boolean\n"
             "Is the named sound retriggerable?\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound to query."
         )
        .def("stop",
             (void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::stop,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_stop,
             "stop(alias)\n"
             "Stops the named sound.\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound to stop."
         )
        .def("pause",
             (void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::pause,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_pause,
             "pause(alias)\n"
             "Pauses the sound.  Use unpause() to return playback from where\n"
             "the sound was paused.\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound to pause."
         )
        .def("unpause",
             (void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::unpause,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_unpause,
             "unpause(alias)\n"
             "Resumes playback of the named sound from a paused state.  This\n"
             "does nothing if the sound was not paused.\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound to unpause."
         )
        .def("isPaused",
             (bool (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::isPaused,
             (bool (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_isPaused,
             "isPaused(alias) -> Boolean\n"
             "If the sound is paused, then return True.\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound to query."
         )
        .def("setAmbient",
             (void (snx::SoundImplementation::*)(const std::string&, bool) )&snx::SoundImplementation::setAmbient,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, bool))&pyj::snx_SoundImplementation_Wrapper::default_setAmbient_2,
             "setAmbient(alias, ambient)\n"
             "Sets the named sound as either ambient or positional depending\n"
             "on the value of the given argument.  If the sound is ambient,\n"
             "it is attached to the listener, and its volume does not change\n"
             "when the listener moves.  If the sound is positional, the\n"
             "volume changes when the listener moves.\n\n"
             "Arguments:\n"
             "alias   -- The alias of the sound to change\n"
             "ambient -- A Boolean flag identifying whether this sound is\n"
             "           ambient (True) or positional (False).\n\n"
             "setAmbient(alias)\n"
             "Sets the sound as being positional.\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound to change\n"
         )
        .def("setAmbient",
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_setAmbient_1
         )
        .def("isAmbient",
             (bool (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::isAmbient,
             (bool (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_isAmbient,
             "isAmbient(alias) -> Boolean\n"
             "Is the named sound ambient?\n\n"
             "Arguments:\n"
             "alias -- The name of the sound to query."
         )
        .def("setPitchBend",
             (void (snx::SoundImplementation::*)(const std::string&, float) )&snx::SoundImplementation::setPitchBend,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, float))&pyj::snx_SoundImplementation_Wrapper::default_setPitchBend,
             "setPitchBend(alias, amount)\n"
             "Alters the frequency of the named sound.\n\n"
             "Arguments:\n"
             "alias  -- The alias of the sound to change.\n"
             "amount -- A floating-point value that determines the pitch\n"
             "          bend.  1.0 means that there is no change.  A value\n"
             "          less than 1.0 is low; a value greather than 1.0 is\n"
             "          high."
         )
        .def("setVolume",
             (void (snx::SoundImplementation::*)(const std::string&, float) )&snx::SoundImplementation::setVolume,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, float))&pyj::snx_SoundImplementation_Wrapper::default_setVolume,
             "setVolume(alias, amount)\n"
             "Sets the effect volume of the named sound.  The value must be\n"
             "in the range [0,1].\n\n"
             "Argument:\n"
             "alias  -- The alias of the sound to change.\n"
             "amount -- A floating-point value that determines the volume.\n"
             "          It must be between 0.0 and 1.0 inclusive."
         )
        .def("setCutoff",
             (void (snx::SoundImplementation::*)(const std::string&, float) )&snx::SoundImplementation::setCutoff,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, float))&pyj::snx_SoundImplementation_Wrapper::default_setCutoff,
             "setCutoff(alias, amount)\n"
             "Sets the effect cutoff of the named sound.  Set to a value in\n"
             "the range [0,1].\n\n"
             "Arguments:\n"
             "alias  -- The alias of the sound to change.\n"
             "amount -- A floating-point value that determines the cutoff.\n"
             "          The value must be between 0.0 and 1.0 inclusive.\n"
             "          1.0 means no change; 0.0 is total cutoff."
         )
        .def("setPosition",
             (void (snx::SoundImplementation::*)(const std::string&, float, float, float) )&snx::SoundImplementation::setPosition,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, float, float, float))&pyj::snx_SoundImplementation_Wrapper::default_setPosition,
             "setPosition(alias, x, y, z)\n"
             "Sets the named sound's three-dimensional position.\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound to change.\n"
             "x     -- The X coordinate of the sound in 3D OpenGL coordinates.\n"
             "y     -- The Y coordinate of the sound in 3D OpenGL coordinates.\n"
             "z     -- The Z coordinate of the sound in 3D OpenGL coordinates."
         )
        .def("getPosition",
             &pyj::snx_SoundImplementation_Adapter::getPositionWrapper,
             (tuple (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_getPositionWrapper,
             "getPosition(alias) -> tuple\n"
             "Gets the named sound's 3D position.\n\n"
             "Arguments:\n"
             "alias  -- The alias of the sound to change.\n\n"
             "Returns:\n"
             "A tuple of floating-point values providing the (x, y, z)\n"
             "position of the named sound."
         )
        .def("setListenerPosition",
             (void (snx::SoundImplementation::*)(const gmtl::Matrix44f&) )&snx::SoundImplementation::setListenerPosition,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const gmtl::Matrix44f&))&pyj::snx_SoundImplementation_Wrapper::default_setListenerPosition,
             "setListenerPosition(matrix)\n"
             "Sets the position of the listener.\n\n"
             "Arguments:\n"
             "matrix -- A gmtl.Matrix44f object representing the position\n"
             "          of the listener."
         )
        .def("getListenerPosition",
             (void (snx::SoundImplementation::*)(gmtl::Matrix44f&) )&snx::SoundImplementation::getListenerPosition,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(gmtl::Matrix44f&))&pyj::snx_SoundImplementation_Wrapper::default_getListenerPosition,
             "getListenerPosition(matrix)\n"
             "Gets the listeners's 3D position.\n\n"
             "Arguments:\n"
             "matrix -- A gmtl.Matrix44f object used to store the position\n"
             "          of the listener."
         )
        .def("startAPI",
             pure_virtual((int (snx::SoundImplementation::*)() )&snx::SoundImplementation::startAPI),
             "startAPI() -> integer\n"
             "Starts the sound API, creating any contexts or other\n"
             "configurations at startup.  This function should be callled\n"
             "before using the other functions in the class.\n\n"
             "Post-condition:\n"
             "Sound APi is ready to go.\n\n"
             "Returns:\n"
             "1 if success, 0 otherwise."
         )
        .def("isStarted",
             pure_virtual((bool (snx::SoundImplementation::*)() const)&snx::SoundImplementation::isStarted),
             "isStarted() -> Boolean\n"
             "Queries whether the API has been started.\n\n"
             "Returns:\n"
             "True if the API has been started, False otherwise."
         )
        .def("shutdownAPI",
             (void (snx::SoundImplementation::*)() )&snx::SoundImplementation::shutdownAPI,
             (void (pyj::snx_SoundImplementation_Wrapper::*)())&pyj::snx_SoundImplementation_Wrapper::default_shutdownAPI,
             "shutdownAPI()\n"
             "Kills the sound API, deallocating any sounds, etc.  This\n"
             "function could be called any time.  The function could be\n"
             "called multiple times, so it should be smart.\n\n"
             "Post-condition:\n"
             "The sound API is no longer running or available for use."
         )
        .def("configure",
             (void (snx::SoundImplementation::*)(const snx::SoundAPIInfo&) )&snx::SoundImplementation::configure,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const snx::SoundAPIInfo&))&pyj::snx_SoundImplementation_Wrapper::default_configure,
             "configure(description)\n"
             "Configures/reconfigures the sound API global settings.\n\n"
             "Arguments:\n"
             "description -- A snx.SoundAPIInfo object that describes the\n"
             "               settings for this sound API."
         )
        .def("configure",
             (void (snx::SoundImplementation::*)(const std::string&, const snx::SoundInfo&) )&snx::SoundImplementation::configure,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&, const snx::SoundInfo&))&pyj::snx_SoundImplementation_Wrapper::default_configure,
             "configure(alais, description)\n"
             "Configures/reconfigures the named sound by associating sound\n"
             "data with the named sound.  Afterwards, the alias can be used\n"
             "to operate on sound data.\n\n"
             "Configure: associate a name (alias) with the description if\n"
             "not already done.\n"
             "Reconfigure: change properties of the sound to the description\n"
             "provided.\n\n"
             "Pre-condition:\n"
             "Provide a snx.SoundInfo that describes the sound.\n\n"
             "Post-condition:\n"
             "This handle will point to loaded sound data.\n\n"
             "Arguments:\n"
             "alias       -- The alias of the sound to be associated with the\n"
             "               given sound data.\n"
             "description -- A snx.SoundInfo object that describes the sound\n"
             "               for which this object will be a handle."
         )
        .def("remove",
             (void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::remove,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_remove,
             "remove(alias)\n"
             "Removes a configured sound.  Any future reference to the alias\n"
             "will not cause an error, but it will not result in a rendered\n"
             "sound.\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound to remove."
         )
        .def("step",
             (void (snx::SoundImplementation::*)(const float&) )&snx::SoundImplementation::step,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const float&))&pyj::snx_SoundImplementation_Wrapper::default_step,
             "step(timeElapsed)\n"
             "Call once per sound frame (which does not have to be the same\n"
             "as the graphics frame).\n\n"
             "Arguments:\n"
             "timeElapsed -- A floating-point value giving the time elapsed\n"
             "               since the last sound frame."
         )
        .def("clear",
             (void (snx::SoundImplementation::*)() )&snx::SoundImplementation::clear,
             (void (pyj::snx_SoundImplementation_Wrapper::*)())&pyj::snx_SoundImplementation_Wrapper::default_clear,
             "clear()\n"
             "Clears all associations.\n\n"
             "Post-conditions:\n"
             "Any existing aliases will be stubbed.  Sounds will be unbound."
         )
        .def("bindAll",
             (void (snx::SoundImplementation::*)() )&snx::SoundImplementation::bindAll,
             (void (pyj::snx_SoundImplementation_Wrapper::*)())&pyj::snx_SoundImplementation_Wrapper::default_bindAll,
             "bindAll()\n"
             "Bind: load (or reload) all associated sounds.\n\n"
             "Post-condition:\n"
             "All sound associations are buffered by the sound API."
         )
        .def("unbindAll",
             (void (snx::SoundImplementation::*)() )&snx::SoundImplementation::unbindAll,
             (void (pyj::snx_SoundImplementation_Wrapper::*)())&pyj::snx_SoundImplementation_Wrapper::default_unbindAll,
             "unbindAll()\n"
             "Unbind: unload/deallocate all associated sounds.\n\n"
             "Post-conditions:\n"
             "All sound associations are unbuffered by the sound API."
         )
        .def("bind",
             pure_virtual((void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::bind),
             "bind(alias)\n"
             "Loads/allocates the sound data this alias refers to in the\n"
             "sound API.\n\n"
             "Post-condition:\n"
             "The sound API has the sound buffered.\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound data to be bound."
         )
        .def("unbind",
             pure_virtual((void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::unbind),
             "unbind(alias)\n"
             "Unloads/deallocates the sound data this alias refers to in the\n"
             "sound API.\n\n"
             "Post-condition:\n"
             "The sound API no longer has the sound buffered.\n\n"
             "Arguments:\n"
             "alias -- The alias of the sound data to be unbound."
         )
        .def("lookup",
             (snx::SoundInfo& (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::lookup,
             (snx::SoundInfo& (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_lookup, return_internal_reference< 1 >()
         )
        .def("setName",
             (void (snx::SoundImplementation::*)(const std::string&) )&snx::SoundImplementation::setName,
             (void (pyj::snx_SoundImplementation_Wrapper::*)(const std::string&))&pyj::snx_SoundImplementation_Wrapper::default_setName
         )
        .def("name",
             (std::string& (snx::SoundImplementation::*)() )&snx::SoundImplementation::name,
             (std::string& (pyj::snx_SoundImplementation_Wrapper::*)())&pyj::snx_SoundImplementation_Wrapper::default_name, return_internal_reference< 1 >()
         )
        .def("copy", &snx::SoundImplementation::copy,
             "copy(si)\n"
             "Copies the current state of the system from the given API into\n"
             "this object.  This does not perform any binding.  That must be\n"
             "done separately.\n\n"
             "Arguments:\n"
             "si -- A snx.SoundImplementation object from which the state\n"
             "      will be copied into this object."
         )
    ;

}
