/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <vrj/vjConfig.h>

#include <vpr/Thread/Thread.h>

#include <vrj/Draw/OGL/GlDrawManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Kernel/Kernel.h>

#include <vrj/Display/Display.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Display/SurfaceViewport.h>

#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Input/Type/Glove.h>
#include <vrj/Input/Type/GloveProxy.h>

#include <vrj/Draw/OGL/GlPipe.h>
#include <vrj/Draw/OGL/GlWindow.h>

#include <vrj/Math/Vec3.h>
#include <vrj/Math/Vec4.h>

namespace vrj
{
   
//vjGlDrawManager* GlDrawManager::_instance = NULL;
vprSingletonImp(GlDrawManager);


//: Set the app the draw should interact with.
void GlDrawManager::setApp(App* _app)
{
   mApp = dynamic_cast<GlApp*>(_app);

   // We have a new app, so the contexts must be re-initialized
   // so... dirty them all.
   dirtyAllWindows();
   vprASSERT(mApp != NULL);
}

//: Return the app we are rednering
GlApp* GlDrawManager::getApp()
{ return mApp; }


//: Do initial configuration for the draw manager
// Doesn't do anything right now
/*
void GlDrawManager::configInitial(ConfigChunkDB*  chunkDB)
{
    // Setup any config data
}
*/

//: Start the control loop
void GlDrawManager::start()
{
   // --- Setup Multi-Process stuff --- //
   // Create a new thread to handle the control
   vpr::Thread* control_thread;

   vpr::ThreadMemberFunctor<GlDrawManager>* memberFunctor =
      new vpr::ThreadMemberFunctor<GlDrawManager>(this, &GlDrawManager::main, NULL);

   control_thread = new vpr::Thread(memberFunctor);

   vjDEBUG(vjDBG_DRAW_MGR,1) << "vjGlDrawManager started. thread: "
                             << control_thread << std::endl << vjDEBUG_FLUSH;
}


    // Enable a frame to be drawn
    // Trigger draw
void GlDrawManager::draw()
{
   drawTriggerSema.release();
}


//: Blocks until the end of the frame
//! POST:
//+      The frame has been drawn
void GlDrawManager::sync()
{
   drawDoneSema.acquire();
}


//: This is the control loop for the manager
void GlDrawManager::main(void* nullParam)
{
   //while(!Exit)
   while (1)
   {
      //**// Runtime config will happen here
      // Because the kernel is the only one that can trigger it
      // we will be waiting here at that time

      // Wait for trigger
      drawTriggerSema.acquire();

      // THEN --- Do Rendering --- //
      drawAllPipes();

      // -- Done rendering --- //
      drawDoneSema.release();

      // Allow run-time config
      //**//mRuntimeConfigSema.release();
         // This is the time that reconfig can happen
      // configProcessPending();
      //**//mRuntimeConfigSema.acquire();
   }
}

void GlDrawManager::drawAllPipes()
{
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,vjDBG_HVERB_LVL)
      << "vjGLDrawManager::drawAllPipes: " << std::endl << std::flush
      << vjDEBUG_FLUSH;
   unsigned int pipeNum;

   // RENDER
      // Start rendering all the pipes
   for(pipeNum=0; pipeNum<pipes.size(); pipeNum++)
      pipes[pipeNum]->triggerRender();

      // Wait for rendering to finish on all the pipes
   for(pipeNum=0; pipeNum<pipes.size(); pipeNum++)
      pipes[pipeNum]->completeRender();

   // SWAP
      // Start swapping all the pipes
   for(pipeNum=0; pipeNum<pipes.size(); pipeNum++)
      pipes[pipeNum]->triggerSwap();

      // Wait for swapping to finish on all the pipes
   for(pipeNum=0; pipeNum<pipes.size(); pipeNum++)
      pipes[pipeNum]->completeSwap();


   vjDEBUG_END(vjDBG_DRAW_MGR,vjDBG_HVERB_LVL)
      << "vjGLDrawManager::drawAllPipes: Done" << std::endl << std::flush
      << vjDEBUG_FLUSH;
}

