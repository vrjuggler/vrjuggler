#ifndef _DISPLAY_INFO_MODE_H_
#define _DISPLAY_INFO_MODE_H_

#include <TestMode.h>

#ifdef VPR_OS_Darwin
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <vrj/Draw/OGL/GlDrawManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/Display.h>
#include <vrj/Draw/OGL/GlContextData.h>

#include <sstream>

#include "WallTest.h"
#include "WallTestHelpers.h"
#include <Text.h>

class DisplayInfoMode : public TestMode
{
public:
   virtual std::string getName()
   { return std::string("DisplayInfoMode"); }

   virtual void update(WallTest *wallTest)
   {
   }
   
   virtual void draw(WallTest *wallTest)
   {
      DisplayManager* displayManager   = vrj::GlDrawManager::instance()->getDisplayManager();
      vrj::GlUserData* user_data       = vrj::GlDrawManager::instance()->currentUserData();
      vrj::Display* display            = user_data->getViewport()->getDisplay();
      vrj::Viewport* viewport          = user_data->getViewport();
      vrj::Projection* proj            = user_data->getProjection();
      int cur_eye                      = proj->getEye();

      std::stringstream display_text;

      display_text << "Display Info\n"
                   << "   Display: " << display->getName() << std::endl
                   << "      pipe: " << display->getPipe() << std::endl;
      
      if(vrj::Projection::LEFT == cur_eye)
      {  display_text << "   Eye: Left" << std::endl; }
      else
      {  display_text << "   Eye: Right" << std::endl; }

      display_text << "  Viewport: " << std::endl;
      viewport->outStream(display_text, 6);
      
      display_text << std::endl;
      
      std::string text = display_text.str();

      glColor3f(1.0,1.0,0.0);
      mText->drawTextBlock(text, 0.4, 0.4);  
   }

protected:
   vrj::GlContextData<Text>   mText;         /**< Context specific copy of text. */   
};

#endif

