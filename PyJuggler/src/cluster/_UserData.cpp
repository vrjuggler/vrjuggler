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
#include <plugins/ApplicationDataManager/UserData.h>
#include <pyjutil/InterpreterGuard.h>
#include <pyjutil/Debug.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj
{

class PickleObject : public vpr::SerializableObject
{
public:
   PickleObject()
   {
      // Save some memory overhead by doing the module loading and function
      // lookup once and storing the results in static data members.  If the
      // module or function lookup fails, then object pickling and unpickling
      // will not be available at all.
      // XXX: Is there a more elegant way to do this?
      if ( ! mModuleLoaded && ! mLoadAttemptMade )
      {
         mLoadAttemptMade = true;

         try
         {
            pickle_mod = object(borrowed(PyImport_ImportModule("pickle")));
            dict pickle_dict = extract<dict>(pickle_mod.attr("__dict__"));
            pickle_dumps = pickle_dict["dumps"];
            pickle_loads = pickle_dict["loads"];
            mModuleLoaded = true;
         }
         catch(error_already_set)
         {
            PyErr_Print();
            vprDEBUG(pyjDBG_CXX, vprDBG_CRITICAL_LVL)
               << clrOutBOLD(clrRED, "ERROR")
               << ": Failed to look up pickle.dumps() and/or pickle.loads()!"
               << std::endl << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(pyjDBG_CXX, vprDBG_CRITICAL_LVL)
               << "PyJuggler.cluster.UserData object sharing will not be "
               << "available!" << std::endl << vprDEBUG_FLUSH;
         }
      }
   }

   void setPythonObject(object obj)
   {
      self     = obj;
      __dict__ = extract<dict>(obj.attr("__dict__"));
   }

   vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0)
   {
      vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                               "pyj::PIckleObject::writeObject()\n",
                               "pyj::PIckleObject::writeObject() done.\n");
      PyJuggler::InterpreterGuard guard;

      vpr::ReturnStatus status;

      // There is nothing to be done until self has been assigned a value.
      if ( self.ptr() != NULL && mModuleLoaded )
      {
         try
         {
            // Pickle the object to be shared.
            str pickled_obj(pickle_dumps(self));

            vprDEBUG(pyjDBG_CXX, vprDBG_HEX_LVL)
               << "pickled_obj: " << PyString_AsString(pickled_obj.ptr())
               << std::endl << vprDEBUG_FLUSH;

            p0->writeString(std::string(PyString_AsString(pickled_obj.ptr())));
         }
         catch(error_already_set)
         {
            vprDEBUG(pyjDBG_CXX, vprDBG_WARNING_LVL)
               << clrOutBOLD(clrYELLOW, "WARNING")
               << ": Caught a Python exception in pyj::PythonObject::writeObject()"
               << std::endl << vprDEBUG_FLUSH;
            PyErr_Print();
            status.setCode(vpr::ReturnStatus::Fail);
         }
      }

      return status;
   }

   vpr::ReturnStatus readObject(vpr::ObjectReader* p0)
   {
      vpr::DebugOutputGuard og(pyjDBG_CXX, vprDBG_VERB_LVL,
                               "pyj::PIckleObject::readObject()\n",
                               "pyj::PIckleObject::readObject() done.\n");
      PyJuggler::InterpreterGuard guard;

      vpr::ReturnStatus status;

      // There is nothing to be done until self has been assigned a value.
      if ( self.ptr() != NULL && mModuleLoaded )
      {
         try
         {
            str pystring_rep(p0->readString());

            vprDEBUG(pyjDBG_CXX, vprDBG_HEX_LVL)
               << "pystring_rep: " << PyString_AsString(pystring_rep.ptr())
               << std::endl << vprDEBUG_FLUSH;

            // Un-pickle the received object.
            object obj = pickle_loads(pystring_rep);

            // Pull out the dictionary for the un-pickled object and
            // prepare to copy its contents into self.__dict__.
            dict obj_dict = extract<dict>(obj.attr("__dict__"));
            list attr_names(obj_dict.keys());
            int length = extract<int>(attr_names.attr("__len__")());

            // Clear the dictionary of self before we start mucking around
            // with it.
            __dict__.clear();

            // Iterate over the attributes in the un-pickled object and
            // copy the values into self.
            for ( int i = 0; i < length; ++i )
            {
               str name(attr_names[i]);
               __dict__[name] = obj_dict[name];
            }
         }
         catch(error_already_set)
         {
            vprDEBUG(pyjDBG_CXX, vprDBG_WARNING_LVL)
               << clrOutBOLD(clrYELLOW, "WARNING")
               << ": Caught a Python exception in pyj::PythonObject::readObject()"
               << std::endl << vprDEBUG_FLUSH;
            PyErr_Print();
            status.setCode(vpr::ReturnStatus::Fail);
         }
      }

      return status;
   }

