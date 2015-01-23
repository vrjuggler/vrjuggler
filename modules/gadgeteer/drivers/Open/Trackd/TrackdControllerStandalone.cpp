#include <drivers/Open/Trackd/TrackdControllerStandalone.h>

#include <assert.h>

/** Gets the num input values. */
int TrackdControllerStandalone::numButtons()
{
   assert(mCon != NULL);
   return trackd_controller_num_buttons(mCon);
}
int TrackdControllerStandalone::numValuators()
{
   assert(mCon != NULL);
   return trackd_controller_num_valuators(mCon);
}

/** Returns the value of the button. */
int TrackdControllerStandalone::getButton(int btnNum)
{
   assert(mCon != NULL);
   return trackd_controller_button(mCon, btnNum);
}

float TrackdControllerStandalone::getValuator(int valNum)
{
   assert(mCon != NULL);
   return trackd_controller_valuator(mCon, valNum);
}

/**
 * Attaches to the memory segment with key (mShmKey).
 * @post mCon = address of ControllerConnection object.
 */
void TrackdControllerStandalone::attachToMem()
{
   assert(mShmKey != 0 && "Key was not set correctly");
   assert(mCon    == NULL && "Already attached");
   mCon = trackd_controller_attach(mShmKey);
}

/** Releases the ControllerConnection object. */
void TrackdControllerStandalone::releaseMem()
{
   assert(mCon != NULL && "Trying to release trackd memory that was NULL");
   trackd_controller_release(mCon);
   mCon = NULL;
}
