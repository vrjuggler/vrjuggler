// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Includes ====================================================================
#include <boost/python.hpp>
#include <jccl/Config/ConfigTokens.h>

// Using =======================================================================
using namespace boost::python;


// Module ======================================================================
void _Export_VarType()
{
   enum_<jccl::VarType>("VarType")
      .value("T_INT", jccl::T_INT)
      .value("T_FLOAT", jccl::T_FLOAT)
      .value("T_BOOL", jccl::T_BOOL)
      .value("T_STRING", jccl::T_STRING)
      .value("T_ELEMENT_PTR", jccl::T_ELEMENT_PTR)
      .value("T_CHILD_ELEMENT", jccl::T_CHILD_ELEMENT)
      .value("VJ_T_INVALID", jccl::VJ_T_INVALID)
   ;
}
