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
#include <vpr/Util/GUID.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vpr_GUID_Wrapper: vpr::GUID
{
    vpr_GUID_Wrapper(PyObject* self_, const vpr::GUID::GenerateTag& p0):
        vpr::GUID(p0), self(self_) {}

    vpr_GUID_Wrapper(PyObject* self_):
        vpr::GUID(), self(self_) {}

    vpr_GUID_Wrapper(PyObject* self_, const std::string& p0):
        vpr::GUID(p0), self(self_) {}

    vpr_GUID_Wrapper(PyObject* self_, const vpr::GUID& p0, const std::string& p1):
        vpr::GUID(p0, p1), self(self_) {}

    vpr_GUID_Wrapper(PyObject* self_, const vpr::GUID& p0):
        vpr::GUID(p0), self(self_) {}


    PyObject* self;
};

struct GuidPickle : pickle_suite
{
   static tuple getstate(const vpr::GUID& g)
   {
      return make_tuple(g.mGuid.packed.l0, g.mGuid.packed.l1,
                        g.mGuid.packed.l2, g.mGuid.packed.l3);
   }

   static void setstate(vpr::GUID& g, tuple state)
   {
      g.mGuid.packed.l0 = extract<vpr::Uint32>(state[0]);
      g.mGuid.packed.l1 = extract<vpr::Uint32>(state[1]);
      g.mGuid.packed.l2 = extract<vpr::Uint32>(state[2]);
      g.mGuid.packed.l3 = extract<vpr::Uint32>(state[3]);
   }
};

}// namespace pyj


// Module ======================================================================
void _Export_GUID()
{
    scope* vpr_GUID_scope = new scope(
    class_< vpr::GUID, bases< vpr::SerializableObject > , pyj::vpr_GUID_Wrapper >("GUID",
         init<  >(
            "__init__()\n"
            "Creates an empty GUID equal to PyJuggler.vpr.GUID.NullGUID.\n\n"
            "__init__(GenerateTag)\n"
            "Creates a new, non-empty GUID.\n\n"
            "__init__(guid_string)\n"
            "Generates a GUID from the given string representation of the GUID using\n"
            "a string.\n"
            "Format: \"8x-4x-4x-2x2x-2x2x2x2x2x2x\"\n\n"
            "Keyword arguments:\n"
            "guid_string -- A string that is used to initialize a GUID.\n\n"
            "__init__(GUID, 'name')\n"
            "Generates a GUID based on the given name that is part of the namespace\n"
            "identified by the given namespace GUID."
         ))
        .def(init< const vpr::GUID::GenerateTag& >())
        .def(init< const std::string& >())
        .def(init< const vpr::GUID&, const std::string& >())
        .def(init< const vpr::GUID& >())
        .def_readwrite("generateTag", &vpr::GUID::generateTag)
        .def_readonly("NullGUID", &vpr::GUID::NullGUID)
        .def("toString", &vpr::GUID::toString,
            "Converts this GUID to its corresponding string representation.")
        .def("generate", (void (vpr::GUID::*)() )&vpr::GUID::generate)
        .def("generate", (void (vpr::GUID::*)(const vpr::GUID &, const std::string&) )&vpr::GUID::generate)
        .def_pickle(pyj::GuidPickle())
        .def(self_ns::str(self))
        .def( self == self )
        .def( self != self )
        .def( self < self )
    );

    class_< vpr::GUID::GenerateTag >("GenerateTag",
                                     "Tag to the constructor to force generation.",
                                     init<  >())
        .def(init< const vpr::GUID::GenerateTag& >())
    ;

    class_< vpr::GUID::hash >("hash",
                             "Hasher for vpr.GUID.  This can be used with std::hash_map and\n"
                             "friends.",
                             init<  >())
        .def(init< const vpr::GUID::hash& >())
        .def("__call__", &vpr::GUID::hash::operator ())
    ;

    delete vpr_GUID_scope;

}
