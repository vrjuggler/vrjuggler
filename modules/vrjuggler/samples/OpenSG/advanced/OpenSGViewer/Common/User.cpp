#include <Common/User.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>


void User::update()
{
   mViewPlatform.update(this);   // Update the view platform

   // --- UPDATE USER POSITION --- //
   gmtl::Matrix44f vw_M_plat = mViewPlatform.getTransform_virtMplat();
   gmtl::Matrix44f head_pos_world;  // Head position in real world (platform) coords
   head_pos_world = *(getHeadPos()->getData());

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
}