//: Initialize the drawing API (if not already running)
//! POST: Control thread is started
void GlDrawManager::initAPI()
{
   start();
}


//: Callback when display is added to display manager
//! PRE: Must be in kernel controlling thread
//! NOTE: This function can only be called by the display manager
//+      functioning on behalf of a thread the holds the kernel
//+      reconfiguration lock.
//+      This guarantees that we are not rendering currently.
//+      We will most likely be waiting for a render trigger.
void GlDrawManager::addDisplay(Display* disp)
{
   vprASSERT(disp != NULL);    // Can't add a null display

   vjDEBUG(vjDBG_DRAW_MGR,3) << "vrj::GlDrawManager:addDisplay: " << disp
                             << std::endl << vjDEBUG_FLUSH;

   // -- Create a window for new display
   // -- Store the window in the wins vector
   // Create the gl window object.  NOTE: The glPipe actually "creates" the opengl window and context later
   GlWindow* new_win = getGLWindow();
   new_win->config(disp);                                            // Configure it
   mWins.push_back(new_win);                                         // Add to our local window list

   // -- Create any needed Pipes & Start them
   unsigned int pipe_num = new_win->getDisplay()->getPipe();    // Find pipe to add it too

   if(pipes.size() < (pipe_num+1))           // ASSERT: Max index of pipes is < our pipe
   {                                         // +1 because if pipeNum = 0, I still need size() == 1
      while(pipes.size() < (pipe_num+1))     // While we need more pipes
      {
         GlPipe* new_pipe = new GlPipe(pipes.size(), this);  // Create a new pipe to use
         pipes.push_back(new_pipe);                          // Add the pipe
         new_pipe->start();                                  // Start the pipe running
                                                             // NOTE: Run pipe even if now windows.  Then it waits for windows.
      }
   }

   // -- Add window to the correct pipe
   GlPipe* pipe;                           // The pipe to assign it to
   pipe = pipes[pipe_num];                    // ASSERT: pipeNum is in the valid range
   pipe->addWindow(new_win);              // Window has been added

   vprASSERT(isValidWindow(new_win));      // Make sure it was added to draw manager

   // Dump the state
   vjDEBUG(vjDBG_DRAW_MGR, 1) << "Reconfiged the glDrawManager.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, 1) << (*this) << vjDEBUG_FLUSH;
}


//: Callback when display is removed to display manager
//! PRE: disp must be a valid display that we have
//! POST: window for disp is removed from the draw manager and child pipes
void GlDrawManager::removeDisplay(Display* disp)
{
   GlPipe* pipe;  pipe = NULL;
   GlWindow* win; win = NULL;     // Window to remove

   for(unsigned int i=0;i<mWins.size();i++)
   {
      if(mWins[i]->getDisplay() == disp)      // FOUND it
      {
         win = mWins[i];
         pipe = pipes[win->getDisplay()->getPipe()];
      }
   }

   // Remove the window from the pipe and our local list
   if(win != NULL)
   {
      vprASSERT(pipe != NULL);
      vprASSERT(isValidWindow(win));
      pipe->removeWindow(win);                                                   // Remove from pipe
      mWins.erase(std::remove(mWins.begin(),mWins.end(),win), mWins.end());      // Remove from draw manager
      vprASSERT(!isValidWindow(win));
   }
   else
   {
      vjDEBUG(vjDBG_ERROR, 0) << clrOutNORM(clrRED,"ERROR:") << "vrj::GlDrawManager::removeDisplay: Attempted to remove a display that was not found.\n" << vjDEBUG_FLUSH;
      vprASSERT(false);
   }

}


/// Shutdown the drawing API
void GlDrawManager::closeAPI()
{
   vjDEBUG(vjDBG_DRAW_MGR,0) << "vrj::GlDrawManager::closeAPI: NOT IMPLEMENTED.\n" << vjDEBUG_FLUSH;
    // Stop all pipes
   ;
    // Delete all pipes

    // Close and delete all glWindows
}

