#include <Kernel/vjApp.h>
#include <Kernel/vjKernel.h>

vjApp::vjApp(vjKernel* kern)
{
   vjASSERT(kern != NULL);    // We don't want a NULL Kernel
   kernel = kern;
   mHaveFocus = true;
}

vjApp::vjApp()
{
   kernel = vjKernel::instance();
   mHaveFocus = true;
}
