#include <Input/Multi/aTrackdController.h>
//#include <Kernel/vjDebug.h>

#include <assert.h>

//: Get the num input values
int aTrackdController::numButtons()
{
   assert(mMem != NULL);
   return trackd_num_buttons(mMem);
}
int aTrackdController::numValuators()
{
   assert(mMem != NULL);
   return trackd_num_valuators(mMem);
}

// Return the value of the button
int aTrackdController::getButton(int btnNum)
{
   assert(mMem != NULL);
   return trackd_button(mMem, btnNum);
}


float aTrackdController::getValuator(int valNum)
{
   assert(mMem != NULL);
   return trackd_valuator(mMem, valNum);
}

//: Attach to the memory segment with key (mShmKey)
//! POST: mMem = address of the shared memory area
void aTrackdController::attachToMem()
{
   assert(mShmKey != 0 && "Key was not set correctly");
   mMem = trackd_attach_tracker_mem(mShmKey);
}

//: Release the memory segment of mMem
void aTrackdController::releaseMem()
{
   assert(mMem != NULL && "Trying to release trackd memory that was NULL");
   trackd_release_tracker_mem(mMem);
}

