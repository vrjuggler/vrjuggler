// vjDummyGesture.h
//
//

#ifndef _VJ_DUMMY_GESTURE_H_
#define _VJ_DUMMY_GESTURE_H_

//: A dummy gesture device for gesture devices
//
// See also: vjGesture
//!PUBLIC_API:
class vjDummyGesture : public vjGesture
{
public:
   vjDummyGesture() : vjGesture()
   { active = 1; }
   ~vjDummyGesture() {}

   /* Dummies do nothing */
   int StartSampling() { return 1; }
	int StopSampling() { return 0; }
	int Sample() { return 1;}
	void UpdateData() { return ; }

   char* GetDeviceName()  {return "vjDummyGesture"; }

   //: Retuns the id of the named gesture.
   //! RETURNS: -1 - Gesture name Not found.
   virtual int getGestureIndex(std::string name) { return -1; }

   //: Not implemented
   // returns null string
   virtual std::string getGestureString(int id) { return std::string(""); }

   //: Not implemented
   virtual void train() { }

   //: Get the current gesture. Not implemented
   //! RETURNS: -1
   virtual int getGesture() { return -1; }

   //: Not implemented
   virtual int createGesture(std::string gestureName) { return -1; }

   //: Not implemented
   virtual void addSample(int gestureId) { }

   //: Not implemented
   virtual void loadSamplesFile(std::string filename) { }

   //: Not implemented
   virtual void clearSamples(int gestureId=-1) { }

   //: Not implemented
   virtual void loadTrainedFile(std::string fileName) {}

   //: Not implemented
   virtual void saveTrainedFile(std::string fileName) {}

   //: Not implemented
   virtual void saveSamplesFile(std::string filename) {}
};

#endif
