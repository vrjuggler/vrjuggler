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
#include <vpr/IO/ObjectWriter.h>
#include <pyjutil/InterpreterGuard.h>
#include <pyjutil/Debug.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vpr_ObjectWriter_Wrapper: vpr::ObjectWriter
{
    vpr::ReturnStatus beginTag(std::string p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::beginTag()\n",
                                 "vpr_ObjectWriter_Wrapper::beginTag() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "beginTag", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::ReturnStatus endTag() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::endTag()\n",
                                 "vpr_ObjectWriter_Wrapper::endTag() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "endTag");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::ReturnStatus beginAttribute(std::string p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::beginAttribute()\n",
                                 "vpr_ObjectWriter_Wrapper::beginAttribute() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "beginAttribute", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::ReturnStatus endAttribute() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::endAttribute()\n",
                                 "vpr_ObjectWriter_Wrapper::endAttribute() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "endAttribute");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::ReturnStatus writeUint8(vpr::Uint8 p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::writeUint8()\n",
                                 "vpr_ObjectWriter_Wrapper::writeUint8() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeUint8", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::ReturnStatus writeUint16(vpr::Uint16 p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::writeUint16()\n",
                                 "vpr_ObjectWriter_Wrapper::writeUint16() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeUint16", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::ReturnStatus writeUint32(vpr::Uint32 p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::writeUint32()\n",
                                 "vpr_ObjectWriter_Wrapper::writeUint32() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeUint32", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::ReturnStatus writeUint64(vpr::Uint64 p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::writeUint64()\n",
                                 "vpr_ObjectWriter_Wrapper::writeUint64() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeUint64", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::ReturnStatus writeFloat(float p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::writeFloat()\n",
                                 "vpr_ObjectWriter_Wrapper::writeFloat() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeFloat", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::ReturnStatus writeDouble(double p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::writeDouble()\n",
                                 "vpr_ObjectWriter_Wrapper::writeDouble() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeDouble", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::ReturnStatus writeString(std::string p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::writeString()\n",
                                 "vpr_ObjectWriter_Wrapper::writeString() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeString", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::ReturnStatus writeBool(bool p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::writeBool()\n",
                                 "vpr_ObjectWriter_Wrapper::writeBool() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeBool", p0);
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
void _Export_ObjectWriter()
{
    class_< vpr::ObjectWriter, boost::noncopyable, pyj::vpr_ObjectWriter_Wrapper >("ObjectWriter",
         "Interface used to write object data to a stream.\n\n"
         "ObjectReader and ObjectWriter support an interface that allows for\n"
         "using tags and attributes in the written output data.  This allows\n"
         "support for formats such as XML where there is a logical grouping\n"
         "of the data.\n\n"
         "The structure looks something like the following (based on XML):\n\n"
         "<tag1>\n"
         "  <tag2 attrib1=\"XXX\">\n"
         "  </tag2>\n"
         "</tag1>",
         no_init)
        .def("beginTag", pure_virtual(&vpr::ObjectWriter::beginTag),
             "beginTag(tagName) -> PyJuggler.vpr.ReturnStatus object\n"
             "Starts a new section/element of name tagName.")
        .def("endTag", pure_virtual(&vpr::ObjectWriter::endTag),
             "endTag() -> PyJuggler.vpr.ReturnStatus object\n"
             "Ends the most recently named tag.")
        .def("beginAttribute", pure_virtual(&vpr::ObjectWriter::beginAttribute),
             "beginAttribute(attributeName) -> PyJuggler.vpr.ReturnStatus object\n"
             "Starts an attribute of the name attributeName.")
        .def("endAttribute", pure_virtual(&vpr::ObjectWriter::endAttribute),
             "endAttribute() -> PyJuggler.vpr.ReturnStatus object\n"
             "Ends the most recently named attribute.")
        .def("writeUint8", pure_virtual(&vpr::ObjectWriter::writeUint8))
        .def("writeUint16", pure_virtual(&vpr::ObjectWriter::writeUint16))
        .def("writeUint32", pure_virtual(&vpr::ObjectWriter::writeUint32))
        .def("writeUint64", pure_virtual(&vpr::ObjectWriter::writeUint64))
        .def("writeFloat", pure_virtual(&vpr::ObjectWriter::writeFloat))
        .def("writeDouble", pure_virtual(&vpr::ObjectWriter::writeDouble))
        .def("writeString", pure_virtual(&vpr::ObjectWriter::writeString))
        .def("writeBool", pure_virtual(&vpr::ObjectWriter::writeBool))
        .def("isBinary", &vpr::ObjectWriter::isBinary,
             "Returns true if the writer is using a binary-based format.\n"
             "This can be used to choose whether to use human-readable forms\n"
             "of serialization.")
        .def("attribExists", &vpr::AttributeMapBase::attribExists,
             "attribExists(name) -> bool\n"
             "Determines if the named attribute exists.")
    ;

}
