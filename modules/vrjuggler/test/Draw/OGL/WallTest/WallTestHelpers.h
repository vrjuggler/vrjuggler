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
 
#ifndef _WALL_TEST_HELPERS_H_
#define _WALL_TEST_HELPERS_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>

#include <vrj/Display/SurfaceViewport.h>

/** Draw a unit cube. */
void drawCube();

/** Draw a unit pyramid. */
void drawPyramid();

gmtl::Matrix44f calculateSurfaceRotation(gmtl::Point3f ll,gmtl::Point3f lr,gmtl::Point3f ul,gmtl::Point3f ur);

/** Calculate parameters for the given surface. */
void calcSurfaceParameters(vrj::SurfaceViewportPtr surf,
                           gmtl::Matrix44f& surfRotation, gmtl::Vec3f& center,
                           float& width, float& height);

#endif