/////// CHUNK HANDLERS ////////////////////
//: Add the chunk to the draw manager config
//! PRE: configCanHandle(chunk) == true
//! POST: The chunks have reconfigured the system
bool GlDrawManager::configAdd(ConfigChunk* chunk)
{
   return false;
}

//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//!RETURNS: success
bool GlDrawManager::configRemove(ConfigChunk* chunk)
{
   return false;
}

//: Can the handler handle the given chunk?
//! RETURNS: false - We can't handle anything
bool GlDrawManager::configCanHandle(ConfigChunk* chunk)
{
   return false;
}


//: Set the dirty bits off all the gl windows
// Dirty all the window contexts
void GlDrawManager::dirtyAllWindows()
{
    // Create Pipes & Add all windows to the correct pipe
   for(unsigned int winId=0;winId<mWins.size();winId++)   // For each window we created
   {
      mWins[winId]->setDirtyContext(true);
   }
}


bool GlDrawManager::isValidWindow(GlWindow* win)
{
   bool ret_val = false;
   for(unsigned int i=0;i<mWins.size();i++)
      if(mWins[i] == win)
         ret_val = true;

   return ret_val;
}


// Draw any objects that we need to display in the scene
// from the system.  (i.e. Gloves, etc)
// XXX: Performance Critical problems here
void GlDrawManager::drawObjects()
{
   glPushAttrib( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT );
   {
      glDisable(GL_LIGHTING);
      glDisable(GL_BLEND);
      //vjInputManager*  input_mgr = Kernel::instance()->getInputManager();

      // Draw all glove Proxies that have drawing flag set
      /* XXX: Broken because we don't have list anymore
      GloveProxy* cur_glove_proxy;
      for (int glv = 0; glv < input_mgr->getNumGloveProxies(); glv++)    // For each glove in system
      {
         cur_glove_proxy = input_mgr->getGloveProxy(glv);         // Get the glove proxy
         if (cur_glove_proxy->isVisible())                         // If flag set
            drawGlove( cur_glove_proxy );                           // draw it
      }
      */

      // Draw any other object that need to be seen
   }
   glPopAttrib();
}


// Draw the projections
//!POST: Draws the projections
//+      If withApex, then it draws the frustums with different colors
//+      If !withApex, then just draws the surfaces in all white
void GlDrawManager::drawProjections(bool drawFrustum, Vec3 surfColor)
{
   const float ALPHA_VALUE(0.25f);

   std::vector<Display*> disps = mDisplayManager->getAllDisplays();

   Vec3 apex, ur, lr, ul, ll;
   Projection* proj; proj = NULL;

   for (unsigned int i=0;i<disps.size();i++)
   {
      for (unsigned int v=0;v<disps[i]->getNumViewports();v++)
      {
         Viewport* view_port = disps[i]->getViewport(v);

         if (view_port->isSurface())
         {
            // Get a pointer to the surface
            SurfaceViewport* surf_vp = dynamic_cast<SurfaceViewport*>(view_port);
            vprASSERT(surf_vp != NULL);
            proj = surf_vp->getLeftProj();

            // Create color values that are unique
            // Basically count in binary (skipping 0), and use the first 3 digits.  That will give six colors
            int red_on = (i & 0x1); int green_on = ((i >> 1) & 0x1); int blue_on = ((i >> 2) & 0x1);

            float red(0.0f), green(0.0f), blue(0.0f);
            if (red_on > 0) red = 1.0f;
            if (green_on > 0) green = 1.0f;
            if (blue_on > 0) blue = 1.0f;

            if ((!red_on) && (!blue_on) && (!green_on))      // Case of 0's (black is bad)
               red = blue = green = 0.75f;

            Vec3 surf_color;
            if (drawFrustum)
            {
               surf_color = Vec3(red,blue,green);
            }
            else
            {
               surf_color = surfColor;
            }

            // Compute scaled colors for the corners
            // ll is going to be lighter and upper right is going to be darker
            const float ll_scale(0.10f); const float ul_scale(0.55); const float ur_scale(1.0f);
            Vec4 ll_clr(ll_scale*surf_color[0],ll_scale*surf_color[1],ll_scale*surf_color[2],ALPHA_VALUE);
            Vec4 ul_clr(ul_scale*surf_color[0],ul_scale*surf_color[1],ul_scale*surf_color[2],ALPHA_VALUE);
            Vec4 lr_clr(ul_scale*surf_color[0],ul_scale*surf_color[1],ul_scale*surf_color[2],ALPHA_VALUE);
            Vec4 ur_clr(ur_scale*surf_color[0],ur_scale*surf_color[1],ur_scale*surf_color[2],ALPHA_VALUE);

            // Draw the thingy
            proj->getFrustumApexAndCorners(apex, ur, lr, ul, ll);
            glColor4fv(ur_clr.vec);
            glPushMatrix();
            if (drawFrustum)
            {
               drawLine(apex, ur); drawLine(apex, lr); drawLine(apex, ul); drawLine(apex, ll);
            }

            // Draw the outline
            drawLine(ur, lr); drawLine(lr, ll); drawLine(ll, ul); drawLine(ul, ur);

            // Draw the surface
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_TRIANGLES);
            glColor4fv(ll_clr.vec);  glVertex3fv(ll.vec);
            glColor4fv(lr_clr.vec);  glVertex3fv(lr.vec);
            glColor4fv(ur_clr.vec);  glVertex3fv(ur.vec);

            glColor4fv(ur_clr.vec); glVertex3fv(ur.vec);
            glColor4fv(ul_clr.vec); glVertex3fv(ul.vec);
            glColor4fv(ll_clr.vec); glVertex3fv(ll.vec);
            glEnd();
            glDisable(GL_BLEND);
            glPopMatrix();
         }  // if surface
      }  // for viewports
   }  // for disps
}

