// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _PYJUGGLER_VRJ_HELPERS_H_
#define _PYJUGGLER_VRJ_HELPERS_H_


namespace vrjHelpers
{

   template<typename OBJ_TYPE, typename DATA_TYPE>
   void setArrayElement(OBJ_TYPE* obj, const unsigned i, DATA_TYPE value)
   {
      (*obj)[i] = value;
   }

}

#endif
