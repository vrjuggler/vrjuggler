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

#include <DataSpaceSecurityManager.h>

#include <View/GrabController.h>

#include <User/InterfaceMethod.h>
#include <User/SimpleWandInterface.h>

#include <Visitor/Traverser.h>

#include <Entity/idl/MatrixHelper.h>
#include <dso/idl/GUIDDataHelper.h>

#include <DataSpace.h>
#include <User/User.h>

#include <gmtl/Coord.h>
#include <gmtl/Output.h>
#include <gmtl/Quat.h>
#include <gmtl/Generate.h>
#include <gmtl/MatrixOps.h>

void GrabController::initSounds()
{
   // Make sounds the right size
   mSounds.resize(NumSounds);

   mSounds[LocalSelect].init("LocalSelect");
   mSounds[LocalDeselect].init("LocalDeselect");
   mSounds[LocalGrab].init("LocalGrab");
   mSounds[LocalUngrab].init("LocalUngrab");
   mSounds[LocalLock].init("LocalLock");
   mSounds[RemoteSelect].init("RemoteSelect");
   mSounds[RemoteDeselect].init("RemoteDeselect");
   mSounds[RemoteLock].init("RemoteLock");
   mSounds[RemoteUnlock].init("RemoteUnlock");   
}


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

void GrabController::initLocalGrabControllerData()
{
   vprASSERT(getObjId() != vpr::GUID::NullGUID);

   bool was_constructed, found_remotely;
   dso::GUID local_grabber_guid(dso::GUID::generateTag);
   mLocalGrabber.init(local_grabber_guid, true, NULL, found_remotely, was_constructed);
   bool found_it = !was_constructed;
   
   vprASSERT( (found_it || was_constructed) && "Initialized failed???");
   vprASSERT(!found_it && "Somehow we found the local grabber and that is not good");

   vprDEBUG(dspaceBUILDER, vprDBG_CONFIG_LVL) <<  "   Initialized grab controller::localGrabber: [" << local_grabber_guid.toString() 
                                << "] found=[" << (found_it?"true":"false") << "]\n" << vprDEBUG_FLUSH;
       
   while(!mLocalGrabber.lock().success())
   {;}
      mLocalGrabber->mIsGrabbed = 0;
      mLocalGrabber->mDesiredPos = dspace::getMatrixData( gmtl::MAT_IDENTITY44F );
   mLocalGrabber.unlock();  
   mLocalGrabber.sync();
}

/*
void GrabController::init(dso::ObjectProxyBase* objData)
{
   vprASSERT(objData != NULL);
   mGroupEntityData = *objData;

   // Call to parent class init method
   Entity::init(objData);
}
*/

void GrabController::initSharedDataDefaults()
{
   //setObjId(dso::GUID::NullGUID);
   mGrabControllerData->mObjId = dso::getGUIDData(getObjId());
   mGrabControllerData->mLockState = 0;
}

