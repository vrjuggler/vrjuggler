#include <vjConfig.h>
#include <Kernel/vjDisplayManager.h>
#include <Kernel/vjDisplay.h>
#include <Kernel/vjSurfaceDisplay.h>
#include <Kernel/vjSimDisplay.h>
#include <Kernel/vjDrawManager.h>
#include <Kernel/vjKernel.h>
#include <Math/vjCoord.h>

vjDisplayManager* vjDisplayManager::_instance = NULL;

std::vector<vjDisplay*> vjDisplayManager::getAllDisplays()
{
   std::vector<vjDisplay*> ret_val = mActiveDisplays;
   ret_val.insert(ret_val.end(), mInactiveDisplays.begin(), mInactiveDisplays.end());
   return ret_val;
}


void vjDisplayManager::setDrawManager(vjDrawManager* drawMgr)
{ mDrawManager = drawMgr; }

//: Add the chunk to the configuration
//! PRE: configCanHandle(chunk) == true
//! RETURNS: success
bool vjDisplayManager::configAdd(vjConfigChunk* chunk)
{
   vjASSERT(configCanHandle(chunk));      // We must be able to handle it first of all

   vjDEBUG_BEGIN(1) << "------- vjDisplayManager::configAdd() Entering -------\n" << vjDEBUG_FLUSH;


   if((std::string)chunk->getType() == std::string("surfaceDisplay"))      // Surface DISPLAY
   {
      vjDisplay* newDisp = new vjSurfaceDisplay();    // Create display
      newDisp->config(chunk);                         // Config it
      addDisplay(newDisp);                            // Add it
      vjDEBUG(1) << "Display: "  << *newDisp << endl << flush << vjDEBUG_FLUSH;
   }

   if((std::string)chunk->getType() == std::string("simDisplay"))      // Surface DISPLAY
   {
      vjDisplay* newDisp = new vjSimDisplay();     // Create display
      newDisp->config(chunk);                      // Config it
      addDisplay(newDisp);                         // Add it
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
   return ((((std::string)chunk->getType()) == "surfaceDisplay")
            || (((std::string)chunk->getType()) == "simDisplay"));
}



// notifyDrawMgr = 0; Defaults to 0
int vjDisplayManager::addDisplay(vjDisplay* disp, bool notifyDrawMgr)
{
   //XXX: When we add this back in, we have to deal wiht several issues
   // - drawmanager needs to be known by the time we get here
   // - If it is not (because kernel has no app) then we just store the windows
   // - If it is, then pass the window on
   // - There is a lot of kernel, app, draw manager going on here that needs ironed out
   //vjASSERT(mDrawManager != NULL);     // If draw mgr is null, then we can't update it.

   // Test if active or not, to determine correct list
   // The place it in the list
   if(disp->isActive())
      mActiveDisplays.push_back(disp);
   else
      mInactiveDisplays.push_back(disp);

   // --- Update Local Display structures
   //Open new window object;
   //Assign it the correct size,  position,  and system specific data???;
   //Place it in the vector;

   //if(notifyDrawMgr)
   //    Tell Draw Manager to add dislay;

   return 1;
}

/*
int vjDisplayManager::closeDisplay(int dispId)
{
   //Tell draw manager to kill Display;
   //Update local data structures;

   return 1;
}
*/

/*
vjDisplay* vjDisplayManager::getDisplay(int dispId)
{
    return mDisplays[dispId];
}
*/

void vjDisplayManager::updateProjections()
{
   // for (all displays) update the projections
   for (std::vector<vjDisplay*>::iterator i = mActiveDisplays.begin(); i != mActiveDisplays.end(); i++)
      (*i)->updateProjections();

   for (std::vector<vjDisplay*>::iterator j = mInactiveDisplays.begin(); j != mInactiveDisplays.end(); j++)
      (*j)->updateProjections();
}


