/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_ASSERT_H_
#define _VPR_ASSERT_H_

#include <vpr/vprConfig.h>

/**
 * Assert wrapper.  If debugging is enabled, <code>vprASSERT</code> will
 * trigger an assertion when the given expression evaluates to false.  If
 * debugging is disabled, <code>vprASSERT</code> is a null operation.
 */
#ifdef VPR_DEBUG
#   include <assert.h>
#   include <iostream>
#   include <vpr/SystemBase.h>
#   define vprASSERT(val) { if(!(val)) { std::cout << vpr::SystemBase::getCallStack(); assert((val)); } }
#else
#   define vprASSERT(val) ((void)0)
#endif

#endif
