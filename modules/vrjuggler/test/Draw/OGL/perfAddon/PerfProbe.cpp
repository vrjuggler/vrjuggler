#include <GL/gl.h>
#include <GL/glext.h>
#include <PerfProbe.h>
#include <gmtl/Output.h>
#include <Text.h>
#include <string>

#include <vpr/Perf/ProfileManager.h>
#include <vpr/Perf/ProfileIterator.h>

#include <vrj/Kernel/Kernel.h>

//#include <boost/format.hpp>

namespace
{
   template<class OutIt>
   void splitStr(
      const std::string& s,
      const std::string& sep,
      OutIt dest)
   {
      std::string::size_type left = s.find_first_not_of( sep );
      std::string::size_type right = s.find_first_of( sep, left );
      while( left < right )
      {
         *dest = s.substr( left, right-left );
         ++dest;
         left = s.find_first_not_of( sep, right );
         right = s.find_first_of( sep, left );
      }
   }

   //std::string s = "apple, orange, cherry, peach, grapefruit, cantalope,watermelon";

   //std::vector<std::string> v;
   //split( s, " ,", std::back_inserter(v) );

}


PerfProbe::PerfProbe()
   : mLastUpdateTime(0, vpr::Interval::Base),
     mFrameRate(0)
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
      mFrameTimes.resize(10);
      mFrameRate = 0.0f;
      for(std::deque<float>::iterator i=mFrameTimes.begin(); i!=mFrameTimes.end(); ++i)
      { mFrameRate += *i; }
      mFrameRate /= float(mFrameTimes.size());
      mFrameRate = 1/mFrameRate;
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

   glPopAttrib();
}


void PerfProbe::drawOverlayText()
{
   const float font_height(10.0f);
   const float font_h_spacing(font_height*1.3f);
   float win_height(1000);
   float win_width(win_height * 1.4f);

   std::string overlay_text = mOverlayTextStream.str();
   std::vector<std::string> overlay_lines;
   splitStr(overlay_text,"\n",std::back_inserter(overlay_lines));

   float needed_h = font_h_spacing*float(overlay_lines.size());
   if(needed_h > win_height)
   {
      win_height = needed_h * 1.1f;
      win_width = win_height * 1.4f;
   }

   glMatrixMode( GL_PROJECTION );
   glPushMatrix();
   glLoadIdentity();
   glOrtho( 0.0, win_width, 0.0, win_height, -1.0, 1.0);     // Setup virtual space 1000, 1000

   glMatrixMode( GL_MODELVIEW );
   glPushMatrix();
   glLoadIdentity();

   glDisable(GL_DEPTH_TEST);

   /*
   glBegin(GL_TRIANGLES);
      glColor3f(1.0, 0.0, 0);
      glVertex3f(100,100,-0.5);
      glColor3f(0,1,0);
      glVertex3f(900,100,-0.5);
      glColor3f(0,0,1);
      glVertex3f(450,800,-0.5);
   glEnd();
   */
   //Text::getSingleton()->drawString(Text::StrokeFont, std::string("No Translation"));

   glColor3f(1.0,1.0,0.0);
   glTranslatef(10, win_height-30, 0);
   //float num_lines(overlay_lines.size());

   for (unsigned l=0;l<overlay_lines.size();++l)
   {
      glPushMatrix();
      glTranslatef(0,-font_h_spacing*float(l), 0);
      Text::getSingleton()->drawString(Text::StrokeFont, overlay_lines[l]);
      glPopMatrix();
   }

   glEnable(GL_DEPTH_TEST);

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode( GL_MODELVIEW);
   glPopMatrix();
}


