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
#include <vpr/IO/SerializableObject.h>

// Using =======================================================================
using namespace boost::python;


// Module ======================================================================
void _Export_SerializableObject()
{
    class_< vpr::SerializableObject, bases< vpr::WriteableObject, vpr::ReadableObject > , boost::noncopyable >("SerializableObject", no_init)
    ;

}
