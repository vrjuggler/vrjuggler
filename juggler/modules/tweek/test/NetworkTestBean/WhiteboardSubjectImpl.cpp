#include <WhiteboardSubjectImpl.h>


namespace networktest
{

void WhiteboardSubjectImpl::addText (const char* text)
{
   m_text.append(text);
   notify();
}

char* WhiteboardSubjectImpl::getAllText ()
{
   // XXX: The client is responsible for releasing this memory!
   return CORBA::string_dup(m_text.c_str());
}

} // End networktest namespace
