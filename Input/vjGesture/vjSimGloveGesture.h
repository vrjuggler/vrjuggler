#ifndef _VJ_SIM_GLOVE_GESTURE_H_
#define _VJ_SIM_GLOVE_GESTURE_H_

#include <vjConfig.h>
#include <Input/vjGlove/vjGlove.h>
#include <Input/vjGesture/vjGloveGesture.h>


class vjSimGloveGesture : public vjGloveGesture, public vjGlove
{
public:
   public void UpdateData() {}

   public void GetDeviceName() {}

   public void loadTrainedFile(string filename) {}

   public void vjSimGloveGesture(vjConfigChunk* chunk) {}

   public void vjSimGloveGesture() {}

   public void StartSampling() {}

   public void StopSampling()
   {}

   void getGesture()
   {
   }

private:
  //vjKeyboardProxy mEventSource;
};
