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
   {  vjDEBUG(vjDBG_DISP_MGR,2) << "WARNING: window sizeX set to: " << sizeX << ".  Setting to 10." << endl << vjDEBUG_FLUSH;
      sizeX = 10; }

   if(sizeY <= 0)
   {  vjDEBUG(vjDBG_DISP_MGR,2) << "WARNING: window sizeY set to: " << sizeY << ".  Setting to 10." << endl << vjDEBUG_FLUSH;
      sizeY = 10; }

   if(pipe < 0)
   {
      vjDEBUG(vjDBG_DISP_MGR,2) << "WARNING: pipe was negative, pipe set to: " << pipe << ".  Setting to 0.\n" << vjDEBUG_FLUSH;
      pipe = 0;
   }

      // -- Set local window attributes --- //
    setOriginAndSize(originX, originY, sizeX, sizeY);

    // Get the user for this display
    std::string user_name = chunk->getProperty("user");
    mUser = vjKernel::instance()->getUser(user_name);

    if(NULL == mUser)
    {
       vjDEBUG(vjDBG_ERROR,0) << "ERROR: User not found named: " << user_name << endl << vjDEBUG_FLUSH;
      vjASSERT(false);
    }

    setName(name);
    setPipe(pipe);

    mDisplayChunk = chunk;        // Save the chunk for later use
}

	
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
