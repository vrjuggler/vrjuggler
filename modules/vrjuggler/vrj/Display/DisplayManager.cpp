/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>
#include <boost/concept_check.hpp>
#include <jccl/Config/ConfigElement.h>
#include <jccl/RTRC/ConfigManager.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Display.h>
#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Draw/DrawManager.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Display/DisplayManager.h>


namespace vrj
{

vprSingletonImp(DisplayManager);

std::vector<DisplayPtr> DisplayManager::getAllDisplays() const
{
   std::vector<DisplayPtr> ret_val;
   ret_val.insert(ret_val.end(), mActiveDisplays.begin(), mActiveDisplays.end());
   ret_val.insert(ret_val.end(), mInactiveDisplays.begin(), mInactiveDisplays.end());
   return ret_val;
}

jccl::ConfigElementPtr DisplayManager::getDisplaySystemElement()
{
   jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

   if ( mDisplaySystemElement.get() == NULL )
   {
      cfg_mgr->lockActive();
      {
         std::vector<jccl::ConfigElementPtr>::iterator i;
         for (i = cfg_mgr->getActiveBegin() ; i != cfg_mgr->getActiveEnd() ; ++i)
         {
            if ( (*i)->getID() == std::string("display_system") )
            {
               setDisplaySystemElement(*i);
               break;         // This guarantees that we get the first display_system element.
            }
         }
      }
      cfg_mgr->unlockActive();
   }

   // XXX: We should never have to ask the pending list for the display_system element because
   //      it should always be loaded first, and already in the active list. If for some reason
   //      we have to look at the pending list it is possible to have a deadlock. This happens
   //      when we call DisplaySystem::getDisplaySystemElement() while we are configuring anything
   //      device in the system.
   // XXX: This could be fixed in the furute using better dependancy managment so that we can ensure
   //      that the display_system is always in the active list before configuring anythin that
   //      calls this function while getting configured.

   // If display_system element was not found in the active list.
   if ( mDisplaySystemElement.get() == NULL )
   {   
      cfg_mgr->lockPending();
      {
         std::list<jccl::ConfigManager::PendingElement>::iterator i;
         for (i = cfg_mgr->getPendingBegin() ; i != cfg_mgr->getPendingEnd() ; ++i)
         {
            if ( (*i).mElement->getID() == std::string("display_system") )
            {
               setDisplaySystemElement((*i).mElement);
               break;         // This guarantees that we get the first display_system element.
            }
         }
      }
      cfg_mgr->unlockPending();
//      vprASSERT(mDisplaySystemElement.get() != NULL && "No Display Manager config element found!");
   }

   return mDisplaySystemElement;
}

void DisplayManager::setDisplaySystemElement(jccl::ConfigElementPtr elt)
{
   if ( elt->getVersion() < 3 )
   {
      vprDEBUG(vrjDBG_DISP_MGR, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING") << ": Display system element '"
         << elt->getName() << "'" << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DISP_MGR, vprDBG_WARNING_LVL)
         << "         is out of date.\n";
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
         << "         Expected version 3 but found version "
         << elt->getVersion() << ".  Pipe\n";
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
         << "         configurations will not work.\n" << vprDEBUG_FLUSH;
   }

   mDisplaySystemElement = elt;
}

void DisplayManager::setDrawManager(DrawManager* drawMgr)
{
   vprDEBUG(vrjDBG_DISP_MGR, vprDBG_STATE_LVL)
      << "vrj::DisplayManager: Setting draw manager.\n" << vprDEBUG_FLUSH;

   // set the draw manager
   mDrawManager = drawMgr;

   // Alert the draw manager about all the active windows currently configured
   if (mDrawManager != NULL)
   {
      for (unsigned int i=0;i<mActiveDisplays.size();i++)
      {
         mDrawManager->addDisplay(mActiveDisplays[i]);
      }
   }
}

/**
 * Adds the element to the configuration.
 * @pre configCanHandle(element) == true.
 */
bool DisplayManager::configAdd(jccl::ConfigElementPtr element)
{
   vprASSERT(configCanHandle(element));

   const std::string element_type(element->getID());

   if ( (element_type == std::string("surfaceDisplay")) ||
        (element_type == std::string("simDisplay")) )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Element of type: " << element_type
         << " is no longer supported.  Use display_window type instead.\n"
         << vprDEBUG_FLUSH;
      return false;
   }
   else if ( (element_type == std::string("display_window")))
   {
      return configAddDisplay(element);
   }
   else if (element_type == std::string("display_system"))
   {
      // XXX: Put signal here to tell draw manager to lookup new stuff
      mDisplaySystemElement = element; // Keep track of the display system element
      return true;                     // We successfully configured.
                                       // This tell processPending to add it to the active config
   }

