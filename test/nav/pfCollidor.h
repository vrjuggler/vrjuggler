#include <collidor.h>
#include <collide.h>

#ifndef _PF_COLLIDOR_H_
#define _PF_COLLIDOR_H_

//: Collidor class for testing collisions in Performer
//
// This class will test collisions against a given world model
//
// NOTE: Remember that the navigation system is dealing
//     in Juggler (OpenGL) coordinates, not in Performer coords.
//     So there is some coord system conversion that will be
//     going on here.
class pfVolumeCollidor : public collidor
{
public:
   // ARGS: world - The node to start collision with
   //       it should be the one under the one being used for navigation ( ie. pfNaver)
   pfVolumeCollidor(pfNode* world)
   {
      mWorldNode = world;
      collideVolumeSetup(0x1);      // Setup the collision routines
   }

   //: test a movement
   //!ARGS: curPos - The current position we are at
   //!ARGS: delta - The amount we want to move
   //!ARGS: correction - The amount to correct the movement so that we do not collide
   //!RETURNS: true - There was a hit.
   virtual bool testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction, bool curPosWithDelta = false);

protected:
   pfNode* mWorldNode;        // The world to collide with
};


bool pfVolumeCollidor::testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction, bool curPosWithDelta)
{
   pfVec3 pf_cur_pos = vjGetPfVec(curPos);
   pfVec3 pf_delta = vjGetPfVec(delta);
   pfVec3 pf_correction;         // Needs to be set

   pfVec3 pf_new_pos;
   if(curPosWithDelta)
      pf_new_pos = pf_cur_pos;
   else
      pf_new_pos = (pf_cur_pos + pf_delta);

   float vol_radius = pf_delta.length();
   if(vol_radius < 1.0f)
      vol_radius = 1.0f;      // Always have at least unit length

   vol_radius *= 2.0;         // Terry hack

   cerr << "Vol Rad: " << vol_radius << endl;
   cerr << "Testing: " << vjGetVjVec(pf_new_pos) << endl;

   collideVolumeRadius(vol_radius);    // Setup collision volume

   if(collideVolume(pf_correction,mWorldNode,pf_new_pos))
   {
      correction = vjGetVjVec(pf_correction);

      setDidCollide(true);
      return true;
   }
   else
   {
      setDidCollide(false);
      return false;
   }


}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

//: Collidor class for testing collisions in Performer
//
// This class will test collisions against a given world model
//
// NOTE: Remember that the navigation system is dealing
//     in Juggler (OpenGL) coordinates, not in Performer coords.
//     So there is some coord system conversion that will be
//     going on here.
class pfRideCollidor : public collidor
{
public:
   // ARGS: world - The node to start collision with
   //       it should be the one under the one being used for navigation ( ie. pfNaver)
   pfRideCollidor(pfNode* world)
   {
      mWorldNode = world;
      collideVolumeSetup(0x1);      // Setup the collision routines
   }

   //: test a movement
   //!ARGS: curPos - The current position we are at
   //!ARGS: delta - The amount we want to move
   //!ARGS: correction - The amount to correct the movement so that we do not collide
   //!RETURNS: true - There was a hit.
   virtual bool testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction, bool curPosWithDelta = false);

protected:
   pfNode* mWorldNode;        // The world to collide with
};


bool pfRideCollidor::testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction, bool curPosWithDelta)
{
   pfVec3 pf_cur_pos = vjGetPfVec(curPos);
   pfVec3 pf_delta = vjGetPfVec(delta);
   pfVec3 pf_correction;         // Needs to be set

   float height(6.0f);

   pfVec3 pf_new_pos;
   pf_new_pos = (pf_cur_pos + pf_delta);
   float delta_mag = pf_delta.length();
   if(delta_mag > height)
      cerr << "ERROR: pfRideCollidor: Trying to move faster than the vector is checking.\n";

   // This function tests to make sure that for height straight down, there is no collision
   // from the position we are testing.
   // So I need to move that position up in the air to make "room" for the collision vector
   // XXX: This is because of hacks in the collision code
   pf_new_pos += pfVec3(0,0,height);

   if(collideGround(pf_correction,mWorldNode,0x1,pf_new_pos, height))
   {
      correction = vjGetVjVec(pf_correction);

      setDidCollide(true);
      return true;
   }
   else
   {
      setDidCollide(false);
      return false;
   }
}



#endif
