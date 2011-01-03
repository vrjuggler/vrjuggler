/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_MOUSE_EVENT_COMPAT_H_
#define _GADGET_MOUSE_EVENT_COMPAT_H_

/**
 * \file
 *
 * This file is for backwards compatibility with VR Juggler 3.x code. New
 * code should not include this header and should use
 * gadget/Event/KeyboardMouse/MouseEvent.h.
 *
 * @deprecated This file has been deprecated in favor of
 *             gadget/Event/KeyboardMouse/MouseEvent.h.
 */

#if defined(__GNUC__)
#warning "This header is deprecated. Use gadget/Event/KeyboardMouse/MouseEvent.h instead."
#endif

#include <gadget/Event/KeyboardMouse/MouseEvent.h>

#endif /* _GADGET_MOUSE_EVENT_COMPAT_H_ */