//: Draw a simulator using OpenGL commands
//! NOTE: This is called internally by the library
void GlDrawManager::drawSimulator(SimViewport* sim_vp)
{
   const float head_radius(0.60f);      // 7.2 inches
   const float eye_vertical(0.22f);
   const float eye_horizontal(0.7f);
   const float interocular(0.27f);
   const float eye_radius(0.08f);

   glPushAttrib( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT);
   {
      //-----------------set up materials....
      float mat_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
      float mat_shininess[] = {50.0f};
      float mat_diffuse[] = {.7f, .7f, .7f, 1.0f};
      float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
      //-----------------Call Materials.....
      glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
      glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
      //----------------Enable Materials.....
      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);

      glDisable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_1D);

      // Draw base coordinate axis
      ///*
      glDisable(GL_LIGHTING);
      glPushMatrix();
         Vec3 x_axis(2.0f,0.0f,0.0f); Vec3 y_axis(0.0f, 2.0f, 0.0f); Vec3 z_axis(0.0f, 0.0f, 2.0f); Vec3 origin(0.0f, 0.0f, 0.0f);
         glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 0.0f); glVertex3fv(origin.vec); glVertex3fv(x_axis.vec);
            glColor3f(0.0f, 1.0f, 0.0f); glVertex3fv(origin.vec); glVertex3fv(y_axis.vec);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex3fv(origin.vec); glVertex3fv(z_axis.vec);
         glEnd();
      glPopMatrix();
      //*/

      glEnable(GL_LIGHTING);
      // Draw the user's head
      glPushMatrix();
         glMultMatrixf(sim_vp->getHeadPos().getFloatPtr());

         //glEnable(GL_BLEND);
         //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glColor4f(0.5f, 0.75f, 0.90f, 0.67f);
         drawSphere(head_radius, 10, 10);             // Head
         glDisable(GL_BLEND);

         glPushMatrix();
            glColor3f(0.2f, 0.2f, 0.2f);
            glTranslatef(0.0f, eye_vertical, -eye_horizontal);
            glPushMatrix();                     // Right eye
               glTranslatef((interocular/2.0f), 0.0f, 0.0f);
               drawSphere(eye_radius, 5, 5);
            glPopMatrix();
            glPushMatrix();                     // Left eye
               glTranslatef(-(interocular/2.0f), 0.0f, 0.0f);
               drawSphere(eye_radius, 5, 5);
            glPopMatrix();
         glPopMatrix();
      glPopMatrix();

      // Draw the wand
      glPushMatrix();
         glMultMatrixf(sim_vp->getWandPos().getFloatPtr());
         glColor3f(0.0f, 1.0f, 0.0f);
         drawCone(0.2f, 0.6f, 6, 1);
      glPopMatrix();

       // Draw a The display surfaces
      glDisable(GL_LIGHTING);
      glPushMatrix();
         drawProjections(sim_vp->shouldDrawProjections(), sim_vp->getSurfaceColor());
      glPopMatrix();
   }
   glPopAttrib();
}

    /// dumps the object's internal state
