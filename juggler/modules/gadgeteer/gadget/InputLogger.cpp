
#include <gadget/InputLogger.h>

#include <gadget/gadgetConfig.h>
#include <vpr/vpr.h>
#include <jccl/Config/ConfigChunk.h>

#include <vpr/IO/XMLObjectWriter.h>
#include <vpr/IO/XMLObjectReader.h>

#include <fstream>

#include <cppdom/cppdom.h>
#include <stdio.h>

/*

--- Input logger format ---
The logger format is made up of a list of samples.
For each sample there is a list of devices that got sampled.

Each of these devices has an individual element with that device name
and then the contents are an XML serialized version of that device
and it's data (this includes the buffer).

There may be logger_stamp's intermixed between logger_samples at any point.
The stamps only become valid after a logger sample (ie. they stamp the
previous sample).

Note: I could probably write a DTD for this, but I haven't taken the time
to do this yet. :(
--------------------------

<gadget_logger>

   <sample>
      <device dev_name="device name">
        XML serialized version of that device
         <devroot...>
         </devroot...>
      </device>
      * <device>
   </sample>
   <stamp id="log_name"/>   // Not needed after all logger_sample's

   <sample>
   ...

</gadget_logger>
*/

namespace gadget
{

/** Configure the logger */
bool InputLogger::config( jccl::ConfigChunkPtr chunk)
{
   std::string start_name = chunk->getProperty<std::string>("start_digital");
   std::string stamp_name = chunk->getProperty<std::string>("stamp_digital");

   mStartStopButton.init(start_name);
   mStampButton.init(stamp_name);

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL) << "\n--- LOGGER: Configured ---\n" << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL) << "StartStop: " << start_name << std::endl
                                                     << "Stamp: " << stamp_name << std::endl << vprDEBUG_FLUSH;

   return true;
}

/** Do input logging processing.
*
* if(!playing)    // don't check for my keys during playback
*    If start/stop pressed
*       If recording
*          Close recording
*       Else
*          Start recording
*    else if(stamp pressed) && recording
*       collect a stamp
* if(recording)
*    Add sample
* else if(playing)
*    Playback next sample
*/
void InputLogger::process()
{
   // Get state of control keys
   // Note, because of how this is called this sees the state one frame late
   // ie. If toggle on is true here it is NOT true in the data being saved
   const bool start_stop_triggered(mStartStopButton->getData() == Digital::TOGGLE_ON);
   const bool stamp_triggered(mStampButton->getData() == Digital::TOGGLE_ON);

   if(0 == mSleepFramesLeft)
   {
      // Check for control keys
      if(Playing != mCurState)
      {
         if(start_stop_triggered)
         {
            if(Recording == mCurState)
            {  stopRecording(); }
            else
            {  startRecording(); }
         }
         else if(stamp_triggered && (Recording == mCurState))
         {  stampRecord(); }
      }
   
      if(Recording == mCurState)
      {
         addRecordingSample();
      }
      else if(Playing == mCurState)
      {
         playNextSample();
      }
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "InputLogger: Sleeping: " << mSleepFramesLeft << std::endl << vprDEBUG_FLUSH;                              
      mSleepFramesLeft--;
   }
}

/* Starting the recording and initialize all necessary stuff
*
* - Get the output filename
* - Initialize root node
* - Set to recording state
*/
void InputLogger::startRecording()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "\n--- LOGGER: startRecording ---\n" << vprDEBUG_FLUSH;

   // -- Get recording filename
   std::cout << "/n/n------- LOGGER ------\nEnter log filename:" << std::flush;
   std::string file_name;
   //std::string file_name("test_logging.xml");
   std::cin >> file_name;
   std::cout << "\nUsing file: " << file_name << std::endl;

   mRecordingFilename = file_name;

   // -- Init recording
   cppdom::ContextPtr ctx( new cppdom::Context );
   mRootNode = cppdom::NodePtr(new cppdom::Node("gadget_logger", ctx ));

   mCurState = Recording;
}

