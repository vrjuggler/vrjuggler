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

#ifndef _VRJ_DISPLAY_MANAGER_H_
#define _VRJ_DISPLAY_MANAGER_H_

#include <vrj/vrjConfig.h>
#include <vector>

#include <vpr/Util/Singleton.h>

//#include <gadget/Type/Position.h>
#include <jccl/RTRC/ConfigElementHandler.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <vrj/Display/DisplayPtr.h>


namespace vrj
{

class DrawManager;
class Display;

/** \class DisplayManager DisplayManager.h vrj/Display/DisplayManager.h
 *
 * Singleton Container class for all Displays.
 *
 * PURPOSE:
 * This class is responsible for holding the data about display aspects
 * of the application.  The display object are window/system independant
 * this class is in charge of holding all the display data and keeping it
 * current.  This includes updating projections, adding/deleting new displays,
 * etc.
 *
 * @date 9-7-97
 */
class VJ_CLASS_API DisplayManager : public jccl::ConfigElementHandler
{
public:     // --- Config stuff -- //
   /**
    * Adds the element to the configuration.
    * @pre configCanHandle(element) == true
    */
   virtual bool configAdd(jccl::ConfigElementPtr element);

   /**
    * Removes the element from the current configuration.
    * @pre configCanHandle(element) == true
    */
   virtual bool configRemove(jccl::ConfigElementPtr element);

   /**
    * Can the handler handle the given configuration element?
    *
    * @return true if we can handle it, false if we can't.
    */
   virtual bool configCanHandle(jccl::ConfigElementPtr element);

public:
   /**
    * This function is used to update all managed projections at once.
    * @param scaleFactor - Current scale factor for position data
    */
   virtual void updateProjections(const float scaleFactor);

   /**
    * Sets the Draw Manager that the system is running.
    * We need to know this in order to notify the Draw Manager of any display
    * changes.
    *
    * @post Draw Manager is notified of any displays currently configured.
    */
   void setDrawManager(DrawManager* drawMgr);

   /**
    * Returns a list of the current displays.
    * @note DO NOT EDIT THE DISPLAYS
    */
   const std::vector<vrj::DisplayPtr>& getActiveDisplays() const
   {
      return mActiveDisplays;
   }

   /**
    * Returns list of inactive displays.
    * @note DO NOT EDIT THE DISPLAYS
    */
   const std::vector<vrj::DisplayPtr>& getInActiveDisplays() const
   {
      return mInactiveDisplays;
   }

   /**
    * Returns list of all displays (inactive and active).
    * @note DO NOT EDIT THE DISPLAYS
    */
   std::vector<vrj::DisplayPtr> getAllDisplays() const;

   jccl::ConfigElementPtr getDisplaySystemElement();

private:
   void setDisplaySystemElement(jccl::ConfigElementPtr elt);

   /**
    * Adds the element to the configuration.
    *
    * @pre configCanHandle(element) == true
    * @post (display of same name already loaded) ==> old display closed, new
    *       one opened<br>
    *       (display is new) ==> (new display is added)<br>
    *       Draw Manager is notified of the display change.
    */
   bool configAddDisplay(jccl::ConfigElementPtr element);

   /**
    * Removes the element from the current configuration.
    *
    * @pre configCanHandle(element) == true
    * @return success
    */
   bool configRemoveDisplay(jccl::ConfigElementPtr element);


   /**
    * Adds a display to the current system.
    *
    * @pre disp is a valid display
    * @post disp has been added to the list of displays
    *   (notifyDrawMgr == true) && (drawMgr != NULL) && (disp is active)
    *   ==> Draw manager now has been given new window to display
    */
   int addDisplay(vrj::DisplayPtr disp, bool notifyDrawMgr = true);

   /**
    * Closes the given display.
    * @pre disp is a display we know about
    * @post disp has been removed from the list of displays
    *    (notifyDrawMgr == true) && (drawMgr != NULL) && (disp is active)
    *    ==> Draw manager has been told to clode the window for the display
    */
   int closeDisplay(vrj::DisplayPtr disp, bool notifyDrawMgr = true);

   /// Is the display a member of the display manager
   bool isMemberDisplay(vrj::DisplayPtr disp);

   /**
    * Finds a display given the display name.
    * @return NULL if nothing found
    */
   vrj::DisplayPtr findDisplayNamed(const std::string& name);

private:
   std::vector<vrj::DisplayPtr> mActiveDisplays; /**< List of currently active displays */
   std::vector<vrj::DisplayPtr> mInactiveDisplays; /**< List of currently inactive displays */

protected:
   DrawManager*    mDrawManager;           /**< The current Draw Manager to communicate with */
   jccl::ConfigElementPtr mDisplaySystemElement; /**< Config element for the displaySystem */

protected:
   DisplayManager() : mDrawManager(NULL)
   {
      /* Do nothing. */ ;
   }

   virtual ~DisplayManager()
   {
      /* Do nothing. */ ;
   }

   DisplayManager(const DisplayManager& o)
      : jccl::ConfigElementHandler(o)
   {
      /* Do nothing. */ ;
   }

   void operator= (const DisplayManager&) {;}

   vprSingletonHeader( DisplayManager );
};

} // end namespace vrj


#endif
