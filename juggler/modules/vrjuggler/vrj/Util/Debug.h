/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _VRJDebug_h_
#define _VRJDebug_h_
//#pragma once

// #define VJ_DEBUG  Defined in vjConfig.h
#include <vrj/vrjConfig.h>
#include <stdlib.h>

#include <vpr/Util/Debug.h>

// Juggler categories
const vpr::DebugCategory vrjDBG_KERNEL("9cce60c9-9cf9-44e6-a852-1cac224d8b4c", "DBG_KERNEL", "vjKERN:");      /* Kernel output */
const vpr::DebugCategory vrjDBG_DRAW_MGR("04caf702-1886-4ef8-9686-4c0fe234dc68", "DBG_DRAW_MGR", "vjDRAW:");
const vpr::DebugCategory vrjDBG_DISP_MGR("934dc95e-6d5d-4db6-9941-c8fabbcc8995", "DBG_DISP_MGR", "vjDISP:");

#endif