void GrabController::preUpdate(Traverser* traverser)
{
   Controller::preUpdate(traverser);

   SimpleWandInterface* wand_i
      = dynamic_cast<SimpleWandInterface*>( DataSpace::instance()->getUser()->getInterfaceMethod(SimpleWandInterface::typeID()) );

   vprASSERT(wand_i != NULL);

   bool grab_pressed = ( wand_i->getButton(1)->getData() == gadget::Digital::ON);
   bool grab_edge_on = ( wand_i->getButton(1)->getData() == gadget::Digital::TOGGLE_ON );

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
   User* cur_user = DataSpace::instance()->getUser();          // Get the current user
   gmtl::Matrix44f vw_M_plat = cur_user->getXform_vwMrw();     // Get the xform from vw to real world (platform)

   gmtl::Matrix44f plat_M_wand, vw_M_wand;
   plat_M_wand = *(wand_i->getWandPos()->getData());      // Get the wand matrix
   vw_M_wand = vw_M_plat * plat_M_wand;                   // Get the wand matrix in virtual world coords

#ifdef DEBUG_DRAW_VIEW_CONTROLLER
   mDraw_xform_vw_M_wand = vw_M_wand;
   mDraw_xform_vw_M_plat = vw_M_plat;
   mDraw_cur_xform_vm_M_o = traverser->getXformMat();
#endif

   // Update some needed temporary info
   bool     does_isect = this->intersect(vw_M_wand);
   mGrabControllerData.sync();                                        // Get most recent copy
   bool     remote_locked = (1 == mGrabControllerData->mLockState);   // Is the grab controller currently locked
   unsigned num_remote_grabbers = mGrabControllerData->mGrabbers.length();
   if(mLocalSelectionState != NotSelected)   // If we have it selected, then we are in list so decrement it
   {  num_remote_grabbers--; }

   // --- REMOTE STATE TRACKING --- //
   mOldLocalSelectionState = mLocalSelectionState;
   mOldRemoteState = mRemoteState;
   if(remote_locked && (mLocalSelectionState != IsGrabbedAndLocked))
   {  
      mRemoteState = HoldsLock; 
      if(mOldRemoteState != mRemoteState) vprDEBUG(dspaceCNTRL, vprDBG_CONFIG_LVL) << "Remote State: HoldsLock\n" << vprDEBUG_FLUSH;
   }
   else if(num_remote_grabbers > 0)
   {        
      mRemoteState = DoesIsect; 
      if(mOldRemoteState != mRemoteState) vprDEBUG(dspaceCNTRL, vprDBG_CONFIG_LVL) << "Remote State: DoesIsect\n" << vprDEBUG_FLUSH;
   }
   else
   {
      mRemoteState = UnSelected;
      if(mOldRemoteState != mRemoteState) vprDEBUG(dspaceCNTRL, vprDBG_CONFIG_LVL) << "Remote State: UnSelected\n" << vprDEBUG_FLUSH;
   }

   if(mOldRemoteState != mRemoteState)
   {
      if(remote_locked && (mLocalSelectionState != IsGrabbedAndLocked))
      {  
         mSounds[RemoteLock].trigger();
         vprDEBUG(dspaceCNTRL, vprDBG_CONFIG_LVL) << "Remote State: HoldsLock\n" << vprDEBUG_FLUSH;
      }
      else if(num_remote_grabbers > 0)
      {        
         if(HoldsLock == mOldRemoteState) 
         {
            mSounds[RemoteUnlock].trigger();
         }
         else
            mSounds[RemoteSelect].trigger();
         vprDEBUG(dspaceCNTRL, vprDBG_CONFIG_LVL) << "Remote State: DoesIsect\n" << vprDEBUG_FLUSH;
      }
      else
      {
         mSounds[RemoteDeselect].trigger();
         vprDEBUG(dspaceCNTRL, vprDBG_CONFIG_LVL) << "Remote State: UnSelected\n" << vprDEBUG_FLUSH;
      }
   }
      
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
            vprDEBUG(dspaceCNTRL, vprDBG_CONFIG_LVL) << "GrabController: Intersection with entity: " << getEntity()->getName() << std::endl << vprDEBUG_FLUSH;

            // Add to grabber list and go to SelectedState
            addOurGrabberToList();     // Locks the list and adds our grabber to it
            
            mLocalSelectionState = IsSelected;            
            mSelectStateChanged = true;   // Now in selected state
            mSounds[LocalSelect].trigger();
            vprDEBUG(dspaceCNTRL, vprDBG_WARNING_LVL) << "GrabController: NotSelected --> IsSelected.\n" << vprDEBUG_FLUSH;
         }         
      }
      break;
   case IsSelected:
      {
         // If we don't isect any more --> NotSelected
         if(!does_isect)
         {
            removeOurGrabberFromList();      // Locks the list and removes our grabber from it
            mLocalSelectionState = NotSelected;
            mSelectStateChanged = true;
            mSounds[LocalDeselect].trigger();
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
   
               mLocalSelectionState = IsGrabbed;
               mSelectStateChanged = true;
               mSounds[LocalGrab].trigger();
               vprDEBUG(dspaceCNTRL, vprDBG_WARNING_LVL) << "GrabController: IsSelected --> IsGrabbed.\n" << vprDEBUG_FLUSH;
               vprASSERT(isOurGrabberInList() && "We didn't have our grabber in the list for some reason");
            }
         }
      }
      break;
   case IsGrabbed:
   case IsGrabbedAndLocked:
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
            gmtl::Matrix44f new_pos = getAverageDesiredPosition();
            //getEntity()->setPos(xform_n_M_o);      // Set the position
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

