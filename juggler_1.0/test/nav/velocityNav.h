#include <Math/vjQuat.h>
#include <navigator.h>

class velocityNav : public navigator
{
public:
   enum navMode { GROUND=0, FLY=1 };
   velocityNav();

   void update(vjMatrix mat, bool allowTrans, bool allowRot);
   void incTransVelocity();
   void zeroTransVelocity();

   void setMode(navMode new_mode)
   { mMode = new_mode; }

protected:
   virtual bool navTranslate(vjVec3 trans);
   virtual void navRotate(vjMatrix rot_mat);

private:
   float mTransVelocity, mMaxTransVelocity;
   float mRotVelocity, mMaxRotVelocity;

   navMode  mMode;

};

velocityNav::velocityNav()
{
   mRotVelocity = 0.0f;
   mMaxRotVelocity = 1.0f;
   mTransVelocity = 0.0f;
   mMaxTransVelocity = 5.0f;
   zeroTransVelocity();
   mMode = FLY;
}


void velocityNav::update(vjMatrix mat, bool allowTrans, bool allowRot)
{
   //cerr << "--------------------------------" << endl;
   //cerr << "velocityNav::update " << endl;

   // Get forward direction
   vjVec3   forward(0.0, 0.0, -1.0f);         // -Z is forward in VR Juggler
   forward.xformVec(mat, forward);
   vjVec3   gravity(0,-1,0);

   if(allowRot)
   {
      navRotate(mat);
   }
   if(mMode == GROUND)  // Do the gravity
   {
      if(navTranslate(gravity))  // Hit ground
      {;}
      else
      {;}
   }
   if(allowTrans)
   {
      vjVec3 move;
      move = (forward * mTransVelocity);

      if(navTranslate(forward))     // We hit
      {;}
      else
      {;}
   }

}

void velocityNav::navRotate(vjMatrix rot_mat)
{
   // I should scale the rotation matrix here
   // I should also compute a relative rotation from the head here as well
   vjMatrix    transform, transformIdent;
   vjQuat      source_rot, goal_rot, slerp_rot;

   transformIdent.makeIdent();            // Create an identity matrix to rotate from
   source_rot.makeQuat(transformIdent);
   goal_rot.makeQuat(rot_mat);                // Create the goal rotation quaternion

   if(transformIdent != rot_mat)              // If we don't have two identity matrices
   {
      slerp_rot.slerp(0.05, source_rot, goal_rot);    // Transform part way there
      transform.makeQuaternion(slerp_rot);            // Create the transform matrix to use
   }
   else
   {
      transform.makeIdent();
   }
   navigator::navRotate(transform);
}

bool velocityNav::navTranslate(vjVec3 trans)
{
   return navigator::navTranslate(trans);
}


void velocityNav::incTransVelocity()
{
   if(mTransVelocity < mMaxTransVelocity)
      mTransVelocity += 0.05f;
}

void velocityNav::zeroTransVelocity()
{
   mTransVelocity = 0.0f;
}
