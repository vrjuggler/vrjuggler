#include <vjConfig.h>
#include <Kernel/vjDrawManager.h>
#include <Kernel/vjDisplayManager.h>

void vjDrawManager::setDisplayManager(vjDisplayManager* _dispMgr)
{ displayManager = _dispMgr; }


ostream& operator<<(ostream& out, vjDrawManager& drawMgr)
{
   drawMgr.outStream(out);
   return out;
}
