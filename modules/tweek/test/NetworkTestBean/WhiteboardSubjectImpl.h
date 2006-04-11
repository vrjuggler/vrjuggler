#ifndef _WHITEBOARD_SUBJECT_IMPL_H_
#define _WHITEBOARD_SUBJECT_IMPL_H_

#include <tweek/tweekConfig.h>

#include <string>
#include <vector>

#include <tweek/CORBA/SubjectImpl.h>
#include <WhiteboardSubject.h>


namespace networktest
{

/**
 */
class WhiteboardSubjectImpl : public POA_networktest::WhiteboardSubject,
                              public tweek::SubjectImpl
{
public:
   WhiteboardSubjectImpl (void)
      : tweek::SubjectImpl(), mText(""), mLastOffset(0), mLastLength(0),
        mLastChange("")
   {
      /* Do nothing. */ ;
   }

   virtual ~WhiteboardSubjectImpl (void)
   {
      /* Do nothing. */ ;
   }

   virtual void insertText(CORBA::Long offset, CORBA::Long length,
                           const char* text);

   virtual void getLastChange(CORBA::Long& offset, CORBA::Long& length,
                              CORBA::String_out text);

   virtual char* getAllText(void);

   WhiteboardSubject_ptr _this (void)
   {
      return POA_networktest::WhiteboardSubject::_this();
   }

private:
   std::string mText;
   CORBA::Long mLastOffset;
   CORBA::Long mLastLength;
   std::string mLastChange;
};

} // End of networktest namespace


#endif /* _WHITEBOARD_SUBJECT_IMPL_H_ */
