/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>

#include <iomanip>

#include <jccl/Config/ConfigElement.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/Display.h>


namespace vrj
{

Display::Display()
   : mOriginX(-1)
   , mOriginY(-1)
   , mWidth(-1)
   , mHeight(-1)
   , mBorder(true)
   , mHideMouse(false)
   , mFullScreen(false)
   , mAlwaysOnTop(false)
   , mPipe(-1)
   , mActive(true)
   , mStereoRequested(false)
{
   ;
}

Display::~Display()
{
   typedef std::vector<vrj::Viewport*>::iterator iter_type;
   for ( iter_type i = mViewports.begin(); i != mViewports.end(); ++i )
   {
      delete *i;
   }

   mViewports.clear();
}

void Display::updateProjections(const float positionScale)
{
   for ( unsigned int i = 0; i < mViewports.size(); ++i )
   {
      mViewports[i]->updateProjections(positionScale);
   }
}

void Display::setOriginAndSize(const int originX, const int originY,
                               const int width, const int height,
                               const bool updateConfig)
{ 
   mOriginX = originX;
   mOriginY = originY;
   mWidth   = width;
   mHeight  = height;

   if ( updateConfig )
   {
      mDisplayElement->setProperty<int>("origin", 0, originX);
      mDisplayElement->setProperty<int>("origin", 1, originY);
      mDisplayElement->setProperty<int>("size", 0, width);
      mDisplayElement->setProperty<int>("size", 1, height);
   }
}


void Display::config(jccl::ConfigElementPtr element)
{
   vprASSERT(element.get() != NULL);

   configDisplayWindow(element);
   configViewports(element);
}

void Display::configDisplayWindow(jccl::ConfigElementPtr element)
{
   vprASSERT(element.get() != NULL);

   // -- Get config info from element -- //
   int originX      = element->getProperty<int>("origin", 0);
   int originY      = element->getProperty<int>("origin", 1);
   int sizeX        = element->getProperty<int>("size", 0);
   int sizeY        = element->getProperty<int>("size", 1);
   std::string name = element->getName();
   mBorder          = element->getProperty<bool>("border");
   mHideMouse       = element->getProperty<bool>("hide_mouse");
   mFullScreen      = element->getProperty<bool>("full_screen");
   mAlwaysOnTop     = element->getProperty<bool>("always_on_top");
   int pipe         = element->getProperty<int>("pipe");
   mActive          = element->getProperty<bool>("active");
   mStereoRequested = element->getProperty<bool>("stereo");

   // -- Check for error in configuration -- //
   // NOTE: If there are errors, set them to some default value
   if(sizeX <= 0)
   {
      vprDEBUG(vrjDBG_DISP_MGR, vprDBG_WARNING_LVL)
         << "WARNING: window sizeX set to: " << sizeX
         << ".  Setting to 10." << std::endl << vprDEBUG_FLUSH;
      sizeX = 10;
   }

   if(sizeY <= 0)
   {
      vprDEBUG(vrjDBG_DISP_MGR, vprDBG_WARNING_LVL)
         << "WARNING: window sizeY set to: " << sizeY
         << ".  Setting to 10." << std::endl << vprDEBUG_FLUSH;
      sizeY = 10;
   }

   if(pipe < 0)
   {
      vprDEBUG(vrjDBG_DISP_MGR, vprDBG_WARNING_LVL)
         << "WARNING: pipe was negative, pipe set to: " << pipe
         << ".  Setting to 0.\n" << vprDEBUG_FLUSH;
      pipe = 0;
   }

      // -- Set local window attributes --- //
    setOriginAndSize(originX, originY, sizeX, sizeY);

    setName(name);
    setPipe(pipe);

    mDisplayElement = element;        // Save the element for later use
}

void Display::configViewports(jccl::ConfigElementPtr element)
{
   vprASSERT(element.get() != NULL);

   const unsigned int num_sim_vps = element->getNum("simulator_viewports");
   const unsigned int num_surface_vps = element->getNum("surface_viewports");

   jccl::ConfigElementPtr vp_elt;
   SimViewport* sim_vp = NULL;
   SurfaceViewport* surf_vp = NULL;

   // Create sim viewports
   // - Set the parent display
   // - Configure it
   for ( unsigned int i = 0; i < num_sim_vps; ++i )
   {
      vp_elt =
         element->getProperty<jccl::ConfigElementPtr>("simulator_viewports", i);
      sim_vp = new SimViewport;
      sim_vp->setDisplay(shared_from_this());
      if ( sim_vp->config(vp_elt) )
      {
         mViewports.push_back(sim_vp);
      }
      else
      {
         vprDEBUG(vrjDBG_DISP_MGR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR")
            << ": Failed to configure simulator viewport named\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vrjDBG_DISP_MGR, vprDBG_CRITICAL_LVL)
            << "       '" << vp_elt->getName() << "'\n" << vprDEBUG_FLUSH;
         delete sim_vp;
      }
   }

   // Create surface viewports
   // - Set the parent display
   // - Configure it
   for ( unsigned int i = 0; i < num_surface_vps; ++i )
   {
      vp_elt =
         element->getProperty<jccl::ConfigElementPtr>("surface_viewports", i);
      surf_vp = new SurfaceViewport;
      surf_vp->setDisplay(shared_from_this());

      if ( surf_vp->config(vp_elt) )
      {
         mViewports.push_back(surf_vp);
      }
      else
      {
         vprDEBUG(vrjDBG_DISP_MGR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR")
            << ": Failed to configure surface viewport named\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vrjDBG_DISP_MGR, vprDBG_CRITICAL_LVL)
            << "       '" << vp_elt->getName() << "'\n" << vprDEBUG_FLUSH;
         delete surf_vp;
      }
   }
}

jccl::ConfigElementPtr Display::getGlFrameBufferConfig() const
{
   jccl::ConfigElementPtr element;

   // XXX: Refactor this to allow different frame buffer child elements.  Right
   // now, this assumes that the child element type is OpenGLFBConfig.
   if ( mDisplayElement->getNum("frame_buffer_config") == 1 )
   {
      element =
         mDisplayElement->getProperty<jccl::ConfigElementPtr>("frame_buffer_config");
   }

   return element;
}

std::ostream& operator<<(std::ostream& out, Display& disp)
{
   out.setf(std::ios::left);

   char fill_char(out.fill());
   out.fill('.');

   const unsigned int indent_level(2);
   const std::string indent_text(indent_level, ' ');
   const int pad_width_dot(20 - indent_level);

   out << indent_text << std::setw(pad_width_dot)
       << "Name " << " " << disp.mName.c_str() << std::endl;
   out << indent_text << std::setw(pad_width_dot)
       << "Origin " << " " << disp.mOriginX << ", " << disp.mOriginY
       << std::endl;
   out << indent_text << std::setw(pad_width_dot)
       << "Size " << " " << disp.mWidth << "x" << disp.mHeight << std::endl;
   out << indent_text << std::setw(pad_width_dot)
       << "Pipe number " << " " << disp.mPipe << std::endl;
   out << indent_text << std::setw(pad_width_dot)
       << "Stereo requested " << " " << (disp.mStereoRequested ? "Yes" : "No")
       << std::endl;
   out << indent_text << std::setw(pad_width_dot)
       << "Active " << " " << (disp.mActive ? "Yes" : "No") << std::endl;

   for(unsigned i=0;i<disp.mViewports.size();i++)
   {
      out << indent_text << "Viewport " << i << ":\n";
      disp.mViewports[i]->outStream(out, 4);
   }

   // Restore the previous state.
   out.fill(fill_char);

   return out;
}

}
