#include <config.h>
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
   {  vjDEBUG(0) << "Error: window sizeX set to: " << sizeX << ".  Setting to 100." << endl;
      sizeX = 10; }

   if(sizeY <= 0)
   {  vjDEBUG(0) << "Error: window sizeY set to: " << sizeY << ".  Setting to 100." << endl;
      sizeY = 10; }

      // -- Set local window attributes --- //
    setOriginAndSize(originX, originY, sizeX, sizeY);
    
      // Set type
    if(sim)
    {
      mType = SIM;
      cameraProj = new vjCameraProjection;
      cameraProj->config(chunk);
      mSim = new vjSimulator;
      mSim->config(chunk);
    }
    else
    {
      mType = PROJ;
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

void vjDisplay::updateProjections(vjMatrix& leftEyePos, vjMatrix& rightEyePos)
{
    if(mType == PROJ)
    {
      leftProj->calcViewMatrix(leftEyePos);
      rightProj->calcViewMatrix(rightEyePos);
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
    out << "---------------------------------------\n";
    
    return out;	   
}
