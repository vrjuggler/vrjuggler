#include <Kernel/vjViewport.h>

namespace vrj
{
   
void Viewport::config(ConfigChunk* chunk)
{
   vprASSERT(chunk != NULL);

   // -- Get config info from chunk -- //
    float originX = chunk->getProperty("origin", 0);
    float originY = chunk->getProperty("origin", 1);
    float sizeX   = chunk->getProperty("size", 0);
    float sizeY   = chunk->getProperty("size", 1);
    std::string name  = chunk->getProperty("name");
    mView    = (Viewport::View)(int)chunk->getProperty("view");
    mActive  = chunk->getProperty("active");

   // -- Check for error in configuration -- //
   // NOTE: If there are errors, set them to some default value
   if(sizeX <= 0)
   {
      vjDEBUG(vjDBG_DISP_MGR,2) << "WARNING: viewport sizeX set to: " << sizeX
                                << ".  Setting to 1." << std::endl
                                << vjDEBUG_FLUSH;
      sizeX = 1.0f;
   }

   if(sizeY <= 0)
   {
      vjDEBUG(vjDBG_DISP_MGR,2) << "WARNING: viewport sizeY set to: " << sizeY
                                << ".  Setting to 1." << std::endl
                                << vjDEBUG_FLUSH;
      sizeY = 1.0f;
   }

         // -- Set local window attributes --- //
    setOriginAndSize(originX, originY, sizeX, sizeY);

    // Get the user for this display
    std::string user_name = chunk->getProperty("user");
    mUser = Kernel::instance()->getUser(user_name);

    if(NULL == mUser)
    {
       vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << " User not found named: "
                              << user_name.c_str() << std::endl
                              << vjDEBUG_FLUSH;
      vprASSERT(false && "User not found in Viewport::config");
    }

    setName(name);
    mViewportChunk = chunk;        // Save the chunk for later use
}

std::ostream& operator<<(std::ostream& out, Viewport& viewport)
{
   return viewport.outStream(out);
}

std::ostream& Viewport::outStream(std::ostream& out)
{
   out << "user: " << getName()
       << "  org:" << mXorigin << ", " << mYorigin
       << "  sz:" << mXsize << ", " << mYsize
       << "  view:" << ((mView == Viewport::LEFT_EYE) ? "Left" : ((mView==Viewport::RIGHT_EYE)?"Right" : "Stereo") )
       << std::flush;

   return out;
}


};
