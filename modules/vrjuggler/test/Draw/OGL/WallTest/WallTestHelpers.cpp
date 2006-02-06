#include "WallTestHelpers.h"

#include <gadget/Type/Position/PositionUnitConversion.h>

void drawCube()
{
   glBegin(GL_QUADS);
   
   glNormal3f(0,-1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(1,-1,-1);
   glVertex3f(1,-1,1);
   glVertex3f(-1,-1,1);

   glNormal3f(0,1,0);
   glVertex3f(-1,1,-1);
   glVertex3f(-1,1,1);
   glVertex3f(1,1,1);
   glVertex3f(1,1,-1);

   glNormal3f(-1,0,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,1);
   glVertex3f(-1,1,1);
   glVertex3f(-1,1,-1);
   
   glNormal3f(1,0,0);
   glVertex3f(1,-1,-1);
   glVertex3f(1,1,-1);
   glVertex3f(1,1,1);
   glVertex3f(1,-1,1);

   glNormal3f(0,0,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(1,-1,-1);
   glVertex3f(1,1,-1);
   glVertex3f(-1,1,-1);
   
   glNormal3f(0,0,1);
   glVertex3f(-1,-1,1);
   glVertex3f(1,-1,1);
   glVertex3f(1,1,1);
   glVertex3f(-1,1,1);
   
   glEnd();
}

void drawPyramid()
{
   glBegin(GL_TRIANGLES);
   glNormal3f(0,-1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(0,-1,1);
   glVertex3f(1,-1,-1);

   glNormal3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(0,1,0);
   glVertex3f(0,-1,1);

   glNormal3f(0,1,0);
   glVertex3f(0,-1,1);
   glVertex3f(0,1,0);
   glVertex3f(1,-1,-1);

   glNormal3f(0,1,0);
   glVertex3f(1,-1,-1);
   glVertex3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glEnd();

}

gmtl::Matrix44f calculateSurfaceRotation(gmtl::Point3f ll,gmtl::Point3f lr,gmtl::Point3f ul,gmtl::Point3f ur)
{
   // Find the base vectors for the surface axiis (in terms of the base coord system)
   // With z out, x to the right, and y up
   gmtl::Vec3f x_base(lr-ll), 
               y_base(ur-lr), 
               z_base;
   gmtl::cross( z_base, x_base, y_base);

   // They must be normalized
   gmtl::normalize(x_base); 
   gmtl::normalize(y_base); 
   gmtl::normalize(z_base);

   // Calculate the surfaceRotMat using law of cosines
   return gmtl::makeDirCos<gmtl::Matrix44f>(x_base,y_base,z_base);
}


void calcSurfaceParameters(vrj::SurfaceViewport* surf, gmtl::Matrix44f& surfRotation, gmtl::Vec3f& center, float& width, float& height)
{
   gmtl::Point3f ur,lr,ul,ll;
   surf->getCorners(ll,lr,ur,ul);

   // Convert to the units of the application (feet)
   float scale = gadget::PositionUnitConversion::ConvertToFeet;
   ll *= scale;
   lr *= scale;
   ur *= scale;
   ul *= scale;

   // Get the rotation matrix of a surface
   surfRotation = calculateSurfaceRotation(ll,lr,ul,ur);
   center = ll + lr + ul + ur;
   center /= 4.0;
   
   // Get width and height of wall
   width = gmtl::length(gmtl::Vec3f(lr-ll));
   height = gmtl::length(gmtl::Vec3f(ur-lr));   
}



