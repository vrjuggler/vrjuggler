/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <vrj/vrjConfig.h>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>

#include <cluster/ClusterManager.h>

#include <vrj/Draw/OGL/GlDrawManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Kernel/Kernel.h>

#include <vrj/Display/Display.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Display/SurfaceViewport.h>

#include <vrj/Draw/OGL/GlApp.h>

#include <vrj/Draw/OGL/GlPipe.h>
#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Draw/OGL/GlDrawWandFunctors.h>

#include <gmtl/Vec.h>
#include <gmtl/Output.h>

//#include <gadget/Type/Glove.h>
//#include <gadget/Type/GloveProxy.h>


namespace vrj
{

//vjGlDrawManager* GlDrawManager::_instance = NULL;
vprSingletonImp(GlDrawManager);

GlDrawManager::GlDrawManager() : drawTriggerSema(0), drawDoneSema(0), mRuntimeConfigSema(0)
{
   mQuadObj = NULL;

   //setDrawWandFunctor(new GlDrawConeWandFunctor());
   setDrawWandFunctor(new GlDrawRightAngleWandFunctor());
}


/** Sets the app the draw should interact with. */
void GlDrawManager::setApp(App* _app)
{
   mApp = dynamic_cast<GlApp*>(_app);

   // We have a new app, so the contexts must be re-initialized
   // so... dirty them all.
   dirtyAllWindows();
   vprASSERT(mApp != NULL);
}

/** Returns the app we are rednering. */
GlApp* GlDrawManager::getApp()
{ return mApp; }


/**
 * Do initial configuration for the draw manager.
 * Doesn't do anything right now.
 */
/*
void GlDrawManager::configInitial(jccl::ConfigChunkDB*  chunkDB)
{
    // Setup any config data
}
*/

/** Starts the control loop. */
void GlDrawManager::start()
{
   // --- Setup Multi-Process stuff --- //
   // Create a new thread to handle the control
   vpr::Thread* control_thread;

   // XXX: Memory leak.
   vpr::ThreadMemberFunctor<GlDrawManager>* memberFunctor =
      new vpr::ThreadMemberFunctor<GlDrawManager>(this, &GlDrawManager::main, NULL);

   // XXX: Memory leak.
   // XXX: No way to stop this thread later.
   control_thread = new vpr::Thread(memberFunctor);

   vprDEBUG(vrjDBG_DRAW_MGR,1) << "vjGlDrawManager started. thread: "
                             << control_thread << std::endl << vprDEBUG_FLUSH;
}


    // Enable a frame to be drawn
    // Trigger draw
void GlDrawManager::draw()
{
   drawTriggerSema.release();
}


/**
 * Blocks until the end of the frame.
 * @post The frame has been drawn.
 */
void GlDrawManager::sync()
{
   drawDoneSema.acquire();
}


/** This is the control loop for the manager. */
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
   vprDEBUG_BEGIN(vrjDBG_DRAW_MGR,vprDBG_HVERB_LVL)
      << "vjGLDrawManager::drawAllPipes: " << std::endl << std::flush
      << vprDEBUG_FLUSH;
   unsigned int pipeNum;

   // RENDER
      // Start rendering all the pipes
   for(pipeNum=0; pipeNum<pipes.size(); pipeNum++)
      pipes[pipeNum]->triggerRender();

      // Wait for rendering to finish on all the pipes
   for(pipeNum=0; pipeNum<pipes.size(); pipeNum++)
      pipes[pipeNum]->completeRender();

   // Barrier for Cluster
   //vprDEBUG(vprDBG_ALL,1) <<  "BARRIER: Going to sleep for: " << num << std::endl << vprDEBUG_FLUSH;
   cluster::ClusterManager::instance()->createBarrier();
   // vprDEBUG(vprDBG_ALL,1) <<  "BARRIER: IS DONE" << std::endl << vprDEBUG_FLUSH;


   // SWAP
      // Start swapping all the pipes
   for(pipeNum=0; pipeNum<pipes.size(); pipeNum++)
      pipes[pipeNum]->triggerSwap();

