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
#include <vpr/IO/SerializableObject.h>
#include <vpr/IO/ObjectWriter.h>
#include <pyjutil/InterpreterGuard.h>
#include <pyjutil/Debug.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vpr_WriteableObject_Wrapper: vpr::WriteableObject
{
    vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_WriteableObject_Wrapper::writeObject()\n",
                                 "vpr_WriteableObject_Wrapper::writeObject() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    PyObject* self;
};


}// namespace pyj


// Module ======================================================================
void _Export_WriteableObject()
{
    class_< vpr::WriteableObject, boost::noncopyable, pyj::vpr_WriteableObject_Wrapper >("WriteableObject", no_init)
        .def("writeObject", pure_virtual(&vpr::WriteableObject::writeObject))
    ;

}
