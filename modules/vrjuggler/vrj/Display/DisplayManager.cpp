/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


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
   std::vector<vjDisplay*> ret_val;
   ret_val.insert(ret_val.end(), mActiveDisplays.begin(), mActiveDisplays.end());
   ret_val.insert(ret_val.end(), mInactiveDisplays.begin(), mInactiveDisplays.end());
   return ret_val;
}


void vjDisplayManager::setDrawManager(vjDrawManager* drawMgr)
{
   vjDEBUG(vjDBG_DISP_MGR,3) << "vjDisplayManager: Setting draw manager.\n" << vjDEBUG_FLUSH;

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
//! POST: (display of same name already loaded) ==> old display closed, new one opened
//+       (display is new) ==> (new display is added)
//+       draw manager is notified of the display change
bool vjDisplayManager::configAdd(vjConfigChunk* chunk)
{
   vjASSERT(configCanHandle(chunk));      // We must be able to handle it first of all

   vjDEBUG_BEGIN(vjDBG_DISP_MGR,3) << "------- vjDisplayManager::configAdd -------\n" << vjDEBUG_FLUSH;

   // Find out if we already have a window of this name
   // If so, then close it before we open a new one of the same name
   // This basically allows re-configuration of a window
   vjDisplay* cur_disp = findDisplayNamed(chunk->getProperty("name"));
   if(cur_disp != NULL)                         // We have an old display
   {
      vjDEBUG(vjDBG_DISP_MGR,1) << "Removing old window: " << cur_disp->getName().c_str() << vjDEBUG_FLUSH;
      closeDisplay(cur_disp,true);              // Close the display and notify the draw manager to close the window
   }

   // --- Add a display (of the correct type) ---- //
   if((std::string)chunk->getType() == std::string("surfaceDisplay"))      // Surface DISPLAY
   {
      vjDisplay* newDisp = new vjSurfaceDisplay();    // Create display
      newDisp->config(chunk);                         // Config it
      addDisplay(newDisp, true);                            // Add it
      vjDEBUG(vjDBG_DISP_MGR,1) << "Adding display: " << newDisp->getName().c_str() << endl << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_DISP_MGR,4) << "Display: "  << newDisp << endl << vjDEBUG_FLUSH;
   }

   if((std::string)chunk->getType() == std::string("simDisplay"))      // Surface DISPLAY
   {
      vjDisplay* newDisp = new vjSimDisplay();     // Create display
      newDisp->config(chunk);                      // Config it
      addDisplay(newDisp, true);                         // Add it
      vjDEBUG(vjDBG_DISP_MGR,1) << "Adding Display: " << newDisp->getName().c_str() << endl << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_DISP_MGR,4) << "Display: "  << newDisp << endl << flush << vjDEBUG_FLUSH;
   }

   vjDEBUG_END(vjDBG_DISP_MGR,3) << "------- vjDisplayManager::configAdd Done. --------\n" << vjDEBUG_FLUSH;
   return true;
}

//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//!RETURNS: success
bool vjDisplayManager::configRemove(vjConfigChunk* chunk)
{
   vjASSERT(configCanHandle(chunk));      // We must be able to handle it first of all

   vjDEBUG_BEGIN(vjDBG_DISP_MGR,4) << "------- vjDisplayManager::configRemove -------\n" << vjDEBUG_FLUSH;

   bool success_flag(false);

   if((std::string)chunk->getType() == std::string("surfaceDisplay") ||
      (std::string)chunk->getType() == std::string("simDisplay"))      // It is a display
   {
      vjDisplay* remove_disp = findDisplayNamed(chunk->getProperty("name"));
      if(remove_disp != NULL)
      {
         closeDisplay(remove_disp, true);                            // Remove it
         success_flag = true;
      }
   }

   vjDEBUG_END(vjDBG_DISP_MGR,4) << "------- vjDisplayManager::configRemove done. --------\n" << vjDEBUG_FLUSH;
   return success_flag;
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
   vjDEBUG(vjDBG_DISP_MGR,4) << "vjDisplayManager::addDisplay \n" << vjDEBUG_FLUSH;

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

//: Close the given display
//! PRE: disp is a display we know about
//! POST: disp has been removed from the list of displays
//+   (notifyDrawMgr == true) && (drawMgr != NULL) && (disp is active)
//+   ==> Draw manager has been told to clode the window for the display
int vjDisplayManager::closeDisplay(vjDisplay* disp, bool notifyDrawMgr)
{
   vjASSERT(isMemberDisplay(disp));       // Make sure that display actually exists

   // Notify the draw manager to get rid of it
   if((notifyDrawMgr) && (mDrawManager != NULL) && (disp->isActive()))
      mDrawManager->removeDisplay(disp);

   // Remove it from local data structures
      int num_before_close = mActiveDisplays.size() + mInactiveDisplays.size();
   mActiveDisplays.erase(std::remove(mActiveDisplays.begin(), mActiveDisplays.end(), disp), mActiveDisplays.end());
   mInactiveDisplays.erase(std::remove(mInactiveDisplays.begin(), mInactiveDisplays.end(), disp), mInactiveDisplays.end());
      vjASSERT(num_before_close == (1+mActiveDisplays.size() + mInactiveDisplays.size()));

   // Delete the object
   // XXX: Memory leak.  Can't delete display here because the draw manager
   //      may need access to it when the draw manager closes the window up.
   // ex: the glDrawManager window has ref to the display to get current user, etc.
   //XXX//delete disp;

   return 1;
}


// Is the display a member of the display manager
bool vjDisplayManager::isMemberDisplay(vjDisplay* disp)
{
   std::vector<vjDisplay*>::iterator i;

   i = std::find(mActiveDisplays.begin(),mActiveDisplays.end(),disp);
   if(i != mActiveDisplays.end())
      return true;

   i = std::find(mInactiveDisplays.begin(),mActiveDisplays.end(),disp);
   if(i != mActiveDisplays.end())
      return true;

   return false;  // Didn't find any
}

//: Find a display given the display name
//! RETURNS: NULL - not found
vjDisplay* vjDisplayManager::findDisplayNamed(std::string name)
{
   std::vector<vjDisplay*>::iterator i;

   for(i = mActiveDisplays.begin();i!=mActiveDisplays.end();i++)
      if((*i)->getName() == name)
         return (*i);

   for(i = mInactiveDisplays.begin();i!=mInactiveDisplays.end();i++)
      if((*i)->getName() == name)
         return (*i);

   return NULL;  // Didn't find any
}


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


