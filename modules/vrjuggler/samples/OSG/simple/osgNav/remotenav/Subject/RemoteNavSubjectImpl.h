#ifndef _REMOTE_NAV_SUBJECT_IMPL_H_
#define _REMOTE_NAV_SUBJECT_IMPL_H_

#include <tweek/CORBA/Subject.h>

class RemoteNavSubjectImpl
   : POA_vrj::RemoteNavSubject
   , tweek::SubjectImpl
{
public:
   RemoteNavSubjectImpl();

   virtual ~RemoteNavSubjectImpl()
   {;}

   void setPosition(CORBA::Float xPos, CORBA::Float yPos, CORBA::Float zPos);

   void getPosition(CORBA::Float& xPos, CORBA::Float& yPos, CORBA::Float& zPos);

   void getOrientation(CORBA::Float& heading, CORBA::Float& pitch,
                       CORBA::Float& roll);

   void setVelocity(CORBA::Float xDelta, CORBA::Float yDelta,
                    CORBA::Float zDelta);

   void setRotationalVelocity(CORBA::Float headingDelta,
                              CORBA::Float pitchDelta, CORBA::Float rollDelta);

   void setWalkMode(CORBA::Boolean walk);

   vrj::RemoteNavSubject_ptr _this()
   {
      return POA_vrj::RemoveNavSubject::_this();
   }
};


#endif /* _REMOTE_NAV_SUBJECT_IMPL_H_ */
