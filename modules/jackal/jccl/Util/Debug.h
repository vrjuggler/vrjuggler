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

#ifndef _JCCLDebug_h_
#define _JCCLDebug_h_

// #define JCCL_DEBUG  Defined in jcclConfig.h
#include <jccl/jcclConfig.h>
#include <stdlib.h>

#include <vpr/Util/Debug.h>

// Jackal categories
const vpr::GUID jcclDBG_SERVER("1ca1e39f-d798-47f5-acd6-105c4d490476");
const vpr::GUID jcclDBG_PERFORMANCE("02acc29d-6f09-4498-bf38-23b02cd39084");
const vpr::GUID jcclDBG_CONFIG("a0b2de1f-ce63-489b-8248-f03ef56d36f7");
const vpr::GUID jcclDBG_RECONFIGURATION("94a6e028-b810-40e5-b6fd-e41b1a74db0e");

#endif
