#ifndef _VJ_USER_H_
#define _VJ_USER_H_

#include <vjConfig.h>
#include <Config/vjConfigChunk.h>
#include <Input/InputManager/vjPosInterface.h>

//-----------------------------------------
//: Representation for Juggler user in multi-user environments
//
// Each user has a system assigned id as well
// as a string name from the user config chunk
// the created the user.  These ids can be
// used to identify a user at set points in
// a juggler application where user information
// is given.
// <br>
// The system assigned id number can also be useful to use as an index
// into a program array (or other data strucutre) that stores user
// specific data.  Ex. Navigation matrices, input devices
//---------------------------------------------
//! PUBLIC_API:
class vjUser
{
public:
   // Cosntruct the user
   vjUser() : mUserId(-1), mName("")
   {;}

   //: Get the id of the user in the system
   //! RETURNS: -1 => User has not been configured
   int getId();

   //: Get the name of the user object
   //! RETURNS: string name of the head
   string getName();

   //: Configure the user object
   //! POST: User has valid ids (int and string)
   //! POST: Positional device for user location has been set
   virtual bool config(vjConfigChunk* chunk);

   vjMatrix* getHeadPos()
   { return mHead->GetData(); }

private:
   int               mUserId;    //: the id of the user
   string            mName;      //: The string name of the user
   vjPosInterface    mHead;      //: The head positon

private:
   static int mNextUserId;     //: the next user id to assign
};



#endif
