#ifndef USER_H
#define USER_H

#include <vpr/vpr.h>
#include <gmtl/Matrix.h>
#include <Common/ViewPlatform.h>
#include <vpr/Util/Singleton.h>

#include <gadget/Type/DeviceInterface.h>
#include <gadget/Type/PositionProxy.h>
#include <gadget/Type/DigitalProxy.h>
#include <gadget/Type/AnalogProxy.h>


/** Representation and holder for User related data.
*
*
* Coordinate systems used
* 
* The following describes the coordinate systems and representations used
* in the code for User, Platform, and navigation
*
* Coord systems
* - V - Virtual World (vw): The coordinate system that all the data objects are in.
*                      This system is used to represent the "world" that we create.
* - P - Platform (plat): The platform is the "magic carpet" that you are traveling through
*                 the virtual world on.  Navigation methods move this carpet
* - W - World (rw): This is the "real world".  This actually corresponds exactly to the
*              platform since the platform is the VR device you are "in"
*/
class User : public vpr::Singleton<User>
{
public:
   User()
   {;}

public:

   /** Initialize the user */
   void init();

   /**
   * Get the positon of the user in the virtual world
   */
   gmtl::Matrix44f getUserPosition_vw()
   { return M_vwMuser; }

   /**
   * Get the xformation from world space to local user space   
   */
   gmtl::Matrix44f getXform_vwMrw()
   { return mViewPlatform.getTransform_virtMplat(); }

   /** User interaction stuff */   
   gadget::PositionInterface& getHeadPos()
   { return mHeadInterface; }
   gadget::PositionInterface& getWandPos()
   { return mWandInterface; }
   gadget::DigitalInterface& getButton(unsigned buttonNum)
   { return *(mButtonInterfaces[buttonNum]); }

   ViewPlatform& viewPlatform()
   {  return mViewPlatform;   }

public:
   /**
   * Update the user state
   */
   void update();
   
protected:
   ViewPlatform      mViewPlatform;    /**< The viewplatform for this user */ 
   gmtl::Matrix44f   M_vwMuser;        /**< Position of user in virtual world */
    //gmtl::Matrix44f  mUserMworld;      /**< Matrix to convert for world coords to virtual world coords */

protected:
   /** User interface stuff */
   gadget::PositionInterface              mHeadInterface;
   gadget::PositionInterface              mWandInterface;      /**< The wand position interface */
   std::vector<gadget::DigitalInterface*> mButtonInterfaces;   /**< Button interfaces */
};

#endif //USER_H

