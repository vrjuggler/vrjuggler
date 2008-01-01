/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/gl.h>
#  if ! defined(WIN32) && ! defined(WIN64) && ! defined(__sgi)
#     include <GL/glext.h>
#  endif
#endif

#include <PerfProbe.h>
#include <gmtl/Output.h>
#include <Text.h>
#include <string>

#include <vpr/Perf/ProfileManager.h>
#include <vpr/Perf/ProfileIterator.h>

#include <vrj/Kernel/Kernel.h>
#include <vpr/Util/Debug.h>

//#include <boost/format.hpp>



PerfProbe::PerfProbe()
   : mLastUpdateTime(0, vpr::Interval::Base),
     mFrameRate(0), mNumTrackedFrames(500), mMaxFrameRate(200.0f)
{
}

void PerfProbe::update()
{
   // Reset stream for output
   mOverlayTextStream.str(std::string(""));     // Clear it

   // Update frame rate info.
   if(mLastUpdateTime.getBaseVal() != 0)
   {
      vpr::Interval frame_time = vpr::Interval::now() - mLastUpdateTime;
      mFrameTimes.push_front(frame_time.secf());
      mFrameTimes.resize(100);
      mFrameRate = 0.0f;
      for(std::deque<float>::iterator i=mFrameTimes.begin(); i!=mFrameTimes.end(); ++i)
      { mFrameRate += *i; }
      mFrameRate /= float(mFrameTimes.size());
      mFrameRate = 1/mFrameRate;

      // Track frame rates
      mFrameRateHistory.push_front(1.0f/frame_time.secf());    // Add more data about frame rates
      mFrameRateHistory.resize(mNumTrackedFrames,0.0f);        // Resize and fill if needed.
   }
   mLastUpdateTime.setNow();

   mOverlayTextStream << "fps: " << mFrameRate << std::endl;


   mOverlayTextStream << "\nKernel loop profile:\n";

   vpr::ProfileIterator prof_node(vpr::ProfileManager::getRootNode(const_cast<vpr::Thread*>(vrj::Kernel::instance()->getThread())) );
   vpr::ProfileIterator end_node(NULL);
   for(; prof_node != end_node; ++prof_node)
   {
      for(unsigned d=0;d<prof_node.depth();d++)
      { mOverlayTextStream << "   "; }
      mOverlayTextStream << (*prof_node).getName() << "  " << (*prof_node).getSTA().msecf() << std::endl;
   }        
}

void PerfProbe::draw()
{
   glPushAttrib(GL_ALL_ATTRIB_BITS);

   glDisable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);

   drawOverlayText();
   drawFrameRateGraph();

   glPopAttrib();
}


void PerfProbe::drawOverlayText()
{   
   std::string overlay_text = mOverlayTextStream.str();
   
   glColor3f(1.0,1.0,0.0);
   mText->drawTextBlock(overlay_text);      
}

void PerfProbe::drawFrameRateGraph()
{
   float win_height(1000);
   float win_width(1000);
   float pos_x(200),pos_y(875);
   float graph_width(750), graph_height(100);

   glMatrixMode( GL_PROJECTION );
   glPushMatrix();
   glLoadIdentity();
   glOrtho( 0.0, win_width, 0.0, win_height, -1.0, 1.0);     // Setup virtual space 1000, 1000

   glMatrixMode( GL_MODELVIEW );
   glPushMatrix();
   glLoadIdentity();

   glDisable(GL_DEPTH_TEST);

   unsigned num_samples(mFrameRateHistory.size());

   glColor3f(1.0f, 0.0f, 0.0f);
   glBegin(GL_LINE_STRIP);
      for(unsigned i=0;i<num_samples;++i)
      {
         float x_offset(graph_width*(float(i)/float(num_samples)));
         float y_offset(graph_height*(mFrameRateHistory[i]/mMaxFrameRate));
         glVertex3f(pos_x+x_offset, pos_y+y_offset, 0.0f);
      }
   glEnd();

   // Draw border
   glColor3f(0,0,1);
   glBegin(GL_LINE_STRIP);
      glVertex3f(pos_x,pos_y,0);
      glVertex3f(pos_x+graph_width,pos_y,0);
      glVertex3f(pos_x+graph_width,pos_y+graph_height,0);
      glVertex3f(pos_x,pos_y+graph_height,0);
      glVertex3f(pos_x,pos_y,0);
   glEnd();


   glEnable(GL_DEPTH_TEST);

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode( GL_MODELVIEW);
   glPopMatrix();
}
