// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _SIMPLE_MANAGED_CXX_APP_H_
#define _SIMPLE_MANAGED_CXX_APP_H_

#using <mscorlib.dll>
#using <jcclnet.dll>
#using <gadgetnet.dll>
#using <vrjnet.dll>
#using <Tao.OpenGl.dll>


__gc class SimpleApp : public vrj::GlApp
{
public:
   SimpleApp()
      : vrj::GlApp()
      , mBoxGrabbed(false)
   {
      mButton0 = new gadget::DigitalInterface();
      mButton1 = new gadget::DigitalInterface();
      mWand    = new gadget::PositionInterface();
   }

   virtual void init();

   virtual void contextInit()
   {
      initGLState();
   }

   virtual void preFrame();

   virtual void bufferPreDraw();

   virtual void draw();

private:
   void initGLState();

   void drawCube()
   {
      drawbox(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, Tao::OpenGl::Gl::GL_QUADS);
   }

   void drawbox(double x0, double x1, double y0, double y1,
                double z0, double z1, int type);

   bool mBoxGrabbed;
   gadget::DigitalInterface* mButton0;
   gadget::DigitalInterface* mButton1;
   gadget::PositionInterface* mWand;
};


#endif /* _SIMPLE_MANAGED_CXX_APP_H_ */
