#ifndef _WHITEBOARD_SUBJECT_IMPL_H_
#define _WHITEBOARD_SUBJECT_IMPL_H_

#include <tweek/tweekConfig.h>

#include <string>
#include <vector>

#include <tweek/CORBA/SubjectImpl.h>
#include <tweek/CORBA/Observer.h>
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
      : tweek::SubjectImpl(), m_text(""), m_last_offset(0), m_last_length(0),
        m_last_change("")
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
      return WhiteboardSubject::_this();
   }

private:
   std::string m_text;
   CORBA::Long m_last_offset;
   CORBA::Long m_last_length;
   std::string m_last_change;
};

} // End of networktest namespace


#endif /* _SLIDER_SUBJECT_IMPL_H_ */
