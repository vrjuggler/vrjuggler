#ifndef _GADGET_INPUT_LOGGER_H_
#define _GADGET_INPUT_LOGGER_H_

#include <gadget/gadgetConfig.h>
#include <vpr/vpr.h>
#include <jccl/Config/ConfigChunkPtr.h>

#include <gadget/Type/DigitalInterface.h>
#include <cppdom/cppdom.h>

#include <boost/smart_ptr.hpp>
#include <gadget/InputLoggerPtr.h>


namespace gadget
{

/** Input data logger
*/
class InputLogger
{
public:
   InputLogger()
      : mCurState(Inactive), mSleepFramesLeft(0)
   {;}

   /** Destructor */
   virtual ~InputLogger()
   {;}

   /** Configure the logger */
   virtual bool config( jccl::ConfigChunkPtr chunk);

   /** Processing function.
   * This method is called by the input manager each update frame
   * to allow the logger to do any processing that it needs
   * to complete.
   */
   void process();

public:
   /** @name Recording interface */
   //@{
   void startRecording();

   void stopRecording();

   /** Collect a stamp in the recording */
   void stampRecord();
   //@}

public:
   /** @name Playback interface */
   //@{
   /** Load a log file.
   * @param logFilename - The name of the log file to load
   */
   void load(std::string logFilename);

   /** Play the currently active log */
   void play();

   /** Stop playing a log */
   void stop();

   /** Pause log playback. */
   void pause();

   /** Get the stamp for the most recent sample
   * @return Returns empty string if no active stamp.
   */
   std::string getStamp();
   //@}

public:
   /** @name Query methods */
   //@{
   bool getState()
   {  return mCurState; }
   //@}

protected:
   /** @name Internal helpers */
   //@{

   void addRecordingSample();

   void playNextSample();
   //@}


public:
   /** List of states that the Logger can be in */
   enum State
   {
      Inactive,   /**< The Logger is Inactive to be told what to do. ie. doing nothing */
      Recording,  /**< The Logger is current recording */
      Playing,    /**< The Logger is currently playing */
      Paused      /**< The Logger is currently paused */
   };

private:
   State       mCurState;        /**< The current state of the logger */
   
   /** When the state becomes inactive, this is the number of frames that
   * we wait before processing any input.
   */
   unsigned    mSleepFramesLeft; 
   std::string mActiveStamp;     /**< The active stamp for this frame */

   cppdom::NodePtr   mRootNode;           /**< Root node of the data */
   std::string       mRecordingFilename;  /**< Filename to use for the recording */

   cppdom::NodeListIterator   mNextSample_i;    /**< Iterator pointing to the next sample to play */
   cppdom::NodeListIterator   mEndSample_i;     /**< The end of the sample list to play */

   gadget::DigitalInterface   mStartStopButton;    /**< Button for stopping and starting the logger */
   gadget::DigitalInterface   mStampButton;        /**< Button for setting a stamp */
};

} // namespace gadget


#endif

