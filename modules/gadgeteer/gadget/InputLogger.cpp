
#include <gadget/InputLogger.h>

#include <gadget/gadgetConfig.h>
#include <vpr/vpr.h>
#include <jccl/Config/ConfigChunk.h>

#include <vpr/IO/XMLObjectWriter.h>
#include <vpr/IO/XMLObjectReader.h>

#include <fstream>

#include <cppdom/cppdom.h>

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

   vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "\n\n--- LOGGER: Configured ---\n" << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "StartStop: " << start_name << std::endl
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
   const bool start_stop_triggered(mStartStopButton->getData() == Digital::TOGGLE_ON);
   const bool stamp_triggered(mStampButton->getData() == Digital::TOGGLE_ON);

   //vprDEBUG(vprDBG_ALL, 0) << "start:" << start_stop_triggered << "  stamp:" << stamp_triggered << std::endl << vprDEBUG_FLUSH;

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
   std::string file_name("test_logging.xml");
   //std::cin >> file_name;
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

   //mRootNode->save(std::cout);

   std::cout << "---- Done recording ----\n"
             << "Saving data to: " << mRecordingFilename << std::endl;
   std::ofstream out_file;
   out_file.exceptions(std::ofstream::badbit | std::ofstream::failbit);
   try
   {
      std::ofstream out_file(mRecordingFilename.c_str());
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
}

void InputLogger::stampRecord()
{
   vprASSERT(Recording == mCurState && "Tried to stamp input while not recording");

   // -- Get recording filename
   std::cout << "/n/n------- LOGGER ------\nEnter stamp id:" << std::flush;
   std::string stamp_id;
   std::cin >> stamp_id;
   std::cout << "\nStamping with: " << stamp_id << std::endl;

   cppdom::NodePtr stamp_node(new cppdom::Node("stamp", mRootNode->getContext()));
   stamp_node->setAttribute("id", stamp_id);

   mRootNode->addChild(stamp_node);
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
   vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "LOGGER: adding sample.\n" << vprDEBUG_FLUSH;

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


} // namespace gadget

