/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _JCCL_DEBUG_H_
#define _JCCL_DEBUG_H_

#include <jccl/jcclConfig.h>
#include <vpr/Util/Debug.h>

// JCCL categories.
const vpr::DebugCategory jcclDBG_PERF(vpr::GUID("02acc29d-6f09-4498-bf38-23b02cd39084"), "DBG_JCCL_PERF", "JCCL-PERF:");
const vpr::DebugCategory jcclDBG_CONFIG(vpr::GUID("a0b2de1f-ce63-489b-8248-f03ef56d36f7"), "DBG_CONFIG", "JCCL-CFG:");
const vpr::DebugCategory jcclDBG_RECONFIG(vpr::GUID("94a6e028-b810-40e5-b6fd-e41b1a74db0e"), "DBG_RECONFIG", "JCCL-RCFG:");
const vpr::DebugCategory jcclDBG_PLUGIN(vpr::GUID("810faef0-12fb-11d8-88d1-00045a86e9cd"), "DBG_JCCL_PLUGIN", "JCCL-PLUGIN:");

#endif
