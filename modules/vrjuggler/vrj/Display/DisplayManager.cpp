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
{
   vjDEBUG(vjDBG_ALL,0) << "vjDisplayManager::setDrawManager: Entered.\n" << vjDEBUG_FLUSH;

   // set the draw manager
   mDrawManager = drawMgr;

   // Alert the draw manager about all the active windows currently configured
   if(mDrawManager != NULL)
   {
      for(int i=0;i<mActiveDisplays.size();i++)
      {
         mDrawManager->addDisplay(mActiveDisplays[i]);
      }
   }
}

//: Add the chunk to the configuration
//! PRE: configCanHandle(chunk) == true
//! RETURNS: success
bool vjDisplayManager::configAdd(vjConfigChunk* chunk)
{
   vjASSERT(configCanHandle(chunk));      // We must be able to handle it first of all

   vjDEBUG_BEGIN(vjDBG_ALL,1) << "------- vjDisplayManager::configAdd() Entering -------\n" << vjDEBUG_FLUSH;


   if((std::string)chunk->getType() == std::string("surfaceDisplay"))      // Surface DISPLAY
   {
      vjDisplay* newDisp = new vjSurfaceDisplay();    // Create display
      newDisp->config(chunk);                         // Config it
      addDisplay(newDisp, true);                            // Add it
      vjDEBUG(vjDBG_ALL,1) << "Display: "  << *newDisp << endl << flush << vjDEBUG_FLUSH;
   }

   if((std::string)chunk->getType() == std::string("simDisplay"))      // Surface DISPLAY
   {
      vjDisplay* newDisp = new vjSimDisplay();     // Create display
      newDisp->config(chunk);                      // Config it
      addDisplay(newDisp);                         // Add it
      vjDEBUG(vjDBG_ALL,1) << "Display: "  << *newDisp << endl << flush << vjDEBUG_FLUSH;
   }

   vjDEBUG_END(vjDBG_ALL,1) << "------- vjDisplayManager::configAdd() Exiting --------\n" << vjDEBUG_FLUSH;
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
   vjDEBUG(vjDBG_ALL,0) << "vjDisplayManager::addDisplay: Entered.\n" << vjDEBUG_FLUSH;

   // Test if active or not, to determine correct list
   // The place it in the list
   // --- Update Local Display structures
   if(disp->isActive())
      mActiveDisplays.push_back(disp);
   else
      mInactiveDisplays.push_back(disp);

   // If we are supposed to notify about, and valid draw mgr, and disp is active
   if((notifyDrawMgr) && (mDrawManager != NULL) && (disp->isActive()))
      mDrawManager->addDisplay(disp);;    // Tell Draw Manager to add dislay;

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

   if(mDrawManager != NULL)
      mDrawManager->updateProjections();
}


