#include <vjConfig.h>
#include <Kernel/vjDisplay.h>
#include <Kernel/vjDebug.h>
#include <Kernel/vjKernel.h>


void vjDisplay::config(vjConfigChunk* chunk)
{
   vjASSERT(chunk != NULL);

   // -- Get config info from chunk -- //
    int originX = chunk->getProperty("origin", 0);
    int originY = chunk->getProperty("origin", 1);
    int sizeX   = chunk->getProperty("size", 0);
    int sizeY   = chunk->getProperty("size", 1);
    std::string name  = chunk->getProperty("name");
    mBorder     = chunk->getProperty("border");
    int pipe    = chunk->getProperty("pipe");
    mView    = (vjDisplay::DisplayView)(int)chunk->getProperty("view");
    mActive  = chunk->getProperty("active");

    // -- Check for error in configuration -- //
    // NOTE: If there are errors, set them to some default value
   if(sizeX <= 0)
   {  vjDEBUG(vjDBG_ALL,0) << "WARNING: window sizeX set to: " << sizeX << ".  Setting to 10." << endl << vjDEBUG_FLUSH;
      sizeX = 10; }

   if(sizeY <= 0)
   {  vjDEBUG(vjDBG_ALL,0) << "WARNING: window sizeY set to: " << sizeY << ".  Setting to 10." << endl << vjDEBUG_FLUSH;
      sizeY = 10; }

   if(pipe < 0)
   {
      vjDEBUG(vjDBG_ALL,0) << "WARNING: pipe set to: " << pipe << ".  Setting to 0.\n" << vjDEBUG_FLUSH;
      pipe = 0;
   }

      // -- Set local window attributes --- //
    setOriginAndSize(originX, originY, sizeX, sizeY);

    // Get the user for this display
    std::string user_name = chunk->getProperty("user");
    mUser = vjKernel::instance()->getUser(user_name);

    if(NULL == mUser)
    {
       vjDEBUG(vjDBG_ALL,0) << "ERROR: User not found named: " << user_name << endl << vjDEBUG_FLUSH;
      vjASSERT(false);
    }

    setName(name);
    setPipe(pipe);

    mDisplayChunk = chunk;        // Save the chunk for later use
}

// void vjDisplay::updateProjections()
// {
//    vjMatrix left_eye_pos, right_eye_pos;     // NOTE: Eye coord system is -z forward, x-right, y-up

//    // -- Calculate Eye Positions -- //
//    vjMatrix cur_head_pos = *(mUser->getHeadPos());
//    mHeadUpdateTime = mUser->getHeadUpdateTime();

//    vjCoord  head_coord(cur_head_pos);       // Create a user readable version

//    vjDEBUG(4) << "vjDisplay::updateProjections: Getting head position" << endl << vjDEBUG_FLUSH;
//    vjDEBUG(4) << "\tHeadPos:" << head_coord.pos << "\tHeadOr:" << head_coord.orient << endl << vjDEBUG_FLUSH;

//    // Compute location of left and right eyes
//    float interocularDist = 2.75/12.0f;
//    float eye_offset = interocularDist/2.0f;      // Distance to move eye

//    left_eye_pos.postTrans(cur_head_pos, -eye_offset, 0, 0);
//    right_eye_pos.postTrans(cur_head_pos, eye_offset, 0, 0);


//    if (mType == PROJ)
//    {
//       leftProj->calcViewMatrix(left_eye_pos);
//       rightProj->calcViewMatrix(right_eye_pos);
//    }
//    else
//    {
//       mSim->update();
//       vjMatrix camera_pos = mSim->getCameraPos();
//       cameraProj->calcViewMatrix(camera_pos);
//    }
// }

	
    // ---- FRIEND FUNCTIONS ---- //
//! PRE: disp != NULL
//+      disp->mUser != NULL
ostream& operator<<(ostream& out,  vjDisplay* disp)
{
   vjASSERT(disp != NULL);
   vjASSERT(disp->mUser != NULL);

    //out << "vjDisplay:" << (void*)(&disp)
    out << setw(15) << disp->mName
        << "  org:" << disp->_xo << ", " << disp->_yo
        << "  sz:" << disp->_xs << ", " << disp->_ys
        << "  p:" << disp->mPipe
        << "  view:" << ((disp->mView == vjDisplay::LEFT_EYE) ? "Left" : ((disp->mView==vjDisplay::RIGHT_EYE)?"Right" : "Stereo") )
        << "  act:" << (disp->mActive ? "Y" : "N")
        << "  usr:" << disp->mUser->getName();

    return out;	
}
