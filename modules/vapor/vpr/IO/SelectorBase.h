/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
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

#ifndef _VPR_SELECTOR_BASE_H_
#define _VPR_SELECTOR_BASE_H_

#include <vpr/vprConfig.h>


namespace vpr {

/**
 * Common base for all selectors and imp's   
 *
 * @author Allen Bierbaum
 */
class VPR_CLASS_API SelectorBase
{
public:
   enum EventType
      { Read = 1,          /**< Read -  */
        Accept = 1,        /**< Accept - Same as READ  */
        Write = 2,         /**< Write - */
        Except = 4,        /**< Exception - */
        Error = 8,         /**< Error - */
        Invalid = 16       /**< Invalid - Invalid handle */
      };
};

} // End of vpr namespace


#endif	/* _VPR_SELECTOR_BASE_H_ */
