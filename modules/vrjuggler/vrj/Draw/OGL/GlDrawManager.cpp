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
{ app = dynamic_cast<vjGlApp*>(_app);}

//: Return the app we are rednering
vjGlApp* vjGlDrawManager::getApp()
{ return app; }

    /**
     * Function to config API specific stuff.
     * Takes a chunkDB and extracts API specific stuff
     */
void vjGlDrawManager::config(vjConfigChunkDB*  chunkDB)
{
    // Setup any config data
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
      // Wait for trigger
      drawTriggerSema.acquire();

      // THEN --- Do Rendering --- //
      drawAllPipes();

      // -- Done rendering --- //
      drawDoneSema.release();
   }
}

void vjGlDrawManager::drawAllPipes()
{
   vjDEBUG_BEGIN(3) << "vjGLDrawManager::drawAllPipes: Enter" << endl << flush << vjDEBUG_FLUSH;
   int pipeNum;

      // Start rendering all the pipes
   for(pipeNum=0; pipeNum<pipes.size(); pipeNum++)
      pipes[pipeNum]->triggerRender();

      // Wait for rendering to finish on all the pipes
   for(pipeNum=0; pipeNum<pipes.size(); pipeNum++)
      pipes[pipeNum]->completeRender();

   vjDEBUG_END(3) << "vjGLDrawManager::drawAllPipes: Exit" << endl << flush << vjDEBUG_FLUSH;
}

    /// Initialize the drawing API (if not already running)
void vjGlDrawManager::initAPI()
{
    ; /* Do nothing */
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
   vjDEBUG(3) << "vjGlDrawManager::initDrawing: Entering." << endl << vjDEBUG_FLUSH;

   //  For each display:
   //	-- Create a window for it
   //	-- Store the window in the wins vector
   std::vector<vjDisplay*> displays = displayManager->getDisplays();
   for (std::vector<vjDisplay*>::iterator dispIter = displays.begin();
       dispIter != displays.end(); dispIter++)
   {
      vjGlWindow* newWin;
      newWin = vjKernel::instance()->getSysFactory()->getGLWindow();
      newWin->config(*dispIter);
      wins.push_back(newWin);
   }

   // Create Pipes & Add all windows to the correct pipe
   for(int winId=0;winId<wins.size();winId++)   // For each window we created
   {
      int pipeNum = wins[winId]->getDisplay()->getPipe();  // Find pipe to add it too
                                                        // ASSERT: pipeNum := [0...n]

      if(pipes.size() < (pipeNum+1))            // ASSERT: Max index of pipes is < our pipe
      {
                                                // XXX: This is not really a good way to do it
         while(pipes.size() < (pipeNum+1))      // While we are out of range
         {
            vjGlPipe* newPipe = new vjGlPipe(pipeNum, this);   // Create a new pipe to use
            pipes.push_back(newPipe);                          // Add the pipe
         }
      }

      vjGlPipe* pipe;                           // The pipe to assign it to
      pipe = pipes[pipeNum];                    // ASSERT: pipeNum is in the valid range
      pipe->addWindow(wins[winId]);             // Window has been added
   }

      // Start all the pipes running
   for(int pipeNum=0;pipeNum<pipes.size();pipeNum++)
   {
      //**//if(pipes[pipeNum]->hasWindows())    // Actually we want all the pipes to run
      // Start all pipes running.  Even pipes that have no windows
      // This way the pipe can be ready if windows are added to it
      pipes[pipeNum]->start();
   }

      // --- Setup Multi-Process stuff --- //
      // Create a new thread to handle the control
	vjThread* control_thread;

   vjThreadMemberFunctor<vjGlDrawManager>* memberFunctor =
      new vjThreadMemberFunctor<vjGlDrawManager>(this, &vjGlDrawManager::main, NULL);

	control_thread = new vjThread(memberFunctor, 0);

   vjDEBUG(0) << "vjGlDrawManager::thread: " << control_thread << endl << vjDEBUG_FLUSH;
   // Dump the state
   debugDump();
}
	

    /// Shutdown the drawing API
void vjGlDrawManager::closeAPI()
{
    // Stop all pipes
   ;
    // Delete all pipes

    // Close and delete all glWindows
}

