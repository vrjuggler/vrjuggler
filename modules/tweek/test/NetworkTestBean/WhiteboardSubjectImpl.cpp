#include <WhiteboardSubjectImpl.h>


namespace networktest
{

void WhiteboardSubjectImpl::insertText (CORBA::Long offset, CORBA::Long length,
                                        const char* text)
{
   m_text.insert(offset, text);
   m_last_offset = offset;
   m_last_length = length;
   m_last_change = text;
   notify();
}

void WhiteboardSubjectImpl::getLastChange (CORBA::Long& offset,
                                           CORBA::Long& length,
                                           CORBA::String_out text)
{
   offset = m_last_offset;
   length = m_last_length;
   text   = CORBA::string_dup(m_last_change.c_str());
}

char* WhiteboardSubjectImpl::getAllText ()
{
   // XXX: The client is responsible for releasing this memory!
   return CORBA::string_dup(m_text.c_str());
}

} // End networktest namespace
