/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <Common/User.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>

#include <Common/PointAndGo.h>


void User::update()
{
   mViewPlatform.update(this);   // Update the view platform

   // --- UPDATE USER POSITION --- //
   gmtl::Matrix44f vw_M_plat = mViewPlatform.getTransform_virtMplat();
   gmtl::Matrix44f head_pos_world;  // Head position in real world (platform) coords
   head_pos_world = getHeadPos()->getData();

   M_vwMuser = vw_M_plat * head_pos_world;   
}

void User::init()
{
   mHeadInterface.init("VJHead");
   mWandInterface.init("VJWand");

   gadget::DigitalInterface* btn0 = new gadget::DigitalInterface;
   btn0->init("VJButton0");
   gadget::DigitalInterface* btn1 = new gadget::DigitalInterface;
   btn1->init("VJButton1");
   gadget::DigitalInterface* btn2 = new gadget::DigitalInterface;
   btn2->init("VJButton2");
   gadget::DigitalInterface* btn3 = new gadget::DigitalInterface;
   btn3->init("VJButton3");
   gadget::DigitalInterface* btn4 = new gadget::DigitalInterface;
   btn4->init("VJButton4");
   
   mButtonInterfaces.push_back(btn0);
   mButtonInterfaces.push_back(btn1);
   mButtonInterfaces.push_back(btn2);
   mButtonInterfaces.push_back(btn3);
   mButtonInterfaces.push_back(btn4);

   // Set to default navigation strategy
   mViewPlatform.setNavStrategy( new PointAndGo);
}

