/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <vpr/IO/ObjectReader.h>
#include <pyjutil/InterpreterGuard.h>
#include <pyjutil/Debug.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vpr_ObjectReader_Wrapper: vpr::ObjectReader
{
    vpr::ReturnStatus beginTag(std::string p0) {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectReader_Wrapper::beginTag()\n",
                                 "vpr_ObjectReader_Wrapper::beginTag() done.\n");
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
                                 "vpr_ObjectReader_Wrapper::endTag()\n",
                                 "vpr_ObjectReader_Wrapper::endTag() done.\n");
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
                                 "vpr_ObjectReader_Wrapper::beginAttribute()\n",
                                 "vpr_ObjectReader_Wrapper::beginAttribute() done.\n");
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
                                 "vpr_ObjectReader_Wrapper::endAttribute()\n",
                                 "vpr_ObjectReader_Wrapper::endAttribute() done.\n");
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

    void resetReading() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectReader_Wrapper::resetReading()\n",
                                 "vpr_ObjectReader_Wrapper::resetReading() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "resetReading");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void pushState() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectReader_Wrapper::pushState()\n",
                                 "vpr_ObjectReader_Wrapper::pushState() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "pushState");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void popState() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectReader_Wrapper::popState()\n",
                                 "vpr_ObjectReader_Wrapper::popState() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            call_method< void >(self, "popState");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::Uint8 readUint8() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectReader_Wrapper::readUint8()\n",
                                 "vpr_ObjectReader_Wrapper::readUint8() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::Uint8 >(self, "readUint8");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::Uint16 readUint16() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectReader_Wrapper::readUint16()\n",
                                 "vpr_ObjectReader_Wrapper::readUint16() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::Uint16 >(self, "readUint16");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::Uint32 readUint32() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectReader_Wrapper::readUint32()\n",
                                 "vpr_ObjectReader_Wrapper::readUint32() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::Uint32 >(self, "readUint32");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    vpr::Uint64 readUint64() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectReader_Wrapper::readUint64()\n",
                                 "vpr_ObjectReader_Wrapper::readUint64() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< vpr::Uint64 >(self, "readUint64");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    float readFloat() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::readFloat()\n",
                                 "vpr_ObjectReader_Wrapper::readFloat() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< float >(self, "readFloat");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    double readDouble() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::readDouble()\n",
                                 "vpr_ObjectReader_Wrapper::readDouble() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< double >(self, "readDouble");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    std::string readString() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::readString()\n",
                                 "vpr_ObjectReader_Wrapper::readString() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< std::string >(self, "readString");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    bool readBool() {
        vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                                 "vpr_ObjectWriter_Wrapper::readBool()\n",
                                 "vpr_ObjectReader_Wrapper::readBool() done.\n");
        PyJuggler::InterpreterGuard guard;

        try
        {
            return call_method< bool >(self, "readBool");
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
void _Export_ObjectReader()
{
    class_< vpr::ObjectReader, boost::noncopyable, pyj::vpr_ObjectReader_Wrapper >("ObjectReader", no_init)
        .def("beginTag", pure_virtual(&vpr::ObjectReader::beginTag))
        .def("endTag", pure_virtual(&vpr::ObjectReader::endTag))
        .def("beginAttribute", pure_virtual(&vpr::ObjectReader::beginAttribute))
        .def("endAttribute", pure_virtual(&vpr::ObjectReader::endAttribute))
        .def("resetReading", pure_virtual(&vpr::ObjectReader::resetReading))
        .def("pushState", pure_virtual(&vpr::ObjectReader::pushState))
        .def("popState", pure_virtual(&vpr::ObjectReader::popState))
        .def("readUint8", pure_virtual((vpr::Uint8 (vpr::ObjectReader::*)() )&vpr::ObjectReader::readUint8))
        .def("readUint16", pure_virtual((vpr::Uint16 (vpr::ObjectReader::*)() )&vpr::ObjectReader::readUint16))
        .def("readUint32", pure_virtual((vpr::Uint32 (vpr::ObjectReader::*)() )&vpr::ObjectReader::readUint32))
        .def("readUint64", pure_virtual((vpr::Uint64 (vpr::ObjectReader::*)() )&vpr::ObjectReader::readUint64))
        .def("readFloat", pure_virtual((float (vpr::ObjectReader::*)() )&vpr::ObjectReader::readFloat))
        .def("readDouble", pure_virtual((double (vpr::ObjectReader::*)() )&vpr::ObjectReader::readDouble))
        .def("readString", pure_virtual((std::string (vpr::ObjectReader::*)() )&vpr::ObjectReader::readString))
        .def("readBool", pure_virtual((bool (vpr::ObjectReader::*)() )&vpr::ObjectReader::readBool))
        .def("isBinary", &vpr::ObjectReader::isBinary)
        .def("attribExists", &vpr::AttributeMapBase::attribExists)
    ;

}
