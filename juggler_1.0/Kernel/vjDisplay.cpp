#include <vjConfig.h>
#include <Kernel/vjDisplay.h>
#include <Kernel/vjDisplayManager.h>
#include <Kernel/vjWallProjection.h>
#include <Kernel/vjCameraProjection.h>
#include <Kernel/vjSimulator.h>
#include <Kernel/vjDebug.h>

vjDisplay::vjDisplay()
{
   leftProj = NULL;
   rightProj = NULL;

   //dispMgr = _parentManager;
}

void vjDisplay::config(vjConfigChunk* chunk)
{
   vjASSERT(chunk != NULL);

   // -- Get config info from chunk -- //
   char* proj  = chunk->getProperty("projectiontype");
    int originX = chunk->getProperty("origin", 0);
    int originY = chunk->getProperty("origin", 1);
    int sizeX   = chunk->getProperty("size", 0);
    int sizeY   = chunk->getProperty("size", 1);
    char* name  = chunk->getProperty("name");
    mBorder     = chunk->getProperty("border");
    int pipe    = chunk->getProperty("pipe");
    mStereo  = chunk->getProperty("stereo");
    bool sim = chunk->getProperty("simulator");

      // -- Check for error in configuration -- //
      // NOTE: If there are errors, set them to some default value
   if(sizeX <= 0)
   {  vjDEBUG(0) << "Error: window sizeX set to: " << sizeX << ".  Setting to 100." << endl << vjDEBUG_FLUSH;
      sizeX = 10; }

   if(sizeY <= 0)
   {  vjDEBUG(0) << "Error: window sizeY set to: " << sizeY << ".  Setting to 100." << endl << vjDEBUG_FLUSH;
      sizeY = 10; }

      // -- Set local window attributes --- //
    setOriginAndSize(originX, originY, sizeX, sizeY);

      // Set type
    if(sim)
    {
      mType = SIM;
      cameraProj = new vjCameraProjection;
      cameraProj->config(chunk);
      cameraProj->setEye(vjProjection::SIM);
      mSim = new vjSimulator;
      mSim->config(chunk);
    }
    else
    {
      mType = PROJ;
    }

    // Setup head interface
    /*
    char* head_str = NULL;
    head_str  = chunk->getProperty("headPos");
    mHeadInterface.init(head_str);

    if(mHeadInterface.getProxyIndex() == -1)
    {
      cerr << "Fatal Error: Head not found named: " << head_str << endl;
      exit(1);
    }
    */

    // Setup head pos by getting user
    std::string user_name = (std::string)(char*)chunk->getProperty("user");
    mUser = vjKernel::instance()->getUser(user_name);

    if(NULL == mUser)
    {
       vjDEBUG(0) << "ERROR: User not found named: " << user_name << endl << vjDEBUG_FLUSH;
    }

    // XXX HACK: This should not be here
    //**// if(proj != 'User')
    leftProj = new vjWallProjection;
    rightProj = new vjWallProjection;

      // configure projections
    leftProj->config(chunk);
    leftProj->setEye(vjProjection::LEFT);
    rightProj->config(chunk);
    rightProj->setEye(vjProjection::RIGHT);

    setName(name);
    setPipe(pipe);


    displayChunk = chunk;        // Save the chunk for later use
}

void vjDisplay::updateProjections()
{
   vjMatrix left_eye_pos, right_eye_pos;     // NOTE: Eye coord system is -z forward, x-right, y-up

   // -- Calculate Eye Positions -- //
   vjMatrix cur_head_pos = *(mUser->getHeadPos());
   vjCoord  head_coord(cur_head_pos);       // Create a user readable version

   vjDEBUG(4) << "vjDisplay::updateProjections: Getting head position" << endl << vjDEBUG_FLUSH;
   vjDEBUG(4) << "\tHeadPos:" << head_coord.pos << "\tHeadOr:" << head_coord.orient << endl << vjDEBUG_FLUSH;

   // Compute location of left and right eyes
   float interocularDist = 2.75/12.0f;
   float eye_offset = interocularDist/2.0f;      // Distance to move eye

   left_eye_pos.postTrans(cur_head_pos, -eye_offset, 0, 0);
   right_eye_pos.postTrans(cur_head_pos, eye_offset, 0, 0);


   if (mType == PROJ)
   {
      leftProj->calcViewMatrix(left_eye_pos);
      rightProj->calcViewMatrix(right_eye_pos);
   }
   else
   {
      mSim->update();
      vjMatrix camera_pos = mSim->getCameraPos();
      cameraProj->calcViewMatrix(camera_pos);
   }
}
	
    // ---- FRIEND FUNCTIONS ---- //
ostream& operator<<(ostream& out,  vjDisplay& disp)
{
    out << "------- vjDisplay:" << (void*)(&disp) << " ------\n";
    out << "\tOrigin:" << disp._xo << ", " << disp._yo << endl;
    out << "\t  Size:" << disp._xs << ", " << disp._ys << endl;
    out << "\t  Pipe:" << disp.mPipe << endl;
    out << "\t  Stereo:" << disp.mStereo << endl;
    out << "\t  User:" << disp.mUser->getName() << endl;
    out << "---------------------------------------\n";

    return out;	
}
