/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
#include <Kernel/vjDebug.h>

#include <Kernel/vjDisplay.h>
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
       vjDEBUG(vjDBG_ERROR,0) << "ERROR: User not found named: " << user_name.c_str() << endl << vjDEBUG_FLUSH;
      vjASSERT(false);
    }

    setName(name);
    setPipe(pipe);

    mDisplayChunk = chunk;        // Save the chunk for later use
}

	
    // ---- FRIEND FUNCTIONS ---- //
//! PRE: disp != NULL
//+      disp->mUser != NULL
ostream& vjDisplay::outStream(ostream& out)
{
   vjASSERT(mUser != NULL);

    out << setw(15) << mName.c_str()
        << "  org:" << _xo << ", " << _yo
        << "  sz:" << _xs << ", " << _ys
        << "  p:" << mPipe
        << "  view:" << ((mView == vjDisplay::LEFT_EYE) ? "Left" : ((mView==vjDisplay::RIGHT_EYE)?"Right" : "Stereo") )
        << "  act:" << (mActive ? "Y" : "N")
        << "  usr:" << mUser->getName().c_str();

    return out;	
}


ostream& operator<<(ostream& out,  vjDisplay& disp)
{
   return disp.outStream(out);
}