void InputLogger::stopRecording()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "\n--- LOGGER: stopRecording ---\n" << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "Done recording\n"
                                    << "Saving data to: " << mRecordingFilename << std::endl << vprDEBUG_FLUSH;
   std::ofstream out_file;
   out_file.exceptions(std::ofstream::badbit | std::ofstream::failbit);
   try
   {
      out_file.open(mRecordingFilename.c_str());
      mRootNode->save(out_file);
      out_file.flush();
      out_file.close();
   }
   catch(std::ofstream::failure& se)
   {
      std::cerr << "IOS failure saving file: desc:" << se.what() << std::endl;
      if(out_file.is_open())
      {  out_file.close(); }
   }
   catch(...)
   {
      std::cerr << "Unknown error saving file." << std::endl;
   }

   mCurState = Inactive;
   mSleepFramesLeft = 10;     // Wait 10 frames until we start processing anything again
}

void InputLogger::stampRecord()
{
   vprASSERT(Recording == mCurState && "Tried to stamp input while not recording");

   // -- Get recording filename
   char tag_name[1024];
   std::cout << "/n/n------- LOGGER ------\nEnter stamp id:" << std::flush;
   std::string stamp_id;
   //std::cin >> stamp_id;
   scanf("%s", tag_name);
   stamp_id = std::string(tag_name);
   std::cout << "\nStamping with: " << stamp_id << std::endl;

   cppdom::NodePtr stamp_node(new cppdom::Node("stamp", mRootNode->getContext()));
   stamp_node->setAttribute("id", stamp_id);

   mRootNode->addChild(stamp_node);
}

/** Load a log file.
* @param logFilename - The name of the log file to load
*/
void InputLogger::load(std::string logFilename)
{
   vprASSERT(Inactive == mCurState && "Tried to load a file while logger is active");

   std::cout << "InputLogger: Loading file: " << logFilename << std::endl;

   // Create new root to read into
   cppdom::ContextPtr ctx( new cppdom::Context );
   mRootNode = cppdom::NodePtr(new cppdom::Node("not_set", ctx ));

   std::ifstream in_file;
   in_file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
   try
   {
      in_file.open(logFilename.c_str(), std::ios::in);
      mRootNode->load(in_file, ctx);
      in_file.close();
   }
   catch(std::ifstream::failure& se)
   {
      std::cerr << "IOS failure saving file: desc:" << se.what() << std::endl;
      if(in_file.is_open())
      {  in_file.close(); }
   }
   catch(...)
   {
      std::cerr << "Unknown error loading file." << std::endl;
   }

   vprDEBUG(vprDBG_ALL,0) << "InputLogger: Loaded file: num_samples:" << mRootNode->getChildren().size() << std::endl << vprDEBUG_FLUSH;
}

/* Start playing
*
* - Set state to playing
* - Find the first node is list
* - Set the start and end nodes
* - Clear the stamp
*/
void InputLogger::play()
{
   vprASSERT(mCurState == Inactive);

   if(mRootNode.get() == NULL)
   {
      vprDEBUG(vprDBG_ALL,0) << "Logger::play: Null root node, so can't play.\n" << vprDEBUG_FLUSH;
      return;
   }

   mNextSample_i = mRootNode->getChildren().begin();
   mEndSample_i = mRootNode->getChildren().end();
   mActiveStamp.clear();

   if(mNextSample_i == mEndSample_i)
   {
      vprDEBUG(vprDBG_ALL,0) << "Logger::play: Zero children, so can't play.\n" << vprDEBUG_FLUSH;
      return;
   }

   mCurState = Playing;
}

/** Stop playing a log */
void InputLogger::stop()
{;
}

/** Pause log playback. */
void InputLogger::pause()
{;
}

/** Get the stamp for the most recent sample
* @return Returns empty string if no active stamp.
*/
std::string InputLogger::getStamp()
{
   return mActiveStamp;
}


