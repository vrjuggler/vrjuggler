#include <tweek/CORBA/CorbaManager.h>
#include <vpr/Thread/Thread.h>


int main (int argc, char* argv[])
{
   tweek::CorbaManager mgr;

   mgr.init();

   while ( 1 )
   {
      vpr::Thread::msleep(100);
   }

   return 0;
}