      // Wait for swapping to finish on all the pipes
   for(pipeNum=0; pipeNum<pipes.size(); pipeNum++)
      pipes[pipeNum]->completeSwap();


   vprDEBUG_END(vrjDBG_DRAW_MGR,vprDBG_HVERB_LVL)
      << "vjGLDrawManager::drawAllPipes: Done" << std::endl << std::flush
      << vprDEBUG_FLUSH;
}

/**
 * Initializes the drawing API (if not already running).
 * @post Control thread is started.
 */
void GlDrawManager::initAPI()
{
   start();
}


/**
 * Callback when display is added to display manager.
 * @pre Must be in kernel controlling thread.
 * @note This function can only be called by the display manager
 *       functioning on behalf of a thread the holds the kernel
 *       reconfiguration lock.
 *       This guarantees that we are not rendering currently.
 *       We will most likely be waiting for a render trigger.
 */
void GlDrawManager::addDisplay(Display* disp)
{
   vprASSERT(disp != NULL);    // Can't add a null display

   vprDEBUG(vrjDBG_DRAW_MGR,3) << "vrj::GlDrawManager:addDisplay: " << disp
                             << std::endl << vprDEBUG_FLUSH;

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
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "Reconfigured the glDrawManager.\n" << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL) << (*this) << vprDEBUG_FLUSH;
}


/**
 * Callback when display is removed to display manager.
 * @pre disp must be a valid display that we have.
 * @post window for disp is removed from the draw manager and child pipes.
 */
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
      vprDEBUG(vprDBG_ERROR, 0) << clrOutNORM(clrRED,"ERROR:") << "vrj::GlDrawManager::removeDisplay: Attempted to remove a display that was not found.\n" << vprDEBUG_FLUSH;
      vprASSERT(false);
   }

}


/** Shutdown the drawing API */
void GlDrawManager::closeAPI()
{
   vprDEBUG(vrjDBG_DRAW_MGR,0) << "vrj::GlDrawManager::closeAPI: NOT IMPLEMENTED.\n" << vprDEBUG_FLUSH;
    // Stop all pipes
   ;
    // Delete all pipes

    // Close and delete all glWindows
}

/////// CHUNK HANDLERS ////////////////////
/**
 * Adds the chunk to the draw manager config.
 * @pre configCanHandle(chunk) == true
 * @post The chunks have reconfigured the system
 */
bool GlDrawManager::configAdd(jccl::ConfigChunkPtr chunk)
{
   return false;
}

/**
 * Removes the chunk from the current configuration.
 * @pre configCanHandle(chunk) == true
 * @return success
 */
bool GlDrawManager::configRemove(jccl::ConfigChunkPtr chunk)
{
   return false;
}

/**
 * Can the handler handle the given chunk?
 * @return false - We can't handle anything
 */
bool GlDrawManager::configCanHandle(jccl::ConfigChunkPtr chunk)
{
   return false;
}


/**
 * Sets the dirty bits off all the gl windows.
 * Dirty all the window contexts.
 */
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


/**
 * Draw the projections.
 *
 * @post Draws the projections.
 *       If withApex, then it draws the frustums with different colors.
 *       If !withApex, then just draws the surfaces in all white.
 */
