#include <iostream>
#include <StringObserverImpl.h>


void StringObserverImpl::update()
{
   char* cur_value = mSubject->getValue();

   std::cout << "Current string value is now '" << cur_value << "'\n";
   delete cur_value;
}
