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
#include <snx/SoundInfo.h>

// Using =======================================================================
using namespace boost::python;


// Module ======================================================================
void _Export_SoundInfo()
{
    scope* snx_SoundInfo_scope = new scope(
    class_< snx::SoundInfo >("SoundInfo",
         "Informational type that describes one sound entry.  Typically, user\n"
         "code will fill in the properties of an instance and pass it to a\n"
         "snx.SoundHandle object to configure the sound."
         ,
         init<  >()
        )
        .def(init< const snx::SoundInfo& >())
        .def_readwrite("alias", &snx::SoundInfo::alias)
        .def_readwrite("datasource", &snx::SoundInfo::datasource)
        .def_readwrite("filename", &snx::SoundInfo::filename)
        .def_readwrite("position", &snx::SoundInfo::position)
        .def_readwrite("ambient", &snx::SoundInfo::ambient)
        .def_readwrite("retriggerable", &snx::SoundInfo::retriggerable)
        .def_readwrite("repeat", &snx::SoundInfo::repeat)
        .def_readwrite("pitchbend", &snx::SoundInfo::pitchbend)
        .def_readwrite("cutoff", &snx::SoundInfo::cutoff)
        .def_readwrite("volume", &snx::SoundInfo::volume)
        .def_readwrite("streaming", &snx::SoundInfo::streaming)
    );

    enum_< snx::SoundInfo::DataSource >("DataSource")
        .value("DATA_16_MONO", snx::SoundInfo::DATA_16_MONO)
        .value("DATA_8_MONO", snx::SoundInfo::DATA_8_MONO)
        .value("FILESYSTEM", snx::SoundInfo::FILESYSTEM)
    ;

    delete snx_SoundInfo_scope;

}
