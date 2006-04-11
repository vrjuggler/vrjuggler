#include <WhiteboardSubjectImpl.h>


namespace networktest
{

void WhiteboardSubjectImpl::insertText (CORBA::Long offset, CORBA::Long length,
                                        const char* text)
{
   mText.insert(offset, text);
   mLastOffset = offset;
   mLastLength = length;
   mLastChange = text;
   notify();
}

void WhiteboardSubjectImpl::getLastChange (CORBA::Long& offset,
                                           CORBA::Long& length,
                                           CORBA::String_out text)
{
   offset = mLastOffset;
   length = mLastLength;
   text   = CORBA::string_dup(mLastChange.c_str());
}

char* WhiteboardSubjectImpl::getAllText ()
{
   // XXX: The client is responsible for releasing this memory!
   return CORBA::string_dup(mText.c_str());
}

} // End networktest namespace