void GlDrawManager::outStream(std::ostream& out)
{
    out     << clrSetNORM(clrGREEN)
            << "========== GlDrawManager: " << (void*)this << " ========="
            << clrRESET << std::endl
            << clrOutNORM(clrCYAN,"\tapp:") << (void*)mApp << std::endl
            << clrOutNORM(clrCYAN,"\tWins:") << mWins.size() << std::endl
            << std::flush;

    for(unsigned int i = 0; i < mWins.size(); i++)
    {
       vprASSERT(mWins[i] != NULL);
       out << clrOutNORM(clrCYAN,"\tGlWindow:\n") << mWins[i] << std::endl;
    }
    out << "=======================================" << std::endl;
}

void GlDrawManager::initQuadObj()
{
   if (mQuadObj == NULL)
      mQuadObj = gluNewQuadric();
}

void GlDrawManager::drawLine(Vec3& start, Vec3& end)
{
   glBegin(GL_LINES);
      glVertex3fv(start.vec);
      glVertex3fv(end.vec);
   glEnd();
}

void GlDrawManager::drawSphere(float radius, int slices, int stacks)
{
  initQuadObj();
  gluQuadricDrawStyle(mQuadObj, (GLenum) GLU_FILL);
  gluQuadricNormals(mQuadObj, (GLenum) GLU_SMOOTH);
  gluSphere(mQuadObj, radius, slices, stacks);
}


void GlDrawManager::drawCone(float base, float height, int slices, int stacks)
{
  initQuadObj();
  gluQuadricDrawStyle(mQuadObj, (GLenum) GLU_FILL);
  gluQuadricNormals(mQuadObj, (GLenum) GLU_SMOOTH);
  gluCylinder(mQuadObj, base, 0.0, height, slices, stacks);
}



void GlDrawManager::drawBox(float size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };

  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(type);
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]);
    glVertex3fv(&v[faces[i][1]][0]);
    glVertex3fv(&v[faces[i][2]][0]);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}


void GlDrawManager::drawWireCube(float size)
{
  drawBox(size, GL_LINE_LOOP);
}

void GlDrawManager::drawSolidCube(float size)
{
  drawBox(size, GL_QUADS);
}