   return false;
}

/**
 * Removes the element from the current configuration.
 * @pre configCanHandle(element) == true
 */
bool DisplayManager::configRemove(jccl::ConfigElementPtr element)
{
   vprASSERT(configCanHandle(element));

   const std::string element_type(element->getID());

   if ( (element_type == std::string("surfaceDisplay")) ||
        (element_type == std::string("simDisplay")) )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Element of type: " << element_type
         << " is no longer supported.  Use display_window type instead.\n"
         << vprDEBUG_FLUSH;
      return false;
   }
   else if (element_type == std::string("display_window"))
   {
      return configRemoveDisplay(element);
   }
   else if (element_type == std::string("display_system"))
   {
      // XXX: Put signal here to tell draw manager to lookup new stuff
      mDisplaySystemElement.reset();   // Keep track of the display system element
      return true;                     // We successfully configured.
                                       // This tell processPending to remove it to the active config
   }
   else
   {
      return false;
   }

}


/**
 * Is it a display configuration element?
 *
 * @return true if we have a display config element; false if we don't.
 */
bool DisplayManager::configCanHandle(jccl::ConfigElementPtr element)
{
   return (    (element->getID() == std::string("surfaceDisplay"))
            || (element->getID() == std::string("simDisplay"))
            || (element->getID() == std::string("display_system"))
            || (element->getID() == std::string("display_window"))
           );
}

/**
 * Adds the element to the configuration.
 *
 * @pre configCanHandle(element) == true
 * @post (display of same name already loaded) ==> old display closed, new one opened<br>
 *       (display is new) ==> (new display is added)<br>
 *       Draw Manager is notified of the display change.
 */
bool DisplayManager::configAddDisplay(jccl::ConfigElementPtr element)
{
   vprASSERT(configCanHandle(element)); // We must be able to handle it first of all

   vprDEBUG_BEGIN(vrjDBG_DISP_MGR,vprDBG_STATE_LVL) << "------- DisplayManager::configAddDisplay -------\n" << vprDEBUG_FLUSH;

   // Find out if we already have a window of this name
   // If so, then close it before we open a new one of the same name
   // This basically allows re-configuration of a window
   DisplayPtr cur_disp = findDisplayNamed(element->getName());
   if (cur_disp != NULL)                         // We have an old display
   {
      vprDEBUG(vrjDBG_DISP_MGR,vprDBG_CONFIG_LVL) << "Removing old window: " << cur_disp->getName().c_str() << vprDEBUG_FLUSH;
      closeDisplay(cur_disp,true);              // Close the display and notify the draw manager to close the window
   }

   // --- Add a display (of the correct type) ---- //
   if (element->getID() == std::string("display_window"))       // Display window
   {
      DisplayPtr newDisp = Display::create();      // Create the display
      newDisp->config(element);
      addDisplay(newDisp,true);                    // Add it
      vprDEBUG(vrjDBG_DISP_MGR,vprDBG_STATE_LVL) << "Adding display: " << newDisp->getName().c_str() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(vrjDBG_DISP_MGR,vprDBG_STATE_LVL) << "Display: "  << newDisp << std::endl << vprDEBUG_FLUSH;
   }

   vprDEBUG_END(vrjDBG_DISP_MGR,vprDBG_STATE_LVL) << "------- DisplayManager::configAddDisplay Done. --------\n" << vprDEBUG_FLUSH;
   return true;
}

/**
 * Removes the element from the current configuration.
 *
 * @pre configCanHandle(element) == true
 * @return success
 */
bool DisplayManager::configRemoveDisplay(jccl::ConfigElementPtr element)
{
   vprASSERT(configCanHandle(element)); // We must be able to handle it first of all

   vprDEBUG_BEGIN(vrjDBG_DISP_MGR,vprDBG_STATE_LVL) << "------- DisplayManager::configRemoveDisplay -------\n" << vprDEBUG_FLUSH;

   bool success_flag(false);

   if (element->getID() == std::string("display_window"))      // It is a display
   {
      DisplayPtr remove_disp = findDisplayNamed(element->getName());
      if (remove_disp != NULL)
      {
         closeDisplay(remove_disp, true);                            // Remove it
         success_flag = true;
      }
   }

   vprDEBUG_END(vrjDBG_DISP_MGR,vprDBG_STATE_LVL) << "------- DisplayManager::configRemoveDisplay done. --------\n" << vprDEBUG_FLUSH;
   return success_flag;
}

