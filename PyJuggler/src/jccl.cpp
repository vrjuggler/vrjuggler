/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling
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

// Includes ====================================================================
#include <boost/python.hpp>
#include <jccl/RTRC/ConfigChunkHandler.h>
#include <jccl/Config/ConfigChunk.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


struct jccl_ConfigChunkHandler_Wrapper: jccl::ConfigChunkHandler
{
    jccl_ConfigChunkHandler_Wrapper(PyObject* self_, const jccl::ConfigChunkHandler & p0):
        jccl::ConfigChunkHandler(p0), self(self_) {}

    jccl_ConfigChunkHandler_Wrapper(PyObject* self_):
        jccl::ConfigChunkHandler(), self(self_) {}

    bool configCanHandle(boost::shared_ptr<jccl::ConfigChunk> p0) {
        try
        {
            return call_method< bool >(self, "configCanHandle", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    int configProcessPending() {
        try
        {
            return call_method< int >(self, "configProcessPending");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return 0;
    }

    int default_configProcessPending() {
        return jccl::ConfigChunkHandler::configProcessPending();
    }

    bool configAdd(boost::shared_ptr<jccl::ConfigChunk> p0) {
        try
        {
            return call_method< bool >(self, "configAdd", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool configRemove(boost::shared_ptr<jccl::ConfigChunk> p0) {
        try
        {
            return call_method< bool >(self, "configRemove", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(jccl)
{
    scope* jccl_ConfigChunk_scope = new scope(
    class_< jccl::ConfigChunk >("ConfigChunk", init<  >())
        .def(init< boost::shared_ptr<jccl::ChunkDesc> >())
        .def(init< const jccl::ConfigChunk & >())
        .def("isValid", &jccl::ConfigChunk::isValid)
        .def("assertValid", &jccl::ConfigChunk::assertValid)
        .def("initFromNode", &jccl::ConfigChunk::initFromNode)
        .def("getChildChunk", &jccl::ConfigChunk::getChildChunk)
        .def("getNum", &jccl::ConfigChunk::getNum)
        .def("getName", &jccl::ConfigChunk::getName)
        .def("getFullName", &jccl::ConfigChunk::getFullName)
        .def("getDescToken", &jccl::ConfigChunk::getDescToken)
        .def("setProperty", (bool (jccl::ConfigChunk::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> > &, const int, bool) )&jccl::ConfigChunk::setProperty)
        .def("setProperty", (bool (jccl::ConfigChunk::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> > &, int, boost::shared_ptr<jccl::ConfigChunk>) )&jccl::ConfigChunk::setProperty)
        .def("getChunkPtrDependencies", &jccl::ConfigChunk::getChunkPtrDependencies)
        .def("getEmbeddedChunks", &jccl::ConfigChunk::getEmbeddedChunks)
        .def("setDesc", &jccl::ConfigChunk::setDesc)
        .def("getNode", &jccl::ConfigChunk::getNode)
        .def(self_ns::str(self))
        .def( self == self )
        .def( self != self )
        .def( self < self )
    );
    // Temporary code for smart pointers
    objects::class_value_wrapper< 
      boost::shared_ptr< jccl::ConfigChunk >, objects::make_ptr_instance< 
        jccl::ConfigChunk, objects::pointer_holder< 
          boost::shared_ptr< jccl::ConfigChunk >, jccl::ConfigChunk >
      >
    >();
    delete jccl_ConfigChunk_scope;

    class_< jccl::ConfigChunkHandler, boost::noncopyable, jccl_ConfigChunkHandler_Wrapper >("ConfigChunkHandler", init<  >())
        .def("configProcessPending", &jccl::ConfigChunkHandler::configProcessPending, &jccl_ConfigChunkHandler_Wrapper::default_configProcessPending)
    ;

}
