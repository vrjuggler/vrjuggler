#include <vjConfig.h>
#include <Kernel/vjDisplayManager.h>
#include <Kernel/vjDisplay.h>
#include <Kernel/vjDrawManager.h>
#include <Kernel/vjKernel.h>
#include <Math/vjCoord.h>

vjDisplayManager* vjDisplayManager::_instance = NULL;

void vjDisplayManager::setDrawManager(vjDrawManager* drawMgr)
{ mDrawManager = drawMgr; }

//: Add the chunk to the configuration
//! PRE: configCanHandle(chunk) == true
//! RETURNS: success
bool vjDisplayManager::configAdd(vjConfigChunk* chunk)
{
   vjASSERT(configCanHandle(chunk));      // We must be able to handle it first of all

   vjDEBUG_BEGIN(1) << "------- vjDisplayManager::configAdd() Entering -------\n" << vjDEBUG_FLUSH;


   if((string)(char*)chunk->getType() == string("display"))      // DISPLAY
   {
      vjDisplay* newDisp = new vjDisplay();     // Create display
      newDisp->config(chunk);                   // Config it
      addDisplay(newDisp);      // Add it

      vjDEBUG(1) << "Display: "  << *newDisp << endl << flush << vjDEBUG_FLUSH;
   }

   vjDEBUG_END(1) << "------- vjDisplayManager::configAdd() Exiting --------\n" << vjDEBUG_FLUSH;
   return true;
}

//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//!RETURNS: success
bool vjDisplayManager::configRemove(vjConfigChunk* chunk)
{
   return true;
}

//: Is it a display chunk?
//! RETURNS: true - We have a display chunk
//+          false - We don't
bool vjDisplayManager::configCanHandle(vjConfigChunk* chunk)
{
   return (((string)(char*)chunk->getType()) == string("display"));
}



// notifyDrawMgr = 0; Defaults to 0
int vjDisplayManager::addDisplay(vjDisplay* disp, bool notifyDrawMgr)
{
   vjASSERT(mDrawManager != NULL);     // If draw mgr is null, then we can't update it.

   // For now just do this
   mDisplays.push_back(disp);

   // --- Update Local Display structures
   //Open new window object;
   //Assign it the correct size,  position,  and system specific data???;
   //Place it in the vector;

   //if(notifyDrawMgr)
   //    Tell Draw Manager to add dislay;

   return 1;
}

int vjDisplayManager::closeDisplay(int dispId)
{
   //Tell draw manager to kill Display;
   //Update local data structures;

   return 1;
}

vjDisplay* vjDisplayManager::getDisplay(int dispId)
{
    return mDisplays[dispId];
}

void vjDisplayManager::updateProjections()
{
   // for (all displays) update the projections
   for (vector<vjDisplay*>::iterator i = mDisplays.begin(); i != mDisplays.end(); i++)
      (*i)->updateProjections();
}


