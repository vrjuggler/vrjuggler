#include <gadget/Devices/Open/Trackd/TrackdControllerStandalone.h>

#include <assert.h>

/** Gets the num input values. */
int TrackdControllerStandalone::numButtons()
{
   assert(mMem != NULL);
   return trackd_num_buttons(mMem);
}
int TrackdControllerStandalone::numValuators()
{
   assert(mMem != NULL);
   return trackd_num_valuators(mMem);
}

/** Returns the value of the button. */
int TrackdControllerStandalone::getButton(int btnNum)
{
   assert(mMem != NULL);
   return trackd_button(mMem, btnNum);
}


float TrackdControllerStandalone::getValuator(int valNum)
{
   assert(mMem != NULL);
   return trackd_valuator(mMem, valNum);
}

/**
 * Attaches to the memory segment with key (mShmKey).
 * @post mMem = address of the shared memory area.
 */
void TrackdControllerStandalone::attachToMem()
{
   assert(mShmKey != 0 && "Key was not set correctly");
   mMem = trackd_attach_tracker_mem(mShmKey);
}

/** Releases the memory segment of mMem. */
void TrackdControllerStandalone::releaseMem()
{
   assert(mMem != NULL && "Trying to release trackd memory that was NULL");
   trackd_release_tracker_mem(mMem);
}
