#include <vjConfig.h>
#include <Kernel/GL/vjGlDrawManager.h>
#include <Threads/vjThread.h>
#include <Kernel/vjDisplayManager.h>
#include <Kernel/vjKernel.h>

#include <Kernel/vjDisplay.h>
#include <Kernel/vjSimDisplay.h>
#include <Kernel/vjSurfaceDisplay.h>

#include <Kernel/GL/vjGlApp.h>
#include <Input/vjGlove/vjGlove.h>
#include <Input/InputManager/vjGloveProxy.h>

vjGlDrawManager* vjGlDrawManager::_instance = NULL;


//: Set the app the draw should interact with.
void vjGlDrawManager::setApp(vjApp* _app)
{
   mApp = dynamic_cast<vjGlApp*>(_app);

   // We have a new app, so the contexts must be re-initialized
   // so... dirty them all.
   dirtyAllWindows();
   vjASSERT(mApp != NULL);
}

//: Return the app we are rednering
vjGlApp* vjGlDrawManager::getApp()
{ return mApp; }


//: Do initial configuration for the draw manager
// Doesn't do anything right now
void vjGlDrawManager::configInitial(vjConfigChunkDB*  chunkDB)
{
    // Setup any config data
}

//: Start the control loop
void vjGlDrawManager::start()
{
   // --- Setup Multi-Process stuff --- //
   // Create a new thread to handle the control
	vjThread* control_thread;

   vjThreadMemberFunctor<vjGlDrawManager>* memberFunctor =
      new vjThreadMemberFunctor<vjGlDrawManager>(this, &vjGlDrawManager::main, NULL);

	control_thread = new vjThread(memberFunctor, 0);

   vjDEBUG(vjDBG_DRAW_MGR,0) << "vjGlDrawManager::thread: " << control_thread << endl << vjDEBUG_FLUSH;
}


    // Enable a frame to be drawn
    // Trigger draw
void vjGlDrawManager::draw()
{
   drawTriggerSema.release();
}


//: Blocks until the end of the frame
//! POST:
//+	   The frame has been drawn
void vjGlDrawManager::sync()
{
   drawDoneSema.acquire();
}


//: This is the control loop for the manager
void vjGlDrawManager::main(void* nullParam)
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
      //**//mRuntimeConfigSema.acquire();
   }
}

void vjGlDrawManager::drawAllPipes()
{
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,3) << "vjGLDrawManager::drawAllPipes: Enter" << endl << flush << vjDEBUG_FLUSH;
   int pipeNum;

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


   vjDEBUG_END(vjDBG_DRAW_MGR,3) << "vjGLDrawManager::drawAllPipes: Exit" << endl << flush << vjDEBUG_FLUSH;
}

//: Initialize the drawing API (if not already running)
//! POST: Control thread is started
void vjGlDrawManager::initAPI()
{
   start();
}

//: Initialize the drawing state for the API based on
//+ the data in the display manager.
//
//! PRE: API is running (initAPI has been called)
//! POST: API is ready do draw    <br>
//+	 Process model is configured <br>
//+	 Multi-pipe data is set      <br>
//+	 Window list is correct      <br>
void vjGlDrawManager::initDrawing()
{
   vjDEBUG(vjDBG_DRAW_MGR,3) << "vjGlDrawManager::initDrawing: Entering." << endl << vjDEBUG_FLUSH;


}
	

