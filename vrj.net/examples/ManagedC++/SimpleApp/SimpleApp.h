/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

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
