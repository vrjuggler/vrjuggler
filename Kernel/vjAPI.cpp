#include <config.h>
#include <iostream.h>

#include <Kernel/vjAPI.h>
#include <Kernel/vjDebug.h>

#ifdef VJ_API_PERFORMER
#   include <Kernel/Pf/vjPfAPIFactory.h>
#endif

#ifdef VJ_API_OPENGL
#   include <Kernel/GL/vjGlAPIFactory.h>
#endif

#include <Kernel/vjAPIFactory.h>


// Function to return the API factory we want
vjAPIFactory* vjAPI::getAPIFactory()
{
   vjAPIFactory* retVal = NULL;          // Default to a null return

   switch (mApi)
   {
   case PERFORMER:
#ifdef VJ_API_PERFORMER
      retVal = vjPfAPIFactory::instance();
#else
      vjDEBUG(0) << "Performer API is not available.\n";
#endif

      break;
   case OPENGL:
#ifdef VJ_API_OPENGL
      retVal = vjGlAPIFactory::instance();
#else
      vjDEBUG(0) << "OpenGL API is not available.\n";
#endif

      break;
   }

   return retVal;
}