//: Callback when display is added to display manager
//! PRE: Must be in kernel controlling thread
//! NOTE: This function can only be called by the display manager
//+      functioning in the kernel thread to signal a new display added
//+      This guarantees that we are not rendering currently.
//+      We will most likely be waiting for a render trigger.
void vjGlDrawManager::addDisplay(vjDisplay* disp)
{
//**//   vjGuard<vjSemaphore> runtime_guard(mRuntimeConfigSema);     // Have to hold to configure

   vjDEBUG(vjDBG_DRAW_MGR,0) << "vjGlDrawManager:addDisplay\n" << *disp << endl << vjDEBUG_FLUSH;

   //	-- Create a window for new display
   //	-- Store the window in the wins vector
   vjGlWindow* new_win;
   new_win = vjKernel::instance()->getSysFactory()->getGLWindow();   // Create gl window
   new_win->config(disp);                                            // Configure it
   wins.push_back(new_win);                                          // Add to our local window list

   // -- Create any needed Pipes & Start them
   int pipe_num = new_win->getDisplay()->getPipe();    // Find pipe to add it too
   vjASSERT(pipe_num >= 0);                            // ASSERT: pipeNum := [0...n]

   if(pipes.size() < (pipe_num+1))           // ASSERT: Max index of pipes is < our pipe
   {                                         // +1 because if pipeNum = 0, I still need size() == 1
      while(pipes.size() < (pipe_num+1))     // While we need more pipes
      {
         vjGlPipe* new_pipe = new vjGlPipe(pipe_num, this);  // Create a new pipe to use
         pipes.push_back(new_pipe);                          // Add the pipe
         new_pipe->start();                                  // Start the pipe running
                                                            // NOTE: Run pipe even if now windows.  Then it waits for windows.
      }
   }

   // -- Add window to the correct pipe
   vjGlPipe* pipe;                           // The pipe to assign it to
   pipe = pipes[pipe_num];                    // ASSERT: pipeNum is in the valid range
   pipe->addWindow(new_win);             // Window has been added

   // Dump the state
   vjDEBUG(vjDBG_ALL, 0) << (*this) << vjDEBUG_FLUSH;
}


//: Callback when display is removed to display manager
void vjGlDrawManager::removeDisplay(vjDisplay* disp)
{
   vjGlPipe*   pipe(NULL); // The pipe to remove it from
   vjGlWindow* win(NULL);  // Window to remove

   for(int i=0;i<wins.size();i++)
   {
      if(wins[i]->getDisplay() == disp)      // FOUND it
      {
         win = wins[i];
         pipe = pipes[win->getDisplay()->getPipe()];
      }
   }

   // Remove the window from the pipe
   if(win != NULL)
   {
      vjASSERT(pipe != NULL);

      pipe->removeWindow(win);
   }

}


/// Shutdown the drawing API
void vjGlDrawManager::closeAPI()
{
   vjDEBUG(vjDBG_DRAW_MGR,0) << "vjGlDrawManager::closeAPI: Closing.\n" << vjDEBUG_FLUSH;
    // Stop all pipes
   ;
    // Delete all pipes

    // Close and delete all glWindows
}

/////// CHUNK HANDLERS ////////////////////
//: Add the chunk to the draw manager config
//! PRE: configCanHandle(chunk) == true
//! POST: The chunks have reconfigured the system
bool vjGlDrawManager::configAdd(vjConfigChunk* chunk)
{
   return false;
}

//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//!RETURNS: success
bool vjGlDrawManager::configRemove(vjConfigChunk* chunk)
{
   return false;
}

//: Can the handler handle the given chunk?
//! RETURNS: false - We can't handle anything
bool vjGlDrawManager::configCanHandle(vjConfigChunk* chunk)
{
   return false;
}


//: Set the dirty bits off all the gl windows
// Dirty all the window contexts
void vjGlDrawManager::dirtyAllWindows()
{
    // Create Pipes & Add all windows to the correct pipe
   for(int winId=0;winId<wins.size();winId++)   // For each window we created
   {
      wins[winId]->setDirtyContext(true);
   }
}


