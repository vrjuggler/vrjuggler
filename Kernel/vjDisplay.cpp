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
    char* name  = chunk->getProperty("name");
    mBorder     = chunk->getProperty("border");
    int pipe    = chunk->getProperty("pipe");
    mStereo  = chunk->getProperty("stereo");
    mActive  = chunk->getProperty("active");

    // -- Check for error in configuration -- //
    // NOTE: If there are errors, set them to some default value
   if(sizeX <= 0)
   {  vjDEBUG(0) << "WARNING: window sizeX set to: " << sizeX << ".  Setting to 10." << endl << vjDEBUG_FLUSH;
      sizeX = 10; }

   if(sizeY <= 0)
   {  vjDEBUG(0) << "WARNING: window sizeY set to: " << sizeY << ".  Setting to 10." << endl << vjDEBUG_FLUSH;
      sizeY = 10; }

   if(pipe < 0)
   {
      vjDEBUG(0) << "WARNING: pipe set to: " << pipe << ".  Setting to 0.\n" << vjDEBUG_FLUSH;
      pipe = 0;
   }

      // -- Set local window attributes --- //
    setOriginAndSize(originX, originY, sizeX, sizeY);

    // Get the user for this display
    std::string user_name = (std::string)(char*)chunk->getProperty("user");
    mUser = vjKernel::instance()->getUser(user_name);

    if(NULL == mUser)
    { vjDEBUG(0) << "ERROR: User not found named: " << user_name << endl << vjDEBUG_FLUSH; }

    setName(name);
    setPipe(pipe);

    displayChunk = chunk;        // Save the chunk for later use
}

	
    // ---- FRIEND FUNCTIONS ---- //
ostream& operator<<(ostream& out,  vjDisplay& disp)
{
    out << "------- vjDisplay:" << (void*)(&disp) << " ------\n";
    out << "\tOrigin:" << disp._xo << ", " << disp._yo << endl;
    out << "\t  Size:" << disp._xs << ", " << disp._ys << endl;
    out << "\t  Pipe:" << disp.mPipe << endl;
    out << "\t  Stereo:" << disp.mStereo << endl;
    out << "\t  Active:" << disp.mActive << endl;
    out << "\t  User:" << disp.mUser->getName() << endl;
    out << "---------------------------------------\n";

    return out;	
}
