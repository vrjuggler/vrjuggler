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
#include <snx/SoundFactory.h>

// Using =======================================================================
using namespace boost::python;


// Module ======================================================================
void _Export_SoundFactory()
{
    class_< snx::SoundFactory, boost::noncopyable >("SoundFactory", no_init)
        .def_readwrite("mRegisteredImplementations", &snx::SoundFactory::mRegisteredImplementations)
        .def_readwrite("mPlugins", &snx::SoundFactory::mPlugins)
        .def("errorOutput", &snx::SoundFactory::errorOutput)
        .def("isPlugin", &snx::SoundFactory::isPlugin)
        .def("loadPlugins", &snx::SoundFactory::loadPlugins)
        .def("unloadPlugins", &snx::SoundFactory::unloadPlugins)
        .def("createImplementation", &snx::SoundFactory::createImplementation)
        .def("reg", &snx::SoundFactory::reg)
        .def("instance", &snx::SoundFactory::instance, return_value_policy< reference_existing_object >())
        .staticmethod("instance")
    ;

}
