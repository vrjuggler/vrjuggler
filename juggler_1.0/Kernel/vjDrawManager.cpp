#include <vjConfig.h>
#include <Kernel/vjDrawManager.h>
#include <Kernel/vjDisplayManager.h>

void vjDrawManager::setDisplayManager(vjDisplayManager* _dispMgr)
{ displayManager = _dispMgr; }
    
    
void vjDrawManager::updateProjections()
{
    displayManager->updateProjections();
	// NOTE: API Specific stuff should be done here
	//		in derived classes
	//  Like setting up of view frustum params
}
    