/** Add our grabber to the list of grabbers.
* @post Our grabber is in list and it's values are set to default starting states
*/
void GrabController::addOurGrabberToList()
{
   while(!mGrabControllerData.lock().success())
   {;}
   
   dso::Ptr_var local_grabber_var = mLocalGrabber;
   dso::GUID local_grabber_guid = dso::getGuid(local_grabber_var->ptrGUID);
   vprDEBUG(dspaceCNTRL, vprDBG_WARNING_LVL) << "GrabController::addOurGrabberToList: guid: [" << local_grabber_guid.toString() << "]\n" << vprDEBUG_FLUSH;

   vprASSERT(!isOurGrabberInList() && "Tried to add grabber, but it is already there");
      
   int len = mGrabControllerData->mGrabbers.length();
   mGrabControllerData->mGrabbers.length(len+1);                    // Resize it by +1
   mGrabControllerData->mGrabbers[len] = local_grabber_var;      // Add cntrl to the end
      
   mGrabControllerData.unlock();   
   mGrabControllerData.sync();

   vprASSERT(isOurGrabberInList() && "Add failed in some way");
}

/** Removes our grabber from the list of grabbers.
*/
void GrabController::removeOurGrabberFromList()
{
   while(!mGrabControllerData.lock().success())
   {;}
   dso::Ptr_var local_grabber_var = mLocalGrabber;
   dso::GUID local_grabber_guid = dso::getGuid(local_grabber_var->ptrGUID);
   vprDEBUG(dspaceCNTRL, vprDBG_WARNING_LVL) << "GrabController::removeOurGrabberFromList: guid: [" << local_grabber_guid.toString() << "]\n" << vprDEBUG_FLUSH;

   vprASSERT(isOurGrabberInList() && "Trying to remove grabber but it ain't there");   

   // Make sure we don't already have one
   int len = mGrabControllerData->mGrabbers.length();
   int found_at(-1);     // Location found at
   for(int i=0;i<len;++i)
   {
      if( dso::getGuid(mGrabControllerData->mGrabbers[i]->ptrGUID)
          == dso::getGuid(local_grabber_var->ptrGUID))
      {
         found_at = i;
      }
   }

   vprASSERT(found_at != -1);

   // For each entry after found, move them up one
   for(int j=found_at+1;j<len;++j)
   {
      mGrabControllerData->mGrabbers[j-1] = mGrabControllerData->mGrabbers[j];
   }
   mGrabControllerData->mGrabbers.length(len-1);                    // Resize it by -1

   mGrabControllerData.unlock();
   mGrabControllerData.sync();

   vprASSERT(!isOurGrabberInList() && "Remove failed in some way");
}

