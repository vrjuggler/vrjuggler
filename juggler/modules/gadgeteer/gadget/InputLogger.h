#ifndef _GADGET_INPUT_LOGGER_H_
#define _GADGET_INPUT_LOGGER_H_

#include <gadget/gadgetConfig.h>
#include <vpr/vpr.h>
#include <jccl/Config/ConfigChunkPtr.h>

#include <gadget/Type/DigitalInterface.h>
#include <cppdom/cppdom.h>

namespace gadget
{

/** Input data logger
*/
class InputLogger
{
public:
   InputLogger()
      : mCurState(Inactive)
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

   /** @name Recording interface */
   //@{
   void startRecording();

   void stopRecording();

   /** Collect a stamp in the recording */
   void stampRecord();
   //@}

protected:
   /** @name Internal helpers */
   //@{

   void addRecordingSample();
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
   State       mCurState;     /**< The current state of the logger */
   std::string mActiveStamp;  /**< The active stamp for this frame */

   cppdom::NodePtr   mRootNode;           /**< Root node of the data */
   std::string       mRecordingFilename;  /**< Filename to use for the recording */

   gadget::DigitalInterface   mStartStopButton;    /**< Button for stopping and starting the logger */
   gadget::DigitalInterface   mStampButton;        /**< Button for setting a stamp */
};

} // namespace gadget


#endif

