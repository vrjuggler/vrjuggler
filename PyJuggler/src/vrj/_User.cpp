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
#include <vrj/Kernel/User.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vrj_User_Wrapper: vrj::User
{
    vrj_User_Wrapper(PyObject* self_):
        vrj::User(), self(self_) {}


    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_User()
{
    class_< vrj::User, boost::noncopyable, pyj::vrj_User_Wrapper >("User",
         "Representation for Juggler user in multi-user environments.\n\n"
         "Each user has a system-assigned ID as well as a string name from\n"
         "the 'user' config element that created the user.  These IDs can be\n"
         "used to identify a user at set points in the execution of a VR\n"
         "Juggler application object where user information is given.\n\n"
         "The system-assigned ID number can also be used as an index into a\n"
         "program array (or other data structure) that stores user-specific\n"
         "data such as navigation matrices or input devices."
         ,
         init<  >()
        )
        .def("getId", &vrj::User::getId,
             "getId() -> int\n"
             "Returns the ID of the user in the system.\n\n"
             "Post-conditions:\n"
             "-1 implies that the user has not been configured yet."
         )
        .def("getName", &vrj::User::getName,
             "getName() -> string object\n"
             "Returns the name of thss user object."
         )
        .def("getHeadUpdateTime", &vrj::User::getHeadUpdateTime,
             "getHeadUpdateTime() -> vpr.Interval object\n"
             "Returns the time stamp of the last head tracker update."
         )
        .def("getInterocularDistance", &vrj::User::getInterocularDistance,
             "getInterocularDistance() -> float\n"
             "Gets the eye separation."
         )
    ;

}