// Draw any objects that we need to display in the scene
// from the system.  (i.e. Gloves, etc)
// XXX: Performance Critical problems here
void vjGlDrawManager::drawObjects()
{
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


void vjGlDrawManager::drawProjections()
{
   std::vector<vjDisplay*> disps = displayManager->getDisplays();

   vjVec3 apex, ur, lr, ul, ll;
   vjProjection* proj(NULL);

   for(int i=0;i<disps.size();i++)
   {
      if(disps[i]->isSurface())
      {
         if(disps[i]->isSurface())
         {
            vjSurfaceDisplay* surf_disp = dynamic_cast<vjSurfaceDisplay*>(disps[i]);
            vjASSERT(surf_disp != NULL);
            proj = surf_disp->getLeftProj();
         }
         else if (disps[i]->isSimulator())
         {
            vjSimDisplay* sim_disp = dynamic_cast<vjSimDisplay*>(disps[i]);
            vjASSERT(sim_disp != NULL);
            proj = sim_disp->getCameraProj();
         }

         // Create color values that are unique
         // Basically count in binary (skipping 0), and use the first 3 digits.  That will give six colors
         int red_on = (i & 0x001); int green_on = (i & 0x010); int blue_on = (i & 0x100);

         float red(0.0f), green(0.0f), blue(0.0f);
         if(red_on) red = 1.0f;
         if(green_on) green = 1.0f;
         if(blue_on) blue = 1.0f;

         if((!red_on) && (!blue_on) && (!green_on))      // Case of 0's (black is bad)
            red = blue = green = 0.75f;

         // Draw the thingy
         proj->getFrustumApexAndCorners(apex, ur, lr, ul, ll);
         glPushMatrix();
            glColor3f(red, green, blue);
            drawLine(apex, ur); drawLine(apex, lr); drawLine(apex, ul); drawLine(apex, ll);
            drawLine(ur, lr); drawLine(lr, ll); drawLine(ll, ul); drawLine(ul, ur);
         glPopMatrix();
      }
   }
}

//: Draw a simulator using OpenGL commands
//! NOTE: This is called internally by the library
void vjGlDrawManager::drawSimulator(vjSimDisplay* sim)
{
   const float head_radius(0.75);
   const float eye_vertical(0.22);
   const float eye_horizontal(0.7);
   const float interoccular(0.27);
   const float eye_radius(0.08f);

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
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
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
	//-----------------------------------------------------
	
      // Draw a cave-like outline
   glPushAttrib( GL_ENABLE_BIT | GL_LIGHTING_BIT );
   glDisable(GL_LIGHTING);
   glPushMatrix();
      glPushMatrix();
         glColor3f(1.0f, 1.0f, 1.0f);
	      glTranslatef(0.0f, 5.0f, 0.0f);      // Center it on 0,0,0
	      drawWireCube(10.0f);
      glPopMatrix();
      drawProjections();
   glPopMatrix();
   glPopAttrib();


	  // Draw the user's head
       glPushMatrix();
	  glMultMatrixf(sim->getHeadPos().getFloatPtr());
	  glColor3f(1.0f, 0.0f, 0.0f);
	  drawSphere(head_radius, 10, 10);             // Head
	  glPushMatrix();
	     glColor3f(0.0f, 0.0f, 1.0f);
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
	  glColor3f(1.0f, 0.0f, 1.0f);
	  drawCone(0.2, 0.6f, 6, 1);
       glPopMatrix();
   }
   //glPopAttrib();
}

    /// dumps the object's internal state
void vjGlDrawManager::debugDump()
{
    vjDEBUG(0) << "-- DEBUG DUMP --------- vjGlDrawManager: " << (void*)this << " ------------" << endl
	         << "\tapp:" << (void*)app << endl << vjDEBUG_FLUSH;
    vjDEBUG(0) << "\tWins:" << wins.size() << endl << vjDEBUG_FLUSH;

    for(std::vector<vjGlWindow*>::iterator i = wins.begin(); i != wins.end(); i++)
    {
	   vjDEBUG(0) << "\n\t\tvjGlWindow:\n" << *(*i) << endl << vjDEBUG_FLUSH;
    }
    vjDEBUG(0) << flush << vjDEBUG_FLUSH;
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
      glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);

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


