#include <gadget/Devices/Open/Trackd/TrackdControllerStandalone.h>

#include <assert.h>


//: Get the num input values
int TrackdControllerStandalone::numButtons()
{
   assert(mMem != NULL);
#ifdef GADGET_HAVE_TRACKDAPI
   return mMem->trackdGetNumberOfButtons();
#else 
   return trackd_num_buttons(mMem);
#endif
}
int TrackdControllerStandalone::numValuators()
{
   assert(mMem != NULL);
#ifdef GADGET_HAVE_TRACKDAPI
   return mMem->trackdGetNumberOfValuators();
#else 
   return trackd_num_valuators(mMem);
#endif
}

// Return the value of the button
int TrackdControllerStandalone::getButton(int btnNum)
{
   assert(mMem != NULL);
#ifdef GADGET_HAVE_TRACKDAPI
   return mMem->trackdGetButton(btnNum);
#else 
   return trackd_button(mMem, btnNum);
#endif
}


float TrackdControllerStandalone::getValuator(int valNum)
{
   assert(mMem != NULL);
#ifdef GADGET_HAVE_TRACKDAPI
   return mMem->trackdGetValuator(valNum);
#else 
   return trackd_valuator(mMem, valNum);
#endif
}

//: Attach to the memory segment with key (mShmKey)
//! POST: mMem = address of the shared memory area
void TrackdControllerStandalone::attachToMem()
{
   assert(mShmKey != 0 && "Key was not set correctly");
#ifdef GADGET_HAVE_TRACKDAPI
   mMem = new ControllerReader(mShmKey);
#else 
   mMem = trackd_attach_tracker_mem(mShmKey);
#endif
}

//: Release the memory segment of mMem
void TrackdControllerStandalone::releaseMem()
{
   assert(mMem != NULL && "Trying to release trackd memory that was NULL");
#ifdef GADGET_HAVE_TRACKDAPI
   delete mMem;
#else 
   trackd_release_tracker_mem(mMem);
#endif
}

