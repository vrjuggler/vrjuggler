#ifndef _REMOTE_NAV_SUBJECT_IMPL_H_
#define _REMOTE_NAV_SUBJECT_IMPL_H_

#include <tweek/CORBA/SubjectImpl.h>
#include <nav.h>
#include <RemoteNavSubject.h>

class RemoteNavSubjectImpl
   : public POA_vrj::RemoteNavSubject
   , public tweek::SubjectImpl
{
public:
   RemoteNavSubjectImpl(OsgNavigater* nav) : mNavigater(nav)
   {;}

   virtual ~RemoteNavSubjectImpl()
   {;}

   void setPosition(CORBA::Float xPos, CORBA::Float yPos, CORBA::Float zPos)
   {
      /*
      gmtl::Matrix44f temp_pos = mNavigater->getCurPos();
      gmtl::setTrans(temp_pos, gmtl::Vec3f(xPos, yPos, zPos));
      mNavigater->setCurPos(temp_pos);
      */
   }

   void getPosition(CORBA::Float& xPos, CORBA::Float& yPos, CORBA::Float& zPos)
   {
      /*
      gmtl::Vec3f pos = gmtl::makeTrans<gmtl::Vec3f>(mNavigater->getCurPos());
      xPos = pos[0];
      yPos = pos[1];
      zPos = pos[2];
      */
      xPos = 0;
      yPos = 0;
      zPos = 0;
   }

   void getOrientation(CORBA::Float& pitch, CORBA::Float& heading,
                       CORBA::Float& roll)
   {
      /*
      gmtl::Matrix44f pos = mNavigater->getCurPos();
      gmtl::EulerAngleXYZf euler;
      gmtl::set(euler,pos);
      pitch = euler[0];
      heading = euler[1];
      roll = euler[2];
      //pitch = gmtl::makeXRot(pos);
      //heading = gmtl::makeYRot(pos);
      //roll = gmtl::makeZRot(pos);
      */
      pitch = 0;
      heading = 0;
      roll = 0;
   }

   void setOrientation(CORBA::Float pitch, CORBA::Float heading,
                       CORBA::Float roll)
   {
      /*
      gmtl::EulerAngleXYZf euler( pitch, heading, roll );
      gmtl::Matrix44f pos = mNavigater->getCurPos();
      gmtl::setRot(pos, euler);
      mNavigater->setCurPos(pos);
      */
   }

   void setVelocity(CORBA::Float xDelta, CORBA::Float yDelta,
                    CORBA::Float zDelta)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Getting Trans Vel: "
                        << "X: " << xDelta
                        << "Y: " << yDelta
                        << "Z: " << zDelta << std::endl << vprDEBUG_FLUSH;
      mNavigater->setVelocity(gmtl::Vec3f(xDelta, yDelta, zDelta));
   }

   void setRotationalVelocity(CORBA::Float pitchDelta,
                              CORBA::Float headingDelta, CORBA::Float rollDelta)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Getting Rotation Vel: "
                        << "X: " << pitchDelta
                        << "Y: " << headingDelta
                        << "Z: " << rollDelta << std::endl << vprDEBUG_FLUSH;
      gmtl::EulerAngleXYZf euler( pitchDelta, headingDelta, rollDelta );
      gmtl::Matrix44f real = gmtl::makeRot<gmtl::Matrix44f>( euler );
      mNavigater->setRotationalVelocity(real);
   }

   void setWalkMode(CORBA::Boolean walk)
   {
      mNavigater->setWalkMode(walk);
   }

   vrj::RemoteNavSubject_ptr _this()
   {
      return POA_vrj::RemoteNavSubject::_this();
   }

private:
   OsgNavigater* mNavigater;
};


#endif /* _REMOTE_NAV_SUBJECT_IMPL_H_ */
