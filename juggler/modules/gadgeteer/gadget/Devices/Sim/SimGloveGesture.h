
class vjSimGloveGesture : public vjGloveGesture, public vjGlove
{
public:

   vjSimGloveGesture(vjConfigChunk* chunk);

   //: This actually starts the training on the given data.
   virtual void train() {
   }
   //: Get the current gesture.
   //! RETURNS: id of current gesture
   virtual int getGesture() {
   }
   //: Adds a new sample of the given gesture to the training data.
   virtual void addSample(int gestureId) {
   }
   //: Loads the sample training data specified.
   // This file contains previous samples for the gesture recognizer to train from.
   virtual void loadSamplesFile(string filename) {
   }
   //: Clear the samples that we have taken so far.
   //! ARGS: gestureId = -1 (default) then we clear all gestures else we clear only
   // the gesture that is specified.
   virtual void clearSamples(int gestureId=-1) {
   }
   //: Load trained data for the gesture object
   virtual void loadTrainedFile(string fileName) {
   }
   //: Save a trained data file for the gesture object
   virtual void saveTrainedFile(string fileName) {
   }
   //: Saves the sample training data specified.
   // This data can be loaded at a later time to do more sample training.
   virtual void saveSamplesFile(string filename) {
   }
   //: StopSampling.
   //
   //  Reverse the effects of StartSampling()
   virtual int StopSampling() {
   }
   //: Sample the device
   //
   //  Every input device should have a sample function, after which the
   //  device has been sampled to have new data.  (This new data is not
   //  accessable until UpdateData is called, however)
   virtual int Sample() {
   }
   //: Start a device sampling.
   //
   //  Start the device sampling, normally this will spawn a thread which will
   //  just repeatedly call Sample().
   //  This function should return true when it sucessfully starts,
   //      false otherwise.
   virtual int StartSampling() {
   }
   //: UpdateData()
   //
   //  After this function is called subsequent calls to GetData(d) will
   //  return the most recent data at the time of THIS function call.  Data is
   //  guaranteed to be valid and static until the next call to UpdateData.
   virtual void UpdateData() {
   }
   //: GetDevicename()
   //
   //  Returns the name identifying the TYPE of Input Device
   virtual char* GetDeviceName() {
   }

private:

};

