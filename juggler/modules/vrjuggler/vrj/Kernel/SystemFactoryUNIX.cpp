#include <vjConfig.h>
#include <Kernel/vjSGISystemFactory.h>
#include <Kernel/GL/vjGlxWindow.h>


vjSGISystemFactory* vjSGISystemFactory::_instance = NULL;

vjGlWindow* vjSGISystemFactory::getGLWindow()
{
   return new vjGlxWindow;
}