void GlDrawManager::drawProjections(bool drawFrustum, gmtl::Vec3f surfColor, const float scaleFactor)
{
   const float ALPHA_VALUE(0.25f);

   mDisplayManager->updateProjections(scaleFactor);                     // Update all projections for drawing

   std::vector<Display*> disps = mDisplayManager->getAllDisplays();

   gmtl::Vec3f apex, ur, lr, ul, ll;
   Projection* proj; proj = NULL;

   for (unsigned int i=0;i<disps.size();i++)
   {
      for (unsigned int v=0;v<disps[i]->getNumViewports();v++)
      {
         Viewport* view_port = disps[i]->getViewport(v);

         if (view_port->isSurface())
         {
            for(unsigned proj_num=0;proj_num<2;++proj_num)
            {
               // Get a pointer to the surface
               SurfaceViewport* surf_vp = dynamic_cast<SurfaceViewport*>(view_port);
               vprASSERT(surf_vp != NULL);
               proj = NULL;
               if(0 == proj_num)
                  proj = surf_vp->getLeftProj();
               else
                  proj = surf_vp->getRightProj();

               // Create color values that are unique
               // Basically count in binary (skipping 0), and use the first 3 digits.  That will give six colors
               int red_on = (i & 0x1); int green_on = ((i >> 1) & 0x1); int blue_on = ((i >> 2) & 0x1);

               float red(0.0f), green(0.0f), blue(0.0f);
               if (red_on > 0) red = 1.0f;
               if (green_on > 0) green = 1.0f;
               if (blue_on > 0) blue = 1.0f;

               if ((!red_on) && (!blue_on) && (!green_on))      // Case of 0's (black is bad)
                  red = blue = green = 0.75f;

               gmtl::Vec3f surf_color;
               if (drawFrustum)
               {
                  surf_color = gmtl::Vec3f(red,blue,green);
               }
               else
               {
                  surf_color = surfColor;
               }
               if(1 == proj_num) // Right eye
               {
                  surf_color = gmtl::Vec3f(1.0f, 1.0f, 1.0f) - surf_color;    // Invert it
               }

               // Compute scaled colors for the corners
               // ll is going to be lighter and upper right is going to be darker
               const float ll_scale(0.10f); const float ul_scale(0.55); const float ur_scale(1.0f);
               gmtl::Vec4f ll_clr(ll_scale*surf_color[0],ll_scale*surf_color[1],ll_scale*surf_color[2],ALPHA_VALUE);
               gmtl::Vec4f ul_clr(ul_scale*surf_color[0],ul_scale*surf_color[1],ul_scale*surf_color[2],ALPHA_VALUE);
               gmtl::Vec4f lr_clr(ul_scale*surf_color[0],ul_scale*surf_color[1],ul_scale*surf_color[2],ALPHA_VALUE);
               gmtl::Vec4f ur_clr(ur_scale*surf_color[0],ur_scale*surf_color[1],ur_scale*surf_color[2],ALPHA_VALUE);

               // Draw the thingy
               proj->getFrustumApexAndCorners(apex, ur, lr, ul, ll);
               vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL) << "apex: " << apex
                                                           << std::endl << vprDEBUG_FLUSH;

               glColor4fv(&(ur_clr[0]));
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
                  glColor4fv(ll_clr.mData);  glVertex3fv(ll.mData);
                  glColor4fv(lr_clr.mData);  glVertex3fv(lr.mData);
                  glColor4fv(ur_clr.mData);  glVertex3fv(ur.mData);

                  glColor4fv(ur_clr.mData); glVertex3fv(ur.mData);
                  glColor4fv(ul_clr.mData); glVertex3fv(ul.mData);
                  glColor4fv(ll_clr.mData); glVertex3fv(ll.mData);
                  glEnd();
                  glDisable(GL_BLEND);
               glPopMatrix();
            }
         }  // if surface
      }  // for viewports
   }  // for disps
}

/**
 * Draws a simulator using OpenGL commands.
 * @note This is called internally by the library.
 */
