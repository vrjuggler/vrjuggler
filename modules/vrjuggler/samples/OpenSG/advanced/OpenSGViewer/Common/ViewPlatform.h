#ifndef VIEWPLATFORM_H
#define VIEWPLATFORM_H

#include <gmtl/Matrix.h>

class NavStrategy;
class User;

/**
* The view platform is the "relative coordinate" system of the user as they
* move through the virtual world.  This of it as the magic carpet that the
* user is standing upon.  Physically this corresponds closest to the concept
* of an IPT device or just the space imediately around the user in the
* real-world.
*
* See User for a description of the coordinate systems used.
*/
class ViewPlatform
{
public:
   ViewPlatform()
      : mNavStrategy(NULL)
   {;}

    /**
     * Updating the view platform (navigation, etc).
     * Called once per frame
     * @param user - The user that is currently controlling the view platform
     */
    void update(User* user);

    /** Return the current position of the platform in the virtual world. */
    gmtl::Matrix44f getTransform_virtMplat();

    /** Return the current inverse of the current pos of plat in vw.
    * This is the amount to move the vw to center on plat
    */
    gmtl::Matrix44f getTransform_platMvirt();
    

public:
   void setNavStrategy(NavStrategy* navStrategy);
   NavStrategy* getNavStrategy();

protected:    

    /**
     * @supplierCardinality 1
     * @directed 
     */
    NavStrategy* mNavStrategy;
};


#endif //VIEWPLATFORM_H

