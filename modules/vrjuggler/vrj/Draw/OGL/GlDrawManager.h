/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _VRJ_GL_DRAW_MANAGER_H_
#define _VRJ_GL_DRAW_MANAGER_H_

/**
 * \file
 *
 * This file is for backwards compatibility with VR Juggler 2.x code. New
 * code should not include this header and should use
 * vrj/Draw/OpenGL/DrawManager.h.
 *
 * @deprecated This file has been deprecated in favor of
 *             vrj/Draw/OpenGL/DrawManager.h.
 */

#if defined(__GNUC__)
#warning "This header is deprecated. Use vrj/Draw/OpenGL/DrawManager.h instead."
#endif

#include <vrj/Draw/OpenGL/DrawManager.h>


namespace vrj
{

/**
 * Backwards compatibility type declaration for vrj::opengl::DrawManager.
 *
 * @since 2.3.11
 *
 * @deprecated vrj::GlDrawManager has been deprecated in favor of
 *             vrj::opengl::DrawManager.
 */
typedef vrj::opengl::DrawManager GlDrawManager;

}


#endif
