/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _VJ_GL_WINDOW_H_
#define _VJ_GL_WINDOW_H_

/**
 * \file
 *
 * This file is for backwards compatibility with VR Juggler 2.x code. New
 * code should not include this header and should use
 * vrj/Draw/OpenGL/Window.h.
 *
 * @deprecated This file has been deprecated in favor of
 *             vrj/Draw/OpenGL/Window.h.
 */

#if defined(__GNUC__)
#warning "This header is deprecated. Use vrj/Draw/OpenGL/Window.h instead."
#endif

#include <vrj/Draw/OpenGL/Window.h>
#include <vrj/Draw/OGL/GlWindowPtr.h>


namespace vrj
{

/**
 * Backwards compatibility type declaration for vrj::opengl::Window.
 *
 * @since 2.3.11
 *
 * @deprecated vrj::GlWindow has been deprecated in favor of
 *             vrj::opengl::Window.
 */
typedef vrj::opengl::Window GlWindow;

} // End of vrj namespace


#endif
