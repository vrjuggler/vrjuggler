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
#include <vrj/Draw/OGL/GlContextData.h>
#include <pyjutil/InterpreterGuard.h>
#include <pyjutil/Debug.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj
{

/**
 * This class defines a context-specific Python dictionary so that Python
 * code can make use of the OpenGL Draw Manager's context-specific data
 * features.  A instance of this class held by Python will result in a unique
 * dictionary in each OpenGL context for the Python object.
 *
 * @note Reading and writing attributes in the Python object will only be
 *       possible when an OpenGL context is active.  At any other time, the
 *       Python interpreter will probably crash.
 */
class ContextDict
{
public:
   object getAttr(const char* attrName)
   {
      vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                               "pyj::ContextDict::getAttr()\n",
                               "pyj::ContextDict::getAttr() done.\n");
      PyJuggler::InterpreterGuard guard;
      object value;

      vprDEBUG(pyjDBG_CXX, vprDBG_HVERB_LVL)
         << "Returning value of attribute '" << attrName << "'" << std::endl
         << vprDEBUG_FLUSH;

      try
      {
         value = (*__dict__)[attrName];
      }
      catch(error_already_set)
      {
         PyErr_Print();
      }

      vprDEBUG(pyjDBG_CXX, vprDBG_HVERB_LVL)
         << "value == " << PyString_AsString(str(value).ptr())
         << std::hex << " (" << value.ptr() << ")" << std::dec << std::endl
         << vprDEBUG_FLUSH;

      return value;
   }

   void setAttr(const char* attrName, object value)
   {
      vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                               "pyj::ContextDict::setAttr()\n",
                               "pyj::ContextDict::setAttr() done.\n");
      PyJuggler::InterpreterGuard guard;

      vprDEBUG(pyjDBG_CXX, vprDBG_HVERB_LVL)
         << "Setting attribute '" << attrName << "' to "
         << PyString_AsString(str(value).ptr())
         << std::hex << " (" << value.ptr() << ")" << std::dec << std::endl
         << vprDEBUG_FLUSH;

      try
      {
         (*__dict__)[attrName] = value;
      }
      catch(error_already_set)
      {
         PyErr_Print();
      }
   }

private:
   /**
    * The context-specific dictionary.
    *
    * @note I had wanted this type to be vrj::GlContextData<dict>, but
    *       Boost.Python generated errors about missing to_python (by-value)
    *       converters.  This type instantiation works, and it probably has a
    *       lot less overhead.
    *
    * @note This type can be switched to vrj::GlContextData<dict> without
    *       changing any of the code above.
    */
   vrj::GlContextData< std::map<const char*, object> > __dict__;
};

}// namespace 


// Module ======================================================================
void _Export_GlContextData()
{
   class_<pyj::ContextDict, boost::noncopyable>("GlContextData",
       "OpenGL helper class that stores context-specific data.\n\n"
       "This class allows the user to store Python objects or data in a\n"
       "context-specific manner.  In other words, there will be one\n"
       "instance of a given Python type per OpenGL context.  Juggler will\n"
       "take care of the data management transparently for the user so that\n"
       "s/he never has to know about the current configuration.\n\n"
       "An example use would be a Python class full of display list IDs.  The\n"
       "user stores instances of the class in an instance of this class.\n"
       "A simple example would be the following:\n\n"
       "class Data:\n"
       "   def __init__(self, v0, v1):\n"
       "      self.data0 = v0\n"
       "      self.data1 = v1\n\n"
       "class AppObj(vrj.GlApp):\n"
       "   def __init__(self):\n"
       "      vrj.GlApp.__init__(self)\n"
       "      self.context_data = vrj.GlContextData()\n\n"
       "   def contextInit(self):\n"
       "      self.context_data.obj = Data(1, 2)\n\n"
       "   def draw(self):\n"
       "      # Read data from self_context_data.obj ...\n\n"
       "More details can be found in the PyJuggler Getting Started Guide."
       ,
       init<>()
      )
      .def("__getattr__", &pyj::ContextDict::getAttr)
      .def("__setattr__", &pyj::ContextDict::setAttr)
   ;
}