private:
   object self;
   dict __dict__;

   static bool mModuleLoaded;
   static bool mLoadAttemptMade;
   static object pickle_mod;
   static object pickle_dumps;
   static object pickle_loads;
};

bool   PickleObject::mModuleLoaded(false);
bool   PickleObject::mLoadAttemptMade(false);
object PickleObject::pickle_mod;
object PickleObject::pickle_dumps;
object PickleObject::pickle_loads;

static
void setContainedUserDataObject(cluster::UserData<pyj::PickleObject>* self_,
                                object obj)
{
   (*self_)->setPythonObject(obj);
}

}

// Module ======================================================================
void _Export_UserData()
{
   class_<cluster::UserData<pyj::PickleObject>, boost::noncopyable>("UserData",
       "__init__()\n"
       "Creates a new instance of a Python object container for use with\n"
       "VR Juggler clustering.  The Python object contained within a\n"
       "cluster.UserData instance will be distributed to all the nodes of\n"
       "a cluster.\n\n"
       "cluster.UserData is based on the C++ type cluster::UserData<T>, but\n"
       "its usage takes advantage of Python features to simplify the usage.\n"
       "In the C++ version, the contained type must be serializable (it\n"
       "must derive from the C++ type vpr::SerializableObject), but with\n"
       "cluster.UserData, the requirement is that the contained object\n"
       "support pickling (Python's version of serialization).  Most Python\n"
       "objects support pickling by default, so no additional code must be\n"
       "written in most cases.\n\n"
       "The basic usage in a VR Juggler application object is as follows:\n\n"
       "class AppObj(vrj.GlApp):\n"
       "   def __init__(self):\n"
       "      vrj.GlApp.__init__(self)\n"
       "      self.user_data   = cluster.UserData()\n"
       "      self.pickled_obj = MyObj()\n\n"
       "   def init(self):\n"
       "      self.user_data.init(vpr.GUID(\"...\")\n"
       "      self.user_data.setPickleObject(self.pickled_obj)\n\n"
       "   def preFrame(self):\n"
       "      if self.user_data.isLocal():\n"
       "         # Modify self.pickled_obj ...\n\n"
       "   def latePfreFrame(self):\n"
       "      # Set up rendering state based on self.pickled_obj ...\n\n"
       "More details can be found in the PyJuggler Getting Started Guide."
       ,
       init<>()
      )
      .def("init", &cluster::UserData<pyj::PickleObject>::init,
           (args("id"), args("writerAddr") = ""),
           "init(id, writerAddr = '')\n"
           "Initializes this application-specific shared data object with\n"
           "the given vpr.GUID instance.  There must be a distinct GUID for\n"
           "for each instance of shared data, even if the instances are all\n"
           "of the same type.\n\n"
           "If the second argument is not given, the writer (master) host\n"
           "must be identified using a configuration element of type\n"
           "application_data.  Otherwise, the given string object is used\n"
           "to identify the writer host.  Naming the writer host explicitly\n"
           "is somewhat easier to use, but it can be less flexible.\n"
           "Keyword arguments:\n"
           "id         -- A vpr.GUID object that uniquely identifies this\n"
           "              instance of application-specific shared data.\n"
           "writerAddr -- The cluster node (hostnameo or IP address) that\n"
           "              will be the writer of the shared data.  This\n"
           "              argument is optional."
       )
      .def("isLocal", &cluster::UserData<pyj::PickleObject>::isLocal,
           "isLocal() -> Boolean\n"
           "Returns whether this cluster node is responsible for updating\n"
           "the application-specific shared data.  This determination is\n"
           "based on which node was identified as the writer when init()\n"
           "was called.\n"
           "Pre-condition:\n"
           "init() has been invoked."
       )
      .def("setPickleObject", pyj::setContainedUserDataObject,
           "setPickleObject(object)\n"
           "Stores the given Python object as the object to be pickled and\n"
           "un-pickled by the data-sharing code.  The object must support\n"
           "pickling.  Refer to the Python pickling documentation, which\n"
           "can be found at http://docs.python.org/lib/module-pickle.html\n"
           "among other sites."
       )
   ;
}
