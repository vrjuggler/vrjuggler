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
   {;}

   void getPosition(CORBA::Float& xPos, CORBA::Float& yPos, CORBA::Float& zPos)
   {;}

   void getOrientation(CORBA::Float& heading, CORBA::Float& pitch,
                       CORBA::Float& roll)
   {;}

   void setVelocity(CORBA::Float xDelta, CORBA::Float yDelta,
                    CORBA::Float zDelta)
   {
      mNavigater->setVelocity(gmtl::Vec3f(xDelta, yDelta, zDelta));
   }

   void setRotationalVelocity(CORBA::Float headingDelta,
                              CORBA::Float pitchDelta, CORBA::Float rollDelta)
   {
      gmtl::EulerAngleXYZf euler( headingDelta, pitchDelta, rollDelta );
      gmtl::Matrix44f real = gmtl::makeRot<gmtl::Matrix44f>( euler ); 
      mNavigater->setRotationalVelocity(real);
   }

   void setWalkMode(CORBA::Boolean walk)
   {
   }

   vrj::RemoteNavSubject_ptr _this()
   {
      return POA_vrj::RemoteNavSubject::_this();
   }

private:
   OsgNavigater* mNavigater;
};


#endif /* _REMOTE_NAV_SUBJECT_IMPL_H_ */