void GlDrawManager::drawSimulator(SimViewport* sim_vp, const float scaleFactor)
{
   // All units are in meters
   // Note: All the wand and head data in the sim viewport class
   // has already had the scale factor applied to it
   const float head_height(0.254f*scaleFactor);      // 10 inches
   const float head_width_scale(0.7f);
   const float head_depth_scale(0.8f);
   //const float eye_vertical(0.067f*scaleFactor);
   //const float eye_horizontal(0.21f*scaleFactor);
   const float interocular( sim_vp->getUser()->getInterocularDistance()*scaleFactor);
   const float eye_radius(0.0254f*0.5f*scaleFactor);      // 0.5 in

   glPushAttrib( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT);
   {
      // Test to see wethere there is lighting active
      GLboolean lighting_on, light0_on;
      glGetBooleanv(GL_LIGHTING, &lighting_on);
      glGetBooleanv(GL_LIGHT0, &light0_on);

      bool use_lighting_in_sim = (lighting_on == GL_TRUE);

      //vprDEBUG(vprDBG_ALL,0) << "lighting on: " << ((lighting_on == GL_TRUE)?"Y":"N")
      //                       << "  light0_on:" << ((light0_on == GL_TRUE)?"Y":"N") << std::endl << vprDEBUG_FLUSH;

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
      if(lighting_on)
         glDisable(GL_LIGHTING);

      glPushMatrix();
         gmtl::Vec3f x_axis(scaleFactor,0.0f,0.0f); gmtl::Vec3f y_axis(0.0f, scaleFactor, 0.0f);
         gmtl::Vec3f z_axis(0.0f, 0.0f, scaleFactor); gmtl::Vec3f origin(0.0f, 0.0f, 0.0f);
         glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 0.0f); glVertex3fv(origin.mData); glVertex3fv(x_axis.mData);
            glColor3f(0.0f, 1.0f, 0.0f); glVertex3fv(origin.mData); glVertex3fv(y_axis.mData);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex3fv(origin.mData); glVertex3fv(z_axis.mData);
         glEnd();
      glPopMatrix();
      //*/

      if(use_lighting_in_sim)
      {
         glEnable(GL_LIGHTING);
         glEnable(GL_NORMALIZE);
      }
      // Draw the user's head
      glPushMatrix();
         glMultMatrixf(sim_vp->getHeadPos().mData);

         glPushMatrix();
            // Head pos is the center of the eyes, so we need to move the head offset a bit
            // to get it positioned correctly.  We will do this by measuring the distance from center
            // of head to forehead and then translate by the negative of that
            gmtl::Vec3f forehead_offset(0.0f, head_height*0.17f, -(head_depth_scale*head_height)*0.45f);
            glTranslatef(-forehead_offset[0], -forehead_offset[1], -forehead_offset[2]);
            glScalef(head_width_scale, 1.0f, head_depth_scale);      // Scale to get an ellipsoid head
            //glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0.5f, 0.75f, 0.90f, 0.67f);
            drawSphere(head_height/2.0f, 10, 10);             // Head
            //glDisable(GL_BLEND);
         glPopMatrix();

         // --- Draw the eyes --- //
         glPushMatrix();
            glColor3f(0.8f, 0.4f, 0.2f);
            //glTranslatef(0.0f, eye_vertical, -eye_horizontal);
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
         glMultMatrixf(sim_vp->getWandPos().mData);
         glScalef(scaleFactor,scaleFactor,scaleFactor);
         glEnable(GL_NORMALIZE);
         mDrawWandFunctor->draw();
      glPopMatrix();

       // Draw a The display surfaces
      if(use_lighting_in_sim)
         glDisable(GL_LIGHTING);

      glPushMatrix();
         drawProjections(sim_vp->shouldDrawProjections(), sim_vp->getSurfaceColor(), scaleFactor);
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
            << clrOutNORM(clrCYAN,"\tapp: ") << (void*)mApp << std::endl
            << clrOutNORM(clrCYAN,"\tWindow count: ") << mWins.size()
            << std::endl << std::flush;

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

void GlDrawManager::drawLine(gmtl::Vec3f& start, gmtl::Vec3f& end)
{
   glBegin(GL_LINES);
      glVertex3fv(start.mData);
      glVertex3fv(end.mData);
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

/*
// Draw a glove outline for the user
// This may be ugly for now.
// For each finger, step down it's xforms drawing the finger
// links as you go.
void GlDrawManager::drawGlove(gadget::GloveProxy* gloveProxy)
{
   gmtl::Matrix44f    base_glove_pos = gloveProxy->getPos();    // Get the location of the base coord system
   gadget::GloveData gd = gloveProxy->getData();               // Get the glove data
   gmtl::Vec3f      origin(0.0f,0.0f,0.0f);

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
      //GLfloat light_ambient[] = { 0.1f,  0.1f,  0.1f,  1.0f};
      //GLfloat light_diffuse[] = { 0.8f,  0.8f,  0.8f,  1.0f};
      //GLfloat light_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
      //GLfloat light_position[] = {0.0f, 0.75f, 0.75f, 0.0f};
      //-----------------Call the light....
      //glLightfv(GL_LIGHT7, GL_AMBIENT,  light_ambient);
      //glLightfv(GL_LIGHT7, GL_DIFFUSE,  light_diffuse);
      //glLightfv(GL_LIGHT7, GL_SPECULAR,  light_specular);
      //glLightfv(GL_LIGHT7, GL_POSITION,  light_position);

      //glEnable(GL_LIGHTING);
      //glEnable(GL_LIGHT7);

      glPushMatrix();
      {
         glColor3f(0.76f, 0.80f, 0.95f);
         glLineWidth(3.0f);
         // Get to hand coord system to start drawing
         glMultMatrixf(base_glove_pos.mData);

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
            glMultMatrixf(gd.xforms[gadget::GloveData::INDEX][gadget::GloveData::MPJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::INDEX][gadget::GloveData::PIJ]);
            glMultMatrixf(gd.xforms[gadget::GloveData::INDEX][gadget::GloveData::PIJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::INDEX][gadget::GloveData::DIJ]);
            glMultMatrixf(gd.xforms[gadget::GloveData::INDEX][gadget::GloveData::DIJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::INDEX][gadget::GloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw MIDDLE finger
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[gadget::GloveData::MIDDLE][gadget::GloveData::MPJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::MIDDLE][gadget::GloveData::PIJ]);
            glMultMatrixf(gd.xforms[gadget::GloveData::MIDDLE][gadget::GloveData::PIJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::MIDDLE][gadget::GloveData::DIJ]);
            glMultMatrixf(gd.xforms[gadget::GloveData::MIDDLE][gadget::GloveData::DIJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::MIDDLE][gadget::GloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw RING finger
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[gadget::GloveData::RING][gadget::GloveData::MPJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::RING][gadget::GloveData::PIJ]);
            glMultMatrixf(gd.xforms[gadget::GloveData::RING][gadget::GloveData::PIJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::RING][gadget::GloveData::DIJ]);
            glMultMatrixf(gd.xforms[gadget::GloveData::RING][gadget::GloveData::DIJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::RING][gadget::GloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw PINKY finger
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[gadget::GloveData::PINKY][gadget::GloveData::MPJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::PINKY][gadget::GloveData::PIJ]);
            glMultMatrixf(gd.xforms[gadget::GloveData::PINKY][gadget::GloveData::PIJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::PINKY][gadget::GloveData::DIJ]);
            glMultMatrixf(gd.xforms[gadget::GloveData::PINKY][gadget::GloveData::DIJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::PINKY][gadget::GloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw THUMB
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[gadget::GloveData::THUMB][gadget::GloveData::MPJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::THUMB][gadget::GloveData::PIJ]);
            glMultMatrixf(gd.xforms[gadget::GloveData::THUMB][gadget::GloveData::PIJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::THUMB][gadget::GloveData::DIJ]);
            glMultMatrixf(gd.xforms[gadget::GloveData::THUMB][gadget::GloveData::DIJ].mData);
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[gadget::GloveData::THUMB][gadget::GloveData::DIJ+1]);
         }
         glPopMatrix();
      }
      glPopMatrix();
   }
   //glPopAttrib();
}
*/

} // end namespace


#if  defined(VPR_OS_Win32)
#  include <vrj/Draw/OGL/GlWindowWin32.h>
#elif defined(VPR_OS_Darwin)
#  include <vrj/Draw/OGL/GlWindowOSX.h>
#else
#  include <vrj/Draw/OGL/GlWindowXWin.h>
#endif

namespace vrj
{

vrj::GlWindow* GlDrawManager::getGLWindow()
{
#if  defined(VPR_OS_Win32)
   return new vrj::GlWindowWin32;
#elif defined(VPR_OS_Darwin)
   return new vrj::GlWindowOSX;
#else
   return new vrj::GlWindowXWin;
#endif
}

} // end namespace

