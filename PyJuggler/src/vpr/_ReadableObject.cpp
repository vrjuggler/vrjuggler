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
#include <vpr/IO/ObjectReader.h>
#include <pyjutil/InterpreterGuard.h>
#include <pyjutil/Debug.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vpr_ReadableObject_Wrapper: vpr::ReadableObject
{
    vpr::ReturnStatus readObject(vpr::ObjectReader* p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ReadableObject_Wrapper::readObject()\n",
                                 "vpr_ReadableObject_Wrapper::readObject() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "readObject", p0);
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
void _Export_ReadableObject()
{
    class_< vpr::ReadableObject, boost::noncopyable, pyj::vpr_ReadableObject_Wrapper >("ReadableObject", no_init)
        .def("readObject", pure_virtual(&vpr::ReadableObject::readObject),
             "readObject(reader) -> PyJuggler.vpr.ReturnStatus object\n"
             "Template method for reading into this object from the given\n"
             "stream.\n"
             "<b>Post condition:</b> All object data is read from the reader.")
    ;

}