// Draw any objects that we need to display in the scene
// from the system.  (i.e. Gloves, etc)
// XXX: Performance Critical problems here
void vjGlDrawManager::drawObjects()
{
   glPushAttrib( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT );
   {
      glDisable(GL_LIGHTING);
      glDisable(GL_BLEND);
      vjInputManager*  input_mgr = vjKernel::instance()->getInputManager();

      // Draw all glove Proxies that have drawing flag set
      vjGloveProxy* cur_glove_proxy;
      for(int glv=0;glv<input_mgr->getNumGloveProxies();glv++)    // For each glove in system
      {
         cur_glove_proxy = input_mgr->GetGloveProxy(glv);         // Get the glove proxy
         if(cur_glove_proxy->isVisible())                         // If flag set
            drawGlove(cur_glove_proxy);                           // draw it
      }

      // Draw any other object that need to be seen
   }
   glPopAttrib();
}


// Draw the projections
//!POST: Draws the projections
//+      If withApex, then it draws the frustums with different colors
//+      If !withApex, then just draws the surfaces in all white
void vjGlDrawManager::drawProjections(vjSimDisplay* sim)
{
   const float ALPHA_VALUE(0.25f);
   /*
   static float ALPHA_VALUE(0.0f);
   ALPHA_VALUE += 0.001f;
   if(ALPHA_VALUE > 1.0f)
      ALPHA_VALUE = 0.0f;

   vjDEBUG(vjDBG_ALL,0) << "ALPHA_VALUE: " << ALPHA_VALUE << endl << vjDEBUG_FLUSH;
   */

   std::vector<vjDisplay*> disps = displayManager->getAllDisplays();

   vjVec3 apex, ur, lr, ul, ll;
   vjProjection* proj(NULL);

   for(int i=0;i<disps.size();i++)
   {
      if(disps[i]->isSurface())
      {
         //if(disps[i]->isSurface())
         //{
            vjSurfaceDisplay* surf_disp = dynamic_cast<vjSurfaceDisplay*>(disps[i]);
            vjASSERT(surf_disp != NULL);
            proj = surf_disp->getLeftProj();
         //}
         //else if (disps[i]->isSimulator())
         //{
         //   vjSimDisplay* sim_disp = dynamic_cast<vjSimDisplay*>(disps[i]);
         //   vjASSERT(sim_disp != NULL);
         //   proj = sim_disp->getCameraProj();
         //}

         // Create color values that are unique
         // Basically count in binary (skipping 0), and use the first 3 digits.  That will give six colors
         int red_on = (i & 0x1); int green_on = ((i >> 1) & 0x1); int blue_on = ((i >> 2) & 0x1);

         float red(0.0f), green(0.0f), blue(0.0f);
         if(red_on > 0) red = 1.0f;
         if(green_on > 0) green = 1.0f;
         if(blue_on > 0) blue = 1.0f;

         if((!red_on) && (!blue_on) && (!green_on))      // Case of 0's (black is bad)
            red = blue = green = 0.75f;

         // Draw the thingy
         proj->getFrustumApexAndCorners(apex, ur, lr, ul, ll);
         glPushMatrix();
            if(sim->shouldDrawProjections())
            {
               glColor4f(red, green, blue, ALPHA_VALUE);
               drawLine(apex, ur); drawLine(apex, lr); drawLine(apex, ul); drawLine(apex, ll);
            }
            else
               glColor4f(sim->getSurfaceColor()[0], sim->getSurfaceColor()[1],
                         sim->getSurfaceColor()[2], ALPHA_VALUE);
            drawLine(ur, lr); drawLine(lr, ll); drawLine(ll, ul); drawLine(ul, ur);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_TRIANGLES);
               glVertex3fv(ll.vec); glVertex3fv(lr.vec); glVertex3fv(ur.vec);
               glVertex3fv(ur.vec); glVertex3fv(ul.vec); glVertex3fv(ll.vec);
            glEnd();
            glDisable(GL_BLEND);
         glPopMatrix();
      }
   }
}

