#include <Math/vjQuat.h>
#include <navigator.h>
#include <collidor.h>


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

   void setGravityCollidor(collidor* val) { mGravCollidor = val;}
   void setCorrectingCollidor(collidor* val) { mCorrectingCollidor = val;}

protected:
   // Translate and use gravity if it exists
   bool trans(vjVec3 trans);

   // check if we are hitting anything current, and correct for it
   bool correctPosition(vjVec3 prev_delta);

   // Rotate the environment
   void rotate(vjMatrix rot_mat);

private:
   float mTransVelocity, mMaxTransVelocity;
   float mRotVelocity, mMaxRotVelocity;

   collidor* mCorrectingCollidor;
   collidor* mGravCollidor;

   navMode  mMode;
};

velocityNav::velocityNav()
{
   mRotVelocity = 0.0f;
   mMaxRotVelocity = 1.0f;
   mTransVelocity = 0.0f;
   mMaxTransVelocity = 5.0f;
   zeroTransVelocity();
   mMode = GROUND;
}


void velocityNav::update(vjMatrix mat, bool allowTrans, bool allowRot)
{
   cerr << "--------------------------------" << endl;
   cerr << "velocityNav::update " << endl;

   // Get forward direction
   vjVec3   forward(0.0, 0.0, -1.0f);         // -Z is forward in VR Juggler
   forward.xformVec(mat, forward);
   vjVec3   gravity(0,-1,0);
   vjVec3 move(0.0f,0.0f,0.0f);

   if(allowRot)
   {
      rotate(mat);
   }
   if(mMode == GROUND)  // Do the gravity
   {
      move += gravity;
      cerr << "Added gravity: grav: " << gravity << " ==> move: " << move << endl;
   }
   if(allowTrans)
   {
      move += (forward * mTransVelocity);
      cerr << "Added trans: trans: " << forward << "*" << mTransVelocity << " ==> move: " << move << endl;
   }

   if(move != vjVec3(0,0,0))
   {
      if(trans(move))     // We hit
      {;}
      else
      {;}

      correctPosition(move);
   }

}

void velocityNav::rotate(vjMatrix rot_mat)
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


bool velocityNav::trans(vjVec3 trans)
{
   vjVec3 correction;
   vjCoord cur_pos(mCurPos);
   bool ret_val(false);

   if((mGravCollidor != NULL) && mGravCollidor->testMove(cur_pos.pos, trans, correction))
   {
      trans += correction;                // Correct the translation
      //trans += correction*(0.1/correction.length());   // Bounce a little too much
      ret_val = true;
      cerr << "------ TRANS HIT -------\n";
      cerr << "Correction: " << correction << endl;
      cerr << "Corrected:  " << trans << endl;
   }

   navigator::navTranslate(trans);
   if((mGravCollidor != NULL) && mGravCollidor->didCollide())        // Do some damping
      mTransVelocity *= 0.66f;      // We hit, so slow down the translation

   return ret_val;
}

// check if we are hitting anything current, and correct for it
bool velocityNav::correctPosition(vjVec3 prev_delta)
{
   vjVec3 zero_trans(0.0f,0.0f,0.0f);
   vjVec3 correction;
   vjCoord cur_pos(mCurPos);

   if((mCorrectingCollidor != NULL) &&
      mCorrectingCollidor->testMove(cur_pos.pos, prev_delta, correction,true))
   {
      cerr << "-------- CORRECTION HIT --------\n";
      cerr << "Correction: " << correction << endl;
      navigator::navTranslate(correction);
      return true;
   }
   return false;
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