/** Add a recording sample to the current dom tree
*
* - Create the sample node
* - For each device in input manager
*    - Create device node for the device (named after device)
*    - Serialize device into a node
*    - Add serialized dev node as child of dev node
*    - Add dev node as child of sample node
* - Add sample node as child of root node
*/
void InputLogger::addRecordingSample()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL) << "LOGGER: adding sample.\n" << vprDEBUG_FLUSH;

   try
   {
      gadget::InputManager* input_mgr = gadget::InputManager::instance();

      cppdom::NodePtr sample_node(new cppdom::Node("sample", mRootNode->getContext()));

      // For each device
      for(InputManager::tDevTableType::iterator dev_i=input_mgr->mDevTable.begin();
          dev_i != input_mgr->mDevTable.end(); ++dev_i)
      {
         gadget::Input* cur_dev = (*dev_i).second;
         std::string dev_name = cur_dev->getInstanceName();

         cppdom::NodePtr dev_node(new cppdom::Node("device", mRootNode->getContext()));
         dev_node->setAttribute("dev_name", dev_name);

         vpr::XMLObjectWriter xml_writer;       // Create writer for the device
         cur_dev->writeObject(&xml_writer);
         cppdom::NodePtr serialized_dev_node = xml_writer.getRootNode();
         vprASSERT(serialized_dev_node.get() != NULL);

         dev_node->addChild(serialized_dev_node);  // Add the child node
         sample_node->addChild(dev_node);          // Add dev node to the sample
      }

      mRootNode->addChild(sample_node);
   }
   catch (cppdom::Error& ce)
   {
      std::cerr << "Cppdom Error [InputLogger::addRecordingSample]: " << ce.getString() << std::endl;
      std::cerr << "Info: " << ce.getInfo() << std::endl;
      vprASSERT(false);
   }
}

/*
* - Get the node for the next sample
* - For each device element
*    - Get the device name
*    - Get handle to that device from the input manager
*    - Deserialize the device
*
* - Increment next sample
* - If nextSample is a stamp element
*    - Read and set the stamp
*    - Increment next sample
* - Else, clear the stamp
*/
void InputLogger::playNextSample()
{
   vprDEBUG_OutputGuard(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL, "InputLogger::playNextSample\n", "done playing sample\n");

   vprASSERT(mNextSample_i != mEndSample_i && "Overran the logger sample list");
   gadget::InputManager* input_mgr = gadget::InputManager::instance();

   cppdom::NodePtr next_node = (*mNextSample_i);
   vprASSERT(next_node->getName() == std::string("sample") && "Didn't get element of name sample");

   // For each device element
   cppdom::NodeList dev_nodes = next_node->getChildren();
   for(cppdom::NodeListIterator cur_dev_node=dev_nodes.begin(); cur_dev_node != dev_nodes.end(); ++cur_dev_node)
   {
      vprASSERT((*cur_dev_node)->getName() == std::string("device"));
      std::string dev_name = (*cur_dev_node)->getAttribute("dev_name").getValue<std::string>();
      cppdom::NodePtr serial_dev_node = *((*cur_dev_node)->getChildren().begin());
      vprASSERT(serial_dev_node.get() != NULL && "Got null serialized device node");

      gadget::Input* dev_ptr = input_mgr->getDevice(dev_name);
      if(NULL != dev_ptr)
      {
         vprDEBUG_OutputGuard(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL, 
                              std::string("Reading device: ") + dev_name + std::string("\n"), "done reading");

         vpr::XMLObjectReader xml_reader(serial_dev_node);     // Create XML reader
         xml_reader.setAttrib("rim.timestamp.delta", 0);       // Hack for now to work around RIM
         dev_ptr->readObject(&xml_reader);                     // Deserialize the device
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_WARNING_LVL) << "Skipping device: [" << dev_name
                                         << "]  Could not find it.\n" << vprDEBUG_FLUSH;
      }
   }

   // Increment next sample
   mNextSample_i++;
   mActiveStamp.clear();      // Clear the stamp for now

   if(mNextSample_i != mEndSample_i)
   {
      if( (*mNextSample_i)->getName() == std::string("stamp"))
      {
         mActiveStamp = (*mNextSample_i)->getAttribute("id").getValue<std::string>();
         mNextSample_i++;
         vprDEBUG(gadgetDBG_INPUT_MGR,0) << "Logger: Got stamp: [" << mActiveStamp << "]\n" << vprDEBUG_FLUSH;
      }
   }

   if(mNextSample_i == mEndSample_i)   // If done playing
   {
      mCurState = Inactive;
      mSleepFramesLeft = 10;     // Wait 10 frames until we start processing anything again
      vprDEBUG(gadgetDBG_INPUT_MGR,0) << "Logger: Done playing.\n" << vprDEBUG_FLUSH;
   }
   
}

} // namespace gadget

