#ifndef HOVER_SELECTION_METHOD_H_
#define HOVER_SELECTION_METHOD_H_

#include <Common/SelectionMethod.h>

class HoverSelectionMethod : public SelectionMethod
{
public:
   virtual OSG::NodePtr checkSelection(OpenSGViewer* viewer)
   {
      std::vector<OSG::NodePtr> selectable_nodes = viewer->getSelectableNodes();
   }

   /** Initialization method
   * Called by creator when the control should be initialized.
   */
   virtual void init()
   {
      initLocalGrabControllerData();      // Initialize the local grab controller that we have here
   }

   void initLocalGrabControllerData();

   /** Update the grab state */
   virtual void preUpdate(Traverser* traverser);

   /** Template method to be filled in by gfx specific code
   * @param wandWorldPos - The wand in the virtual world coord system
   */
   virtual bool intersect(gmtl::Matrix44f wandWorldPos) = 0;

   bool intersect(OSG::NodePtr node, gmtl::Matrix44f wandWorldPos);

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

   bool           mSelectStateChanged;       /**< True if the state changed. Needed for rendering and sound */   
   SelectionState mLocalSelectionState;      /**< The selection state for the local grabber */
   SelectionState mOldLocalSelectionState;   /**< Needed for some triggering of state changing events */
};

void GrabController::initAndCreate(dso::GUID objId, bool& found, dso::ObjectManager* objMgr)
{
   vprASSERT(objId == getObjId());
   vprASSERT(objId != vpr::GUID::NullGUID);

   bool was_constructed, found_remotely;
   mGrabControllerData.init(objId, true, objMgr, found_remotely, was_constructed);     // Initialize the entity object
   found = !was_constructed;

   vprASSERT( (found || was_constructed) && "Initialized failed???");

   // Call to parent class init method
   //Entity::init(&mGroupEntityData);

   if(was_constructed)
   {
      while(!mGrabControllerData.lock().success())
      {;}
         initSharedDataDefaults();
      mGrabControllerData.unlock();
      mGrabControllerData.sync();
   }
   else
   {
      //vprASSERT(dso::getGuid(mGrabControllerData->mObjId) == objId && "Intialized object, but found objId seems to be incorrect");      
   }
}

