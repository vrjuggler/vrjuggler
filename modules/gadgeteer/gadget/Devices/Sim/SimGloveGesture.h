#ifndef _VJ_SIM_GLOVE_GESTURE_H_
#define _VJ_SIM_GLOVE_GESTURE_H_


//: Simulated glove class
//
// This class simulates a gesture input device
//
// By default the glove is in gesture 0
class vjSimGloveGesture : public vjGesture, public vjGlove, public vjSimInput
{
public:
   //: Construct a vjSimGloveGesture
   vjSimGloveGesture(vjConfigChunk* chunk);

   //: Get the current gesture.
   //! RETURNS: id of current gesture
   virtual int getGesture();

   //: Load trained data for the gesture object
   // Loads the file for trained data
   void loadTrainedFile(string fileName);

   int StartSampling() { return 1; }
   int StopSampling() { return 1; }
   int Sample() { return 1; }

   //: Update the device data
   void UpdateData ();

   //: Return our name
   char* GetDeviceName() { return "vjSimGloveGesture"; }

   /*** These are not supported in sim ***/
   void saveTrainedFile(string fileName) { ;}
   void loadSamplesFile(string filename) {;}
   void saveSamplesFile(string filename) {;}
   void clearSamples(int gestureId=-1) {;}
   void addSample(int gestureId) {;}
   void train() { ; }

private:
   vector<vjKeyModPair>    mSimKeys;      //: The keys to press for the gestures
   int                     mCurGesture;   //: The current gesture id
};