//: Draw a simulator using OpenGL commands
//! NOTE: This is called internally by the library
void vjGlDrawManager::drawSimulator(vjSimDisplay* sim)
{
   const float head_radius(0.60);      // 7.2 inches
   const float eye_vertical(0.22);
   const float eye_horizontal(0.7);
   const float interoccular(0.27);
   const float eye_radius(0.08f);

   glPushAttrib( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT );
   {
   	//-----------------set up materials....
   	float mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
   	float mat_shininess[] = {50.0};
   	float mat_diffuse[] = {.7, .7, .7, 1.0};
   	float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
   	//-----------------Call Materials.....
   	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   	//----------------Enable Materials.....
   	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
   	glEnable(GL_COLOR_MATERIAL);
	
     // Draw the user's head
      glPushMatrix();
	      glMultMatrixf(sim->getHeadPos().getFloatPtr());
	
         // Draw Axis
         /*
         glDisable(GL_LIGHTING);
         glPushMatrix();
            vjVec3 x_axis(10.0f,0.0f,0.0f); vjVec3 y_axis(0.0f, 10.0f, 0.0f); vjVec3 z_axis(0.0f, 0.0f, 10.0f); vjVec3 origin(0.0f, 0.0f, 0.0f);
            glBegin(GL_LINES);
               glColor3f(1.0f, 0.0f, 0.0f); glVertex3fv(origin.vec); glVertex3fv(x_axis.vec);
               glColor3f(0.0f, 1.0f, 0.0f); glVertex3fv(origin.vec); glVertex3fv(y_axis.vec);
               glColor3f(0.0f, 0.0f, 1.0f); glVertex3fv(origin.vec); glVertex3fv(z_axis.vec);
            glEnd();
         glPopMatrix();
         */
         glEnable(GL_LIGHTING);

         //glEnable(GL_BLEND);
         //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glColor4f(0.5, 0.75, 0.90, 0.67f);
	      drawSphere(head_radius, 10, 10);             // Head
         glDisable(GL_BLEND);

	      glPushMatrix();
	         glColor3f(0.2f, 0.2f, 0.2f);
	         glTranslatef(0.0f, eye_vertical, -eye_horizontal);
	         glPushMatrix();                     // Right eye
		         glTranslatef((interoccular/2.0f), 0.0f, 0.0f);
		         drawSphere(eye_radius, 5, 5);
	         glPopMatrix();
	         glPushMatrix();                     // Left eye
		         glTranslatef(-(interoccular/2.0f), 0.0f, 0.0f);
		         drawSphere(eye_radius, 5, 5);
	         glPopMatrix();
	      glPopMatrix();
      glPopMatrix();

	   // Draw the wand
      glPushMatrix();
	      glMultMatrixf(sim->getWandPos().getFloatPtr());
	      glColor3f(0.0f, 1.0f, 0.0f);
	      drawCone(0.2, 0.6f, 6, 1);
      glPopMatrix();

       // Draw a The display surfaces
      glDisable(GL_LIGHTING);
      glPushMatrix();
         drawProjections(sim);
      glPopMatrix();
   }
   glPopAttrib();
}

    /// dumps the object's internal state
void vjGlDrawManager::outStream(ostream& out)
{
    out     << "========== vjGlDrawManager: " << (void*)this << " =========" << endl
	         << "\tapp:" << (void*)mApp << endl
            << "\tWins:" << wins.size();

    for(std::vector<vjGlWindow*>::iterator i = wins.begin(); i != wins.end(); i++)
    {
	   out << "\n\t\tvjGlWindow:\n" << *(*i) << endl;
    }
    out << "=======================================" << endl;
}

void vjGlDrawManager::initQuadObj()
{
   if (mQuadObj == NULL)
      mQuadObj = gluNewQuadric();
}

void vjGlDrawManager::drawLine(vjVec3& start, vjVec3& end)
{
   glBegin(GL_LINES);
      glVertex3fv(start.vec);
      glVertex3fv(end.vec);
   glEnd();
}

void vjGlDrawManager::drawSphere(float radius, int slices, int stacks)
{
  initQuadObj();
  gluQuadricDrawStyle(mQuadObj, GLU_FILL);
  gluQuadricNormals(mQuadObj, GLU_SMOOTH);
  gluSphere(mQuadObj, radius, slices, stacks);
}