// notifyDrawMgr = 0; Defaults to 0
int DisplayManager::addDisplay(DisplayPtr disp, bool notifyDrawMgr)
{
   vprDEBUG(vrjDBG_DISP_MGR, vprDBG_VERB_LVL)
      << "vrj::DisplayManager::addDisplay()\n" << vprDEBUG_FLUSH;

   // Test if active or not, to determine correct list
   // The place it in the list
   // --- Update Local Display structures
   if (disp->isActive())
   {
      mActiveDisplays.push_back(disp);
   }
   else
   {
      mInactiveDisplays.push_back(disp);
   }

   // If we are supposed to notify about, and valid draw mgr, and disp is active
   if ((notifyDrawMgr) && (mDrawManager != NULL) && (disp->isActive()))
   {
      mDrawManager->addDisplay(disp);;    // Tell Draw Manager to add dislay;
   }

   return 1;
}

/**
 * Closes the given display.
 *
 * @pre disp is a display we know about.
 * @post disp has been removed from the list of displays
 *    (notifyDrawMgr == true) && (drawMgr != NULL) && (disp is active)
 *    ==> Draw manager has been told to clode the window for the display
 */
int DisplayManager::closeDisplay(DisplayPtr disp, bool notifyDrawMgr)
{
   vprASSERT(isMemberDisplay(disp));       // Make sure that display actually exists

   vprDEBUG(vrjDBG_DISP_MGR, vprDBG_STATE_LVL)
      << "[vrj::DisplayManager::closeDisplay()] Closing display named '"
      << disp->getName() << "'" << std::endl << vprDEBUG_FLUSH;

   // Notify the draw manager to get rid of it
   // Note: if it is not active, then the draw manager doesn't know about it
   if ((notifyDrawMgr) && (mDrawManager != NULL) && (disp->isActive()))
   {
      mDrawManager->removeDisplay(disp);
   }

   // Remove it from local data structures
   size_t num_before_close = mActiveDisplays.size() + mInactiveDisplays.size();
   mActiveDisplays.erase( std::remove(mActiveDisplays.begin(), mActiveDisplays.end(), disp),
                          mActiveDisplays.end());
   mInactiveDisplays.erase( std::remove(mInactiveDisplays.begin(), mInactiveDisplays.end(), disp),
                            mInactiveDisplays.end());
   vprASSERT(num_before_close == (1+mActiveDisplays.size() + mInactiveDisplays.size()));
   boost::ignore_unused_variable_warning(num_before_close);

   return 1;
}

// Is the display a member of the display manager
bool DisplayManager::isMemberDisplay(DisplayPtr disp)
{
   std::vector<DisplayPtr>::iterator i;

   i = std::find(mActiveDisplays.begin(),mActiveDisplays.end(),disp);
   if (i != mActiveDisplays.end())
   {
      return true;
   }

   i = std::find(mInactiveDisplays.begin(),mInactiveDisplays.end(),disp);
   if (i != mInactiveDisplays.end())
   {
      return true;
   }

   return false;  // Didn't find any
}

/**
 * Finds a display given the display name.
 * @return NULL if nothing found
 */
DisplayPtr DisplayManager::findDisplayNamed(const std::string& name)
{
   std::vector<DisplayPtr>::iterator i;

   for (i = mActiveDisplays.begin();i!=mActiveDisplays.end();i++)
   {
      if ((*i)->getName() == name)
      {
         return (*i);
      }
   }

   for (i = mInactiveDisplays.begin();i!=mInactiveDisplays.end();i++)
   {
      if ((*i)->getName() == name)
      {
         return (*i);
      }
   }

   return vrj::DisplayPtr();  // Didn't find any
}

void DisplayManager::updateProjections(const float scaleFactor)
{
   // for (all displays) update the projections
   for (std::vector<DisplayPtr>::iterator i = mActiveDisplays.begin();
        i != mActiveDisplays.end(); i++)
   {
      (*i)->updateProjections(scaleFactor);
   }

   for (std::vector<DisplayPtr>::iterator j = mInactiveDisplays.begin();
        j != mInactiveDisplays.end(); j++)
   {
      (*j)->updateProjections(scaleFactor);
   }
}

}
