#include <config.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <Kernel/GL/vjGlWindow.h>
#include <Kernel/vjProjection.h>
#include <Kernel/vjFrustum.h>
#include <Kernel/vjDebug.h>

#define USE_PROJECTION_MATRIX 1        // Should we put the camera transforms on the 
                                       // Projection or modelview matrix

int vjGlWindow::mCurMaxWinId = 0;

void vjGlWindow::setLeftEye() {
   if (!window_is_open)
      return;

   float* frust = display->leftProj->frustum.frust;

   vjDEBUG(2)  << "---- Left Frustum ----\n"
               << display->leftProj->frustum.frust << endl;

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
      glMultMatrixf(display->leftProj->viewMat.getFloatPtr());
#endif
   }
   glMatrixMode(GL_MODELVIEW);
#ifndef USE_PROJECTION_MATRIX
      // Set camera rotation and position
   glLoadIdentity();
   glMultMatrixf(display->leftProj->viewMat.getFloatPtr());
#endif
}

/** Sets the projection matrix for this window to draw the right eye frame */
void vjGlWindow::setRightEye()
{
   if (!window_is_open)
      return;

   float* frust = display->rightProj->frustum.frust;
   
   vjDEBUG(2)  << "---- Right Frustum ----\n"
               << display->rightProj->frustum.frust << endl;

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
   glMultMatrixf(display->rightProj->viewMat.getFloatPtr());
#endif
   }
   glMatrixMode(GL_MODELVIEW);

#ifndef USE_PROJECTION_MATRIX
      // Set camera rotation and position
   glLoadIdentity();
   glMultMatrixf(display->rightProj->viewMat.getFloatPtr());
#endif
}


/** Sets the projection matrix for this window to draw the camera eye frame */
void vjGlWindow::setCameraEye()
{
   if (!window_is_open)
      return;

   //float* frust = display->cameraProj->frustum.frust;
   
   vjDEBUG(2)  << "---- Camera Frustrum ----\n"
               << display->cameraProj->frustum.frust << endl;

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
      gluPerspective(80.0f, 1.0f, 0.1, 1000);
#ifdef USE_PROJECTION_MATRIX
       // Set camera rotation and position
   glMultMatrixf(display->cameraProj->viewMat.getFloatPtr());
#endif
   }
   glMatrixMode(GL_MODELVIEW);

#ifndef USE_PROJECTION_MATRIX
      // Set camera rotation and position
   glLoadIdentity();
   glMultMatrixf(display->cameraProj->viewMat.getFloatPtr());
#endif
}


ostream& operator<<(ostream& out, vjGlWindow& win)
{
   out << "-------- vjGlWindow --------" << endl;
   out << "Open: " << win.window_is_open << endl;
   out << "Display:" << *(win.display) << endl;
   out << "Stereo:" << win.in_stereo << endl;
   return out;
}