/** Is our grabber in the list */
bool GrabController::isOurGrabberInList()
{
   bool found(false);
   dso::Ptr_var local_grabber_var = mLocalGrabber;
   
   vprDEBUG_OutputGuard(dspaceCNTRL, vprDBG_CRITICAL_LVL, "Checking Grabber list\n", "Exiting grabber check");

   // Check to see if it is in list
   int len = mGrabControllerData->mGrabbers.length();
   dso::GUID our_ptr_guid = dso::getGuid(local_grabber_var->ptrGUID);
   
   vprDEBUG(dspaceCNTRL, vprDBG_CRITICAL_LVL) << "  Grabber list len: [" << len << "]\n" << vprDEBUG_FLUSH;
   vprDEBUG(dspaceCNTRL, vprDBG_CRITICAL_LVL) << "Local grabber guid: [" << our_ptr_guid << "]\n" << vprDEBUG_FLUSH;

   for(int i=0;i<len;++i)
   {
      vprDEBUG(dspaceCNTRL, vprDBG_CRITICAL_LVL) << "Checking grabber: [" << dso::getGuid(mGrabControllerData->mGrabbers[i]->ptrGUID) << "]\n" << vprDEBUG_FLUSH;
      if( dso::getGuid(mGrabControllerData->mGrabbers[i]->ptrGUID) == our_ptr_guid)
      {  found = true; }
   }

   return found;
}

gmtl::Matrix44f GrabController::getAverageDesiredPosition()
{
   std::vector<gmtl::Coord3fQuat>   desired_positions;   

   // Build list of matrices
   int len = mGrabControllerData->mGrabbers.length();
   for(int i=0;i<len;++i)
   {
      bool found_remotely(false), was_constructed(false);
      dso::ObjectProxy<dspace::GrabController::Grabber>  cur_grabber(mGrabControllerData->mGrabbers[i], false, NULL, found_remotely, 
                                                                     was_constructed, vpr::Interval(1,vpr::Interval::Sec));
      // If found entry
      if(!cur_grabber.isNull())
      {
         cur_grabber.sync();    // Get latest updates
         if(cur_grabber->mIsGrabbed)      // If grabbed
         {
            gmtl::Matrix44f                  cur_matrix;
            gmtl::Coord3fQuat                cur_coord;
            cur_matrix = dspace::getMatrix(cur_grabber->mDesiredPos);
            gmtl::set(cur_coord, cur_matrix);
            desired_positions.push_back(cur_coord);
         }
      }
      else
      {
         vprDEBUG(dspaceCNTRL, vprDBG_CRITICAL_LVL) << "getAverageDesiredPosition: Failed to find grabber for: [???]\n" << vprDEBUG_FLUSH;
      }
   }

   // Compute average matrix
   float scale_value = 1.0f/float(desired_positions.size());      // The scalar to use on each transform

   // The average values to compute
   gmtl::Vec3f av_pos, scaled_pos;
   gmtl::Quatf av_quat, scaled_quat;
   gmtl::Quatf ident_quat;
   gmtl::set(ident_quat, gmtl::MAT_IDENTITY44F);    // Assign rotation identity
   gmtl::set(av_quat, gmtl::MAT_IDENTITY44F);

   gmtl::Matrix44f accum_rotation(gmtl::MAT_IDENTITY44F);

   std::cout << "Cos angles: ";
   for(unsigned j=0;j<desired_positions.size();++j)
   {
      scaled_pos = desired_positions[j].getPos() * scale_value;
      av_pos += scaled_pos;
      gmtl::slerp( scaled_quat, scale_value, ident_quat, desired_positions[j].getRot(), false);

      float cos_angle = gmtl::dot(ident_quat, desired_positions[j].getRot());
      std::cout << cos_angle << " -- "; 
      
      //av_quat = av_quat * scaled_quat;    // Hope this works
      accum_rotation *= gmtl::make<gmtl::Matrix44f>(scaled_quat);
   }
   std::cout << std::endl;

   gmtl::Matrix44f ret_mat;
   ret_mat = gmtl::makeTrans<gmtl::Matrix44f>(av_pos) * accum_rotation;
   
   //gmtl::Coord3fQuat ret_coord;
   //ret_coord.pos() = av_pos;
   //ret_coord.rot() = av_quat;
   //gmtl::set(ret_mat, ret_coord);

   return ret_mat;
}

