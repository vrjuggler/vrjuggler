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
class pfCollidor : public collidor
{
public:
   // ARGS: world - The node to start collision with
   //       it should be the one under the one being used for navigation ( ie. pfNaver)
   pfCollidor(pfNode* world)
   {
      mWorldNode = world;
      collideVolumeSetup(0x1);      // Setup the collision routines
   }

   virtual bool testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction);

protected:
   pfNode* mWorldNode;        // The world to collide with
};


bool pfCollidor::testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction)
{
   pfVec3 pf_cur_pos = vjGetPfVec(curPos);
   pfVec3 pf_delta = vjGetPfVec(delta);
   pfVec3 pf_correction;         // Needs to be set

   pfVec3 pf_new_pos;
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



#endif
