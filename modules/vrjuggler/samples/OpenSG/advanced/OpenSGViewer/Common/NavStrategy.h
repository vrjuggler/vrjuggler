#ifndef NAVSTRATEGY_H
#define NAVSTRATEGY_H

#include <gmtl/Matrix.h>

class User;

/**
* Strategy for doing navigation.
* The Viewplatform uses this strategy to move itself through the virtual world.
*
* We just track the simple idea of our position within the virtual world.  The platform
* and user handle all the math to update the other matricies in the system.
*/
class NavStrategy
{
public:
   /** Explicitly set the navigation position */
   virtual void setPosition(gmtl::Matrix44f pos)
   { mCurPos = pos; }

   /** Get the current navigation position */
   virtual gmtl::Matrix44f getPosition()
   { return mCurPos; }

   /** Get the current navigation position */
   virtual gmtl::Matrix44f getPositionInv()
   { return mCurPosInv; }

   /**
     * Template method for updating the view platform (navigation, etc).
     * Called once per frame
     * @param user - The user that is currently controlling the view platform
     */
    virtual void update(User* user) = 0;
protected:
   gmtl::Matrix44f mCurPos;   /**< The current position */
   gmtl::Matrix44f mCurPosInv;   /**< The current position inverse*/
};
#endif //NAVSTRATEGY_H

