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

#ifndef _VRJDebug_h_
#define _VRJDebug_h_
//#pragma once

// #define VJ_DEBUG  Defined in vjConfig.h
#include <vrj/vrjConfig.h>
#include <stdlib.h>

#include <vpr/Util/Debug.h>

// Juggler categories
const vpr::GUID vrjDBG_KERNEL("9cce60c9-9cf9-44e6-a852-1cac224d8b4c");      /* Kernel output */
vprREGISTER_DBG_CATEGORY(DBG_KERNEL,vrjDBG_KERNEL);

const vpr::GUID vrjDBG_INPUT_MGR("9cce60c9-9cf9-44e6-a852-1cac224d8b4c");       /* Input output */
vprREGISTER_DBG_CATEGORY(DBG_INPUT_MGR,vrjDBG_INPUT_MGR);

const vpr::GUID vrjDBG_DRAW_MGR("04caf702-1886-4ef8-9686-4c0fe234dc68");
vprREGISTER_DBG_CATEGORY(DBG_DRAW_MGR,vrjDBG_DRAW_MGR);

const vpr::GUID vrjDBG_DISP_MGR("934dc95e-6d5d-4db6-9941-c8fabbcc8995");
vprREGISTER_DBG_CATEGORY(DBG_DISP_MGR,vrjDBG_DISP_MGR);

const vpr::GUID vrjDBG_ENV_MGR("1ca1e39f-d798-47f5-acd6-105c4d490476");
vprREGISTER_DBG_CATEGORY(DBG_ENV_MGR,vrjDBG_ENV_MGR);

const vpr::GUID vrjDBG_PERFORMANCE("02acc29d-6f09-4498-bf38-23b02cd39084");
vprREGISTER_DBG_CATEGORY(DBG_PERFORMANCE,vrjDBG_PERFORMANCE);

const vpr::GUID vrjDBG_CONFIG("a0b2de1f-ce63-489b-8248-f03ef56d36f7");
vprREGISTER_DBG_CATEGORY(DBG_CONFIGDB, vrjDBG_CONFIG);

#endif
