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
#include <snx/SoundAPIInfo.h>

// Using =======================================================================
using namespace boost::python;


// Module ======================================================================
void _Export_SoundAPIInfo()
{
    scope* snx_SoundAPIInfo_scope = new scope(
    class_< snx::SoundAPIInfo >("SoundAPIInfo", init<  >())
        .def(init< const snx::SoundAPIInfo& >())
        .def_readwrite("voices", &snx::SoundAPIInfo::voices)
        .def_readwrite("speakerConfig", &snx::SoundAPIInfo::speakerConfig)
        .def_readwrite("sampleRate", &snx::SoundAPIInfo::sampleRate)
    );

    enum_< snx::SoundAPIInfo::SpeakerConfig >("SpeakerConfig")
        .value("MONO", snx::SoundAPIInfo::MONO)
        .value("STEREO", snx::SoundAPIInfo::STEREO)
        .value("DOLBY5_1", snx::SoundAPIInfo::DOLBY5_1)
        .value("OCT", snx::SoundAPIInfo::OCT)
        .value("QUAD", snx::SoundAPIInfo::QUAD)
    ;


    enum_< snx::SoundAPIInfo::SampleRate >("SampleRate")
        .value("STEREO_22050_KHZ", snx::SoundAPIInfo::STEREO_22050_KHZ)
        .value("MONO_44100_KHZ", snx::SoundAPIInfo::MONO_44100_KHZ)
        .value("STEREO_44100_KHZ", snx::SoundAPIInfo::STEREO_44100_KHZ)
        .value("MONO_22050_KHZ", snx::SoundAPIInfo::MONO_22050_KHZ)
    ;

    delete snx_SoundAPIInfo_scope;

}