virtual OSG::NodePtr checkSelection(OpenSGViewer* viewer)
{
   //   vwMo - Virtual World to object xform,
   //   vwMi - virtual world to "interface" (wand),
   //   iMo - Interface to object
   //   vwMn - virtual world to object containing node,
   //   nMo - Node to object (local object xform)
   //   Interface to object is estabilished at "grab time" and from then on while grabbed, wMo is calculated
   //
   // @todo Take into account the fact that the object is under and xform itself.
   //
   // Transformations with _Mo_ are old values from previous frame
   //
   // Transformation to new position
   // vwMo = vm_M_plat * plat_M_wand * wand_Mo_plat * plat_Mo_vw * vw_Mo_o
   //

   User* cur_user = viewer->getUser();
   std::vector<OSG::NodePtr> selectable_nodes = viewer->getSelectableNodes();

   bool grab_pressed = ( cur_user->getButton(1)->getData() == gadget::Digital::ON);
   bool grab_edge_on = ( cur_user->getButton(1)->getData() == gadget::Digital::TOGGLE_ON );
      
   gmtl::Matrix44f vw_M_plat = cur_user->getXform_vwMrw();     // Get the xform from vw to real world (platform)

   gmtl::Matrix44f plat_M_wand, vw_M_wand;
   plat_M_wand = *(cur_user->getWandPos()->getData());      // Get the wand matrix
   vw_M_wand = vw_M_plat * plat_M_wand;                   // Get the wand matrix in virtual world coords

#ifdef DEBUG_DRAW_VIEW_CONTROLLER
   mDraw_xform_vw_M_wand = vw_M_wand;
   mDraw_xform_vw_M_plat = vw_M_plat;
   mDraw_cur_xform_vm_M_o = traverser->getXformMat();
#endif

   // Update some needed temporary info
   // for (each selectable node)
   // - If isects
   bool           does_isect(false);
   OSG::NodePtr   cur_node(viewer->getCurSelectedNode());
   
   does_isect = this->intersect(vw_M_wand);

      
   // -- UPDATE LOCAL STATE --- //
   // Initially no states have changed
   mSelectStateChanged = false;        // Intially state has not changed

   // Switch on selection state
   switch (mLocalSelectionState)
   {
   case NotSelected:
      {
         if(does_isect)
         {
            vprDEBUG(dspaceCNTRL, 1) << "GrabController: Intersection with entity: " << getEntity()->getName() << std::endl << vprDEBUG_FLUSH;

            mLocalSelectionState = IsHighlighted;            
            mSelectStateChanged = true;   // Now in selected state
            vprDEBUG(dspaceCNTRL, vprDBG_WARNING_LVL) << "GrabController: NotSelected --> IsSelected.\n" << vprDEBUG_FLUSH;
         }         
      }
      break;
   case IsHighlighted:
      {
         // If we don't isect any more --> NotSelected
         if(!does_isect)
         {
            mLocalSelectionState = NotSelected;
            mSelectStateChanged = true;
            vprDEBUG(dspaceCNTRL, vprDBG_WARNING_LVL) << "GrabController: IsSelected --> NotSelected.\n" << vprDEBUG_FLUSH;
         }

         // If grab pressed --> Store obj matrix, IsGrabbed
         if(grab_edge_on)
         {
            // Check to see if we have access to grab the entity
            bool granted_access = DataSpace::instance()->getSecurityManager()->shouldGrantEntityAccess(getEntity());
            
            if(granted_access)
            {
               // Store xform values that will be needed next frame
               gmtl::Matrix44f wand_M_vw;
               gmtl::invert(wand_M_vw, vw_M_wand);
               gmtl::Matrix44f xform_vw_M_o = traverser->getXformMat();
   
               // Store the stable wand to object xform
               mXform_wand_M_o =  wand_M_vw * xform_vw_M_o;
   
               // Set grabber to isGrabbed
               while(!mLocalGrabber.lock().success())
               {;}
                  mLocalGrabber->mIsGrabbed = 1;
               mLocalGrabber.unlock();  
               mLocalGrabber.sync();
   
               mLocalSelectionState = IsSelected;
               mSelectStateChanged = true;
               mSounds[LocalGrab].trigger();
               vprDEBUG(dspaceCNTRL, vprDBG_WARNING_LVL) << "GrabController: IsSelected --> IsGrabbed.\n" << vprDEBUG_FLUSH;
               vprASSERT(isOurGrabberInList() && "We didn't have our grabber in the list for some reason");
            }
         }
      }
      break;
   case IsSelected:
      {         
         // Update the model position
         // --- Move to new position --- //
         // - Calculate the new object position (nMo - node to object - for local coord space)
         // xform: vwMo = vm_M_plat * plat_M_wand * wand_M_o
         // update: vwMo = vwMn * nMo
         // Calculate the new desired obj position in the vw
         gmtl::Matrix44f xform_vw_M_o;
         xform_vw_M_o = vw_M_plat * plat_M_wand * mXform_wand_M_o;

         // --- Update the position (solve for nMo) --- //
         // Calculate node to vw xform (we already have node xform applied at this time)
         // nMvw = nMo * oMvw -- node xform = local * inv(obj)
         // nMo = nMvw * vwMo
         gmtl::Matrix44f xform_n_M_o, xform_n_M_vw;
         gmtl::Matrix44f cur_xform_oMvw(traverser->getXformMat());
         gmtl::invert(cur_xform_oMvw);

         xform_n_M_vw = getEntity()->getPos() * cur_xform_oMvw;
         xform_n_M_o = xform_n_M_vw * xform_vw_M_o;

         mLocalGrabber.lock();
            mLocalGrabber->mDesiredPos = dspace::getMatrixData( xform_n_M_o );
         mLocalGrabber.unlock();
         mLocalGrabber.sync();

#ifdef DEBUG_DRAW_VIEW_CONTROLLER
         mDraw_xform_vm_M_o =  xform_vw_M_o;
         gmtl::invert(mDraw_cur_xform_vm_M_n,xform_n_M_vw);
#endif

         // If we hold the lock, then we need to update the position info
         if(IsGrabbedAndLocked == mLocalSelectionState)
         {
            getEntity()->setPos(xform_n_M_o);      // Set the position
            getEntity()->setPos(new_pos);      // Set the position
            getEntity()->pushChanges();            // Let other nodes know about updates
         }
         
         // -- Handle State transitions --- //
         // Let go of grab --> IsSelected
         if(!grab_pressed)
         {
            // If we had the lock, then we must release it first
            if(IsGrabbedAndLocked == mLocalSelectionState)
            {
               while(!mGrabControllerData.lock().success())
               {;}
                  mGrabControllerData->mLockState = 0;      // Set new lock state
               mGrabControllerData.unlock();
               mGrabControllerData.sync();

               vpr::ReturnStatus unlock_status = getEntity()->unlock();
               vprASSERT(unlock_status.success() && "Failed to unlock");
               vprDEBUG(dspaceCNTRL, vprDBG_WARNING_LVL) << "GrabController: IsGrabbedAndLocked --> IsSelected.\n" << vprDEBUG_FLUSH;
            }
            else
            {
               vprDEBUG(dspaceCNTRL, vprDBG_WARNING_LVL) << "GrabController: IsGrabbed --> IsSelected.\n" << vprDEBUG_FLUSH;
            }

            // Set local grabber to un-grabbed
            while(!mLocalGrabber.lock().success())
            {;}
               mLocalGrabber->mIsGrabbed = 0;
            mLocalGrabber.unlock();  
            mLocalGrabber.sync();

            mLocalSelectionState = IsSelected;
            mSelectStateChanged = true;
            mSounds[LocalUngrab].trigger();
         }
         // Check for unlocked state
         // If unlocked, then try to get lock locally
         else if(!remote_locked)
         {
            vprASSERT(mLocalSelectionState != IsGrabbedAndLocked && "Shouldn't be able to have this state and !remote_locked");
            vpr::ReturnStatus lock_status = getEntity()->lock(vpr::Interval(4,vpr::Interval::Msec));
            if(lock_status.success())  // If got lock
            {               
               while(!mGrabControllerData.lock().success())
               {;}
                  mGrabControllerData->mLockState = 1;      // Set new lock state
               mGrabControllerData.unlock();
               mGrabControllerData.sync();
               
               mLocalSelectionState = IsGrabbedAndLocked;
               mSelectStateChanged = true;
               mSounds[LocalLock].trigger();
               vprDEBUG(dspaceCNTRL, vprDBG_WARNING_LVL) << "GrabController: IsGrabbed --> IsGrabbedAndLocked.\n" << vprDEBUG_FLUSH;
            }
         }
      }      
      break;
   default:
      vprASSERT(false && "mLocalSelectionState is in an unknown state, this is bad.");
   }
   
}