void vjGlDrawManager::drawCone(float base, float height, int slices, int stacks)
{
  initQuadObj();
  gluQuadricDrawStyle(mQuadObj, GLU_FILL);
  gluQuadricNormals(mQuadObj, GLU_SMOOTH);
  gluCylinder(mQuadObj, base, 0.0, height, slices, stacks);
}



void vjGlDrawManager::drawBox(float size, GLenum type)
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


void vjGlDrawManager::drawWireCube(float size)
{
  drawBox(size, GL_LINE_LOOP);
}

void vjGlDrawManager::drawSolidCube(float size)
{
  drawBox(size, GL_QUADS);
}

// Draw a glove outline for the user
// This may be ugly for now.
// For each finger, step down it's xforms drawing the finger
// links as you go.
void vjGlDrawManager::drawGlove(vjGloveProxy* gloveProxy)
{
   vjMatrix    base_glove_pos = gloveProxy->getPos();    // Get the location of the base coord system
   vjGloveData gd = gloveProxy->getData();               // Get the glove data
   vjVec3      origin(0.0f,0.0f,0.0f);

   //glPushAttrib( GL_ENABLE_BIT | GL_LIGHTING_BIT );
   {
      //-----------------set up materials....
      float mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
      float mat_shininess[] = {50.0};
      float mat_diffuse[] = {.7, .7, .7, 1.0};
      float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
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
            glMultMatrixf(gd.xforms[vjGloveData::INDEX][vjGloveData::MPJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::INDEX][vjGloveData::PIJ]);
            glMultMatrixf(gd.xforms[vjGloveData::INDEX][vjGloveData::PIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::INDEX][vjGloveData::DIJ]);
            glMultMatrixf(gd.xforms[vjGloveData::INDEX][vjGloveData::DIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::INDEX][vjGloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw MIDDLE finger
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[vjGloveData::MIDDLE][vjGloveData::MPJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::MIDDLE][vjGloveData::PIJ]);
            glMultMatrixf(gd.xforms[vjGloveData::MIDDLE][vjGloveData::PIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::MIDDLE][vjGloveData::DIJ]);
            glMultMatrixf(gd.xforms[vjGloveData::MIDDLE][vjGloveData::DIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::MIDDLE][vjGloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw RING finger
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[vjGloveData::RING][vjGloveData::MPJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::RING][vjGloveData::PIJ]);
            glMultMatrixf(gd.xforms[vjGloveData::RING][vjGloveData::PIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::RING][vjGloveData::DIJ]);
            glMultMatrixf(gd.xforms[vjGloveData::RING][vjGloveData::DIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::RING][vjGloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw PINKY finger
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[vjGloveData::PINKY][vjGloveData::MPJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::PINKY][vjGloveData::PIJ]);
            glMultMatrixf(gd.xforms[vjGloveData::PINKY][vjGloveData::PIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::PINKY][vjGloveData::DIJ]);
            glMultMatrixf(gd.xforms[vjGloveData::PINKY][vjGloveData::DIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::PINKY][vjGloveData::DIJ+1]);
         }
         glPopMatrix();

         // Draw THUMB
         glPushMatrix();
         {
            glMultMatrixf(gd.xforms[vjGloveData::THUMB][vjGloveData::MPJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::THUMB][vjGloveData::PIJ]);
            glMultMatrixf(gd.xforms[vjGloveData::THUMB][vjGloveData::PIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::THUMB][vjGloveData::DIJ]);
            glMultMatrixf(gd.xforms[vjGloveData::THUMB][vjGloveData::DIJ].getFloatPtr());
            drawSphere((0.1f/12.0f), 4, 4);
            drawLine(origin, gd.dims[vjGloveData::THUMB][vjGloveData::DIJ+1]);
         }
         glPopMatrix();
      }
      glPopMatrix();
   }
   //glPopAttrib();
}


