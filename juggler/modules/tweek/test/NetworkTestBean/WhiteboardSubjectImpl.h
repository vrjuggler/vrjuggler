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
      : tweek::SubjectImpl()
   {
      /* Do nothing. */ ;
   }

   virtual ~WhiteboardSubjectImpl (void)
   {
      /* Do nothing. */ ;
   }

   virtual void addText(const char* text);

   virtual char* getAllText(void);

   WhiteboardSubject_ptr _this (void)
   {
      return WhiteboardSubject::_this();
   }

private:
   std::string m_text;
};

} // End of networktest namespace


#endif /* _SLIDER_SUBJECT_IMPL_H_ */
