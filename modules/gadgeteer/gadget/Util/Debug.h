/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _GADGET_DEBUG_H_
#define _GADGET_DEBUG_H_
//#pragma once

#include <gadget/gadgetConfig.h>
#include <stdlib.h>

#include <vpr/Util/Debug.h>

// Gadgeteer categories
const vpr::DebugCategory gadgetDBG_INPUT_MGR(vpr::GUID("d6be4359-e8cf-41fc-a72b-a5b4f3f29aa2"), "DBG_INPUT_MGR", "gadgetINP:");
const vpr::DebugCategory gadgetDBG_RIM(vpr::GUID("2af7e28f-a831-4b7c-b5c9-beda5289ffde"), "DBG_RIM", "gadgetRIM:");
const vpr::DebugCategory gadgetDBG_NET_MGR(vpr::GUID("02be47d5-c5f8-4487-b08c-e99ee23cc1d5"), "DBG_NET_MGR", "gadgetNET:");


#endif