/** Check for intersection
* @returns true If the point is within the boundign volume
*                 of the associated view.
*/
bool HoverSelectionMethod::intersect(OSG::NodePtr node, gmtl::Matrix44f wandWorldPos)
{
   // XXX: The bounding volume returned from getWorldVolume is in the platform (real world)
   //      coordinate frame, so we need to transform wandWorldPos in that coordinate
   //      frame from the vw coordinate frame it starts in
   gmtl::Matrix44f plat_M_virt =  User::instance()->viewPlatform()->getTransform_platMvirt();
   gmtl::Matrix44f plat_M_wand = plat_M_virt * wandWorldPos;

   osg::Matrix osg_wand_pos;
   gmtl::set(osg_wand_pos, plat_M_wand);
   osg::Pnt3f osg_wand_pos_pnt(0.0f, 0.0f, 0.0f);
   osg_wand_pos.multFullMatrixPnt(osg_wand_pos_pnt);

   osg::DynamicVolume bounding_volume;
   node->getWorldVolume(bounding_volume);

   bool does_isect = bounding_volume.intersect(osg_wand_pos_pnt);

   // --- Debugging
   osg::Matrix osg_wand_pos_vw;
   gmtl::set(osg_wand_pos_vw, wandWorldPos);
   updateWandRep(osg_wand_pos_vw);        // Update the wand pos rep

   osg::Pnt3f min_pnt, max_pnt;
   bounding_volume.getBounds(min_pnt, max_pnt);

   gmtl::Vec3f wand_pos_pnt;
   wand_pos_pnt = gmtl::makeTrans<gmtl::Vec3f>(plat_M_wand);
   //std::cout << "Wand pos [plat]: " << wand_pos_pnt << std::endl;
   //std::cout << "Bounding vol (plat): \n" << std::flush;
   //bounding_volume.dump();

   return does_isect;
}





};

#endif

