#ifndef POINTANDGO_H
#define POINTANDGO_H

#include <Common/NavStrategy.h>
#include <vpr/Util/Interval.h>

//#include <snx/SoundHandle.h>

class User;

/**
* Simple navigation strategy that just goes where the user points
*/
class PointAndGo : public NavStrategy
{
public:
   enum SoundNames { NavTranslateSound = 0, NavRotateSound = 1, NumSounds = 2 };

public:
   PointAndGo()
      : mVelocity(0), mAllowRotate(true)
   {
      mLastUpdate = vpr::Interval::NoTimeout;
   //   initSounds();
   }

   virtual ~PointAndGo()
   {;}

   //void initSounds();

   /**
     * Template method for updating the view platform (navigation, etc).
     * Called once per frame
     * @param user - The user that is currently controlling the view platform
     */
    virtual void update(User* user);

protected:
   float          mVelocity;     /**< Velocity of current travel */
   bool           mAllowRotate;  /**< Wether or not to allow rotations */
   vpr::Interval  mLastUpdate;   /**< Time of the last update, used to compute delta */
    
protected:
   //std::vector<snx::SoundHandle> mSounds;
};
#endif //POINTANDGO_H

