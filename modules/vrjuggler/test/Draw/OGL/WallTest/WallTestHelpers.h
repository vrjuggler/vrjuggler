#ifndef _WALL_TEST_HELPERS_H_
#define _WALL_TEST_HELPERS_H_

#ifdef VPR_OS_Darwin
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
void calcSurfaceParameters(vrj::SurfaceViewport* surf, gmtl::Matrix44f& surfRotation, gmtl::Vec3f& center, float& width, float& height);

#endif

