#include <vjConfig.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <Kernel/GL/vjGlWindow.h>
#include <Kernel/vjProjection.h>
#include <Kernel/vjFrustum.h>
#include <Kernel/vjDebug.h>
#include <Kernel/vjSimDisplay.h>
#include <Kernel/vjSurfaceDisplay.h>

// This variable determines which matrix stack we put the viewing transformation
// If it is on the proj matrix, then lighting and env maps work but fog breaks.
#define USE_PROJECTION_MATRIX 1        // Should we put the camera transforms on the
                                       // Projection or modelview matrix


int vjGlWindow::mCurMaxWinId = 0;


void vjGlWindow::config(vjDisplay* _display)
{
   mDisplay = _display;
   mDisplay->getOriginAndSize( origin_x, origin_y, window_width, window_height);
   border = mDisplay->shouldDrawBorder();

   /// Other stuff
}


void vjGlWindow::setLeftEyeProjection()
{
   vjASSERT(mDisplay->getType() == vjDisplay::SURFACE);
   vjSurfaceDisplay* surf_display = dynamic_cast<vjSurfaceDisplay*>(mDisplay);

   if (!window_is_open)
      return;

   vjProjection* left_proj = surf_display->getLeftProj();
   float* frust = left_proj->frustum.frust;

   vjDEBUG(2)  << "---- Left Frustum ----\n"
               << surf_display->getLeftProj()->frustum.frust << endl << vjDEBUG_FLUSH;

      // --- Set to the correct buffer --- //
   if(isStereo())
      glDrawBuffer(GL_BACK_LEFT);
   else
      glDrawBuffer(GL_BACK);

      // --- Set up the projection --- //
   glMatrixMode(GL_PROJECTION);
   {
      glLoadIdentity();             // Load identity matrix
      glFrustum(frust[vjFrustum::LEFT],frust[vjFrustum::RIGHT],
                 frust[vjFrustum::BOTTOM],frust[vjFrustum::TOP],
                 frust[vjFrustum::NEAR],frust[vjFrustum::FAR]);
#ifdef USE_PROJECTION_MATRIX
         // Set camera rotation and position
      glMultMatrixf(left_proj->viewMat.getFloatPtr());
#endif
   }
   glMatrixMode(GL_MODELVIEW);
#ifndef USE_PROJECTION_MATRIX
      // Set camera rotation and position
   glLoadIdentity();
   glMultMatrixf(left_proj->viewMat.getFloatPtr());
#endif
}

/** Sets the projection matrix for this window to draw the right eye frame */
void vjGlWindow::setRightEyeProjection()
{
   vjASSERT(mDisplay->getType() == vjDisplay::SURFACE);
   vjSurfaceDisplay* surf_display = dynamic_cast<vjSurfaceDisplay*>(mDisplay);

   if (!window_is_open)
      return;

   vjProjection* right_proj = surf_display->getRightProj();
   float* frust = right_proj->frustum.frust;

   vjDEBUG(2)  << "---- Right Frustum ----\n"
               << *frust << endl << vjDEBUG_FLUSH;

      // --- Set to the correct buffer --- //
   if(isStereo())
      glDrawBuffer(GL_BACK_RIGHT);
   else
      glDrawBuffer(GL_BACK);

      // --- Set up the projection --- //
   glMatrixMode(GL_PROJECTION);
   {
      glLoadIdentity();             // Load identity matrix
      glFrustum(frust[vjFrustum::LEFT],frust[vjFrustum::RIGHT],
                 frust[vjFrustum::BOTTOM],frust[vjFrustum::TOP],
                 frust[vjFrustum::NEAR],frust[vjFrustum::FAR]);
#ifdef USE_PROJECTION_MATRIX
       // Set camera rotation and position
   glMultMatrixf(right_proj->viewMat.getFloatPtr());
#endif
   }
   glMatrixMode(GL_MODELVIEW);

#ifndef USE_PROJECTION_MATRIX
      // Set camera rotation and position
   glLoadIdentity();
   glMultMatrixf(right_proj->viewMat.getFloatPtr());
#endif
}


/** Sets the projection matrix for this window to draw the camera eye frame */
void vjGlWindow::setCameraProjection()
{
   vjASSERT(mDisplay->getType() == vjDisplay::SIM);
   vjSimDisplay* sim_display = dynamic_cast<vjSimDisplay*>(mDisplay);

   if (!window_is_open)
      return;

   float* frust = sim_display->getCameraProj()->frustum.frust;

   vjDEBUG(2)  << "---- Camera Frustrum ----\n"
               << sim_display->getCameraProj()->frustum.frust << endl << vjDEBUG_FLUSH;

      // --- Set to the correct buffer --- //
   glDrawBuffer(GL_BACK);

      // --- Set up the projection --- //
   glMatrixMode(GL_PROJECTION);
   {
      glLoadIdentity();             // Load identity matrix
      /*
      glFrustum(frust[vjFrustum::LEFT],frust[vjFrustum::RIGHT],
                 frust[vjFrustum::BOTTOM],frust[vjFrustum::TOP],
                 frust[vjFrustum::NEAR],frust[vjFrustum::FAR]);
      */

      gluPerspective(80.0f, float(window_width)/float(window_height),
                     frust[vjFrustum::NEAR], frust[vjFrustum::FAR]);
#ifdef USE_PROJECTION_MATRIX
       // Set camera rotation and position
   glMultMatrixf(sim_display->getCameraProj()->viewMat.getFloatPtr());
#endif
   }
   glMatrixMode(GL_MODELVIEW);

#ifndef USE_PROJECTION_MATRIX
      // Set camera rotation and position
   glLoadIdentity();
   glMultMatrixf(sim_display->getCameraProj()->viewMat.getFloatPtr());
#endif
}


ostream& operator<<(ostream& out, vjGlWindow& win)
{
   out << "-------- vjGlWindow --------" << endl;
   out << "Open: " << win.window_is_open << endl;
   out << "Display:" << *(win.mDisplay) << endl;
   out << "Stereo:" << win.in_stereo << endl;
   return out;
}
