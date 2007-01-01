/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_INPUT_LOGGER_H_
#define _GADGET_INPUT_LOGGER_H_

#include <gadget/gadgetConfig.h>
#include <vector>

#include <boost/smart_ptr.hpp>
#include <cppdom/cppdom.h>

#include <vpr/vpr.h>
#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Type/DigitalInterface.h>
#include <gadget/InputLoggerPtr.h>


namespace gadget
{

/** \class InputLogger InputLogger.h gadget/InputLogger.h
 *
 * Input data logger.
 */
class GADGET_CLASS_API InputLogger
{
public:
   InputLogger()
      : mCurState(Inactive), mSleepFramesLeft(0), mLimitFrameRate(false)
   {;}

   /** Destructor. */
   virtual ~InputLogger()
   {;}

   /** Configures the logger. */
   virtual bool config(jccl::ConfigElementPtr element);

   /**
    * Processing function.
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

   /** Collect a stamp in the recording. */
   void stampRecord();
   //@}

public:
   /** @name Playback interface */
   //@{
   /**
    * Load a log file.
    * @param logFilename The name of the log file to load.
    */
   void load(std::string logFilename);

   /** Play the currently active log. */
   void play();

   /** Stop playing a log. */
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

   /**
    * Limit the framerate to the speed configured.
    *
    * @pre mLimitFrameRate == true
    * @post Method will sleep until enough time passed to slow to configured
    *       framerate.
    */
   void limitFramerate();
   //@}

   /**
    * Eliminates duplicates in the current sampled data.
    * It ignores the timestamp attribute.
    */
   void compressSamples();

public:
   /** List of states that the Logger can be in. */
   enum State
   {
      Inactive,   /**< The Logger is Inactive to be told what to do (i.e., doing nothing). */
      Recording,  /**< The Logger is current recording. */
      Playing,    /**< The Logger is currently playing. */
      Paused      /**< The Logger is currently paused. */
   };

private:
   State       mCurState;        /**< The current state of the logger */

   /**
    * When the state becomes inactive, this is the number of frames that
    * we wait before processing any input.
    */
   unsigned    mSleepFramesLeft;
   std::string mActiveStamp;     /**< The active stamp for this frame */

   unsigned                   mCompressFactor;  /**< Compression factor.  This is the number of duplicate frames to allow in a row */
   std::vector<std::string>   mIgnoreElems;     /**< Elements to ignore when compressing */
   std::vector<std::string>   mIgnoreAttribs;   /**< Attributes to ignore when compressing */

   cppdom::NodePtr   mRootNode;           /**< Root node of the data */
   std::string       mRecordingFilename;  /**< Filename to use for the recording */

   bool              mLimitFrameRate;     /**< Flag. true - we should limit the framerate while logging */
   vpr::Interval     mMinFrameTime;       /**< Minimum time we are to allow a frame to take */
   vpr::Interval     mPrevFrameTimestamp; /**< Timestamp to use for calculating the current frame rate */

   cppdom::NodeListIterator   mNextSample_i;    /**< Iterator pointing to the next sample to play */
   cppdom::NodeListIterator   mEndSample_i;     /**< The end of the sample list to play */

   gadget::DigitalInterface   mStartStopButton;    /**< Button for stopping and starting the logger */
   gadget::DigitalInterface   mStampButton;        /**< Button for setting a stamp */
};

} // namespace gadget


#endif
