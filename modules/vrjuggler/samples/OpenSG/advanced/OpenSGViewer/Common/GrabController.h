/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef GRABCONTROLLER_H
#define GRABCONTROLLER_H

#include <View/Controller.h>
#include <Common/HelperMacros.h>
#include <snx/SoundHandle.h>

SHARED_PTR_TYPE(GrabController);
REGISTER_CONTROLLER_ID("ce6f7303-1ca4-404a-bfbe-f13e2495566e", GrabController);

#include <Util/Debug.h>

// Get the grab controller info
#include <View/idl/GrabControllerData.h>

#include <dso/Pool/ObjectProxy.h>

// This define turns on drawing of debugging visuals for grab controllers
//#define DEBUG_DRAW_VIEW_CONTROLLER 1

/**
* Implements the entire MVC for a grabbable object
* Model: Current state of interaction
* View: What we look like in the SG
* Controller: How to handle user input
*/
class GrabController : public Controller
{
CONTROLLER_ID_MEMBERS(GrabController);

public:
   enum SelectionState { NotSelected=0, /**< The entity is not currently selected (no isect) */ 
                         IsSelected=1, /**< The entity is currently selected (does isect */
                         IsGrabbed=2, /**< The entity is selected AND is grabbed */ 
                         IsGrabbedAndLocked=3 /**< The entity is grabbed and it is locked locally */ };
   enum RemoteState  { UnSelected, DoesIsect, HoldsLock };
   
   /**< Used as index into the sound vector */
   enum SoundNames { LocalSelect = 0, LocalDeselect, LocalGrab, LocalUngrab, LocalLock,
                     RemoteSelect, RemoteDeselect, RemoteLock, RemoteUnlock, NumSounds = 9 };

public:
   GrabController()
      : mLocalSelectionState(NotSelected), mOldLocalSelectionState(NotSelected),
        mRemoteState(UnSelected), mOldRemoteState(UnSelected)
   {;}

   virtual ~GrabController()
   {;}

   /** Initialization method
   * Called by creator when the control should be initialized.
   * @pre Entity exists and is set for view
   * @post GFX specific data structures are created and integrated
   * @note Classes MUST call their parent class's init function first thing
   *       to guarantee correct initialization and dependencies satisfied.
   */
   virtual void init()
   {
      bool found_it(false);
      
      Controller::init();     // Call parent
      
      initSounds();

      initAndCreate(getObjId(), found_it);

      vprDEBUG(dspaceBUILDER,1) <<  "   Initialized grab controller: [" << getObjId().toString() 
                                << "] found=[" << (found_it?"true":"false") << "]\n" << vprDEBUG_FLUSH;

      initLocalGrabControllerData();      // Initialize the local grab controller that we have here
   }

   /** Initialize the sounds stuff. */
   void initSounds();

   // --- HACK FOR NOW: get interface for creation and initialization (copy/slash from Entity) --- //

   /** Intialize the entity after creation
   * @post Shared data internals will be found and hooked up in all classes in hierarchy
   * @param objData - Ptr to an allocated ObjectProxy that we should set equal to
   */
   virtual void initAndCreate(dso::GUID objId, bool& found, dso::ObjectManager* objMgr = NULL );

   /** Initialize the shared data member that is wrapped
   * @param objData - Object data to represent
   * @post Shared data internals will be hooked up in all classes in hierarchy
   */
   //virtual void init(dso::ObjectProxyBase* objData);

   /** Initialize the shared data with default values
   * Called upon local shared data construction to
   * setup the shared data with sane default values.
   */
   virtual void initSharedDataDefaults();

   void initLocalGrabControllerData();

   /** Update the grab state */
   virtual void preUpdate(Traverser* traverser);

   /** Template method to be filled in by gfx specific code
   * @param wandWorldPos - The wand in the virtual world coord system
   */
   virtual bool intersect(gmtl::Matrix44f wandWorldPos) = 0;

protected:  // -- Helpers for state management and shared data management --- //
   /** Add our grabber to the list of grabbers.
   * @post Our grabber is in list and it's values are set to default starting states
   */
   void addOurGrabberToList();

   /** Removes our grabber from the list of grabbers.
   */
   void removeOurGrabberFromList();

   /** Is our grabber in the list */
   bool isOurGrabberInList();

   gmtl::Matrix44f getAverageDesiredPosition();

protected:
   /** Matrices for tracking transformation of grabbing.
   * See GrabController::update for description
   */
   gmtl::Matrix44f   mXform_wand_M_o;     /**< Xfrom from the wand to the object */

#ifdef DEBUG_DRAW_VIEW_CONTROLLER
   // Debug matrices
   gmtl::Matrix44f   mDraw_xform_vm_M_o;      // Desired position of object
   gmtl::Matrix44f   mDraw_cur_xform_vm_M_o;  // Desired position of object
   gmtl::Matrix44f   mDraw_cur_xform_vm_M_n;  /// Desired position of object
   gmtl::Matrix44f   mDraw_xform_vw_M_wand;
   gmtl::Matrix44f   mDraw_xform_vw_M_plat;
#endif

   //bool  mSelected;  /**< Is the object currently selected (highlight it) */
   //bool  mGrabbed;   /**< Is the object currently grabbed */

   bool           mSelectStateChanged;       /**< True if the state changed. Needed for rendering and sound */   
   SelectionState mLocalSelectionState;      /**< The selection state for the local grabber */
   SelectionState mOldLocalSelectionState;   /**< Needed for some triggering of state changing events */

   RemoteState    mRemoteState;
   RemoteState    mOldRemoteState;

   // --- Shared data --- //
   dso::ObjectProxy<dspace::GrabControllerData>       mGrabControllerData;    /**< Shared data for the grab controller object */
   dso::ObjectProxy<dspace::GrabController::Grabber>  mLocalGrabber;          /**< The local grabber data */   

protected:
   std::vector<snx::SoundHandle> mSounds;

};

#endif //GRABCONTROLLER_H

