#include <Common/ViewPlatform.h>
#include <Common/NavStrategy.h>

void ViewPlatform::update(User* user)
{
   mNavStrategy->update(user);
}

gmtl::Matrix44f ViewPlatform::getTransform_virtMplat()
{
   if(NULL != mNavStrategy)
   {
      return mNavStrategy->getPosition();
   }
   else
   {
      return gmtl::Matrix44f();  // Return an identity matrix
   }
}

gmtl::Matrix44f ViewPlatform::getTransform_platMvirt()
{
   if(NULL != mNavStrategy)
   {
      return mNavStrategy->getPositionInv();
   }
   else
   {
      return gmtl::Matrix44f();  // Return an identity matrix
   }
}


void ViewPlatform::setNavStrategy(NavStrategy* navStrategy)
{
   if(NULL != navStrategy)
   {
      // If we already have a strategy, then copy over the current position
      if(NULL != mNavStrategy)
      {
         navStrategy->setPosition( mNavStrategy->getPosition() );
      }
      mNavStrategy = navStrategy;
   }
   else  // Clear out strategy
   {
      mNavStrategy = NULL;
   }
}

NavStrategy* ViewPlatform::getNavStrategy()
{
   return mNavStrategy;
}