// Draw a glove outline for the user
// This may be ugly for now.
// For each finger, step down it's xforms drawing the finger
// links as you go.
void GlDrawManager::drawGlove(GloveProxy* gloveProxy)
{
   Matrix    base_glove_pos = gloveProxy->getPos();    // Get the location of the base coord system
   GloveData gd = gloveProxy->getData();               // Get the glove data
   Vec3      origin(0.0f,0.0f,0.0f);

   //glPushAttrib( GL_ENABLE_BIT | GL_LIGHTING_BIT );
   {
      //-----------------set up materials....
      float mat_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
      float mat_shininess[] = {50.0f};
      float mat_diffuse[] = {.7f, .7f, .7f, 1.0f};
      float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
      //-----------------Call Materials.....
      glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
      glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
      glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
      //----------------Enable Materials.....
      //glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
      //glEnable(GL_COLOR_MATERIAL);
      glDisable(GL_COLOR_MATERIAL);

      //-----------------set up a light....
      /*
      GLfloat light_ambient[] = { 0.1f,  0.1f,  0.1f,  1.0f};
      GLfloat light_diffuse[] = { 0.8f,  0.8f,  0.8f,  1.0f};
      GLfloat light_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
      GLfloat light_position[] = {0.0f, 0.75f, 0.75f, 0.0f};
      */
      //-----------------Call the light....
      /*
      glLightfv(GL_LIGHT7, GL_AMBIENT,  light_ambient);
      glLightfv(GL_LIGHT7, GL_DIFFUSE,  light_diffuse);
      glLightfv(GL_LIGHT7, GL_SPECULAR,  light_specular);
      glLightfv(GL_LIGHT7, GL_POSITION,  light_position);

      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT7);
      */

      glPushMatrix();
      {
         glColor3f(0.76f, 0.80f, 0.95f);
         glLineWidth(3.0f);
         // Get to hand coord system to start drawing
         glMultMatrixf(base_glove_pos.getFloatPtr());

         // Draw PALM
         glPushMatrix();
         {
            glScalef(1.0f, 0.75f, 0.1f);
            drawSphere((1.5f/12.0f), 12, 6);

            //drawSolidCube((1.0f/12.0f));      // 1 in. cube
         }
         glPopMatrix();

         // Draw INDEX finger
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[GloveData::INDEX][GloveData::MPJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::INDEX][GloveData::PIJ]);
            glMultMatrixf(gd.xforms[GloveData::INDEX][GloveData::PIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::INDEX][GloveData::DIJ]);
            glMultMatrixf(gd.xforms[GloveData::INDEX][GloveData::DIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::INDEX][GloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw MIDDLE finger
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[GloveData::MIDDLE][GloveData::MPJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::MIDDLE][GloveData::PIJ]);
            glMultMatrixf(gd.xforms[GloveData::MIDDLE][GloveData::PIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::MIDDLE][GloveData::DIJ]);
            glMultMatrixf(gd.xforms[GloveData::MIDDLE][GloveData::DIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::MIDDLE][GloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw RING finger
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[GloveData::RING][GloveData::MPJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::RING][GloveData::PIJ]);
            glMultMatrixf(gd.xforms[GloveData::RING][GloveData::PIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::RING][GloveData::DIJ]);
            glMultMatrixf(gd.xforms[GloveData::RING][GloveData::DIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::RING][GloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw PINKY finger
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[GloveData::PINKY][GloveData::MPJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::PINKY][GloveData::PIJ]);
            glMultMatrixf(gd.xforms[GloveData::PINKY][GloveData::PIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::PINKY][GloveData::DIJ]);
            glMultMatrixf(gd.xforms[GloveData::PINKY][GloveData::DIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::PINKY][GloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw THUMB
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[GloveData::THUMB][GloveData::MPJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::THUMB][GloveData::PIJ]);
            glMultMatrixf(gd.xforms[GloveData::THUMB][GloveData::PIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::THUMB][GloveData::DIJ]);
            glMultMatrixf(gd.xforms[GloveData::THUMB][GloveData::DIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[GloveData::THUMB][GloveData::DIJ+1]);
         }
         glPopMatrix();
      }
      glPopMatrix();
   }
   //glPopAttrib();
}

} // end namespace


#if  defined(VJ_OS_Win32)
#  include <vrj/Draw/OGL/GlWinWin32.h>
#elif defined(VJ_OS_Darwin)
#  include <vrj/Draw/OGL/GlOSXWindow.h>
#else
#  include <vrj/Draw/OGL/GlxWindow.h>
#endif

namespace vrj
{
   
vrj::GlWindow* GlDrawManager::getGLWindow()
{
#if  defined(VJ_OS_Win32)
   return new vrj::GlWinWin32;
#elif defined(VJ_OS_Darwin)
   return new vrj::GlOSXWindow
#else
   return new vrj::GlxWindow;
#endif
}

} // end namespace

