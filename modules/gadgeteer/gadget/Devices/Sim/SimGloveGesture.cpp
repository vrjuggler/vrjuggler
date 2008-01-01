/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <gadget/gadgetConfig.h>

#include <fstream>
#include <vpr/Util/FileUtils.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/Devices/Sim/SimGloveGesture.h>


namespace gadget
{

std::string SimGloveGesture::getElementType()
{
   return "simulated_glove_gesture";
}

/**
 * Constructs the SimGloveGesture.
 * Set the keyboard key pairs.
 * Load the sample file.
 * Trim the smallest so they are same length.
 * Find/Set pos proxy for glove.
 */
bool SimGloveGesture::config(jccl::ConfigElementPtr element)
{
   if((!GloveGesture::config(element)) || (!SimInput::config(element)))
   {
      return false;
   }

   mCurGesture = 0;     // We are in no gesture yet

   std::vector<jccl::ConfigElementPtr> key_list;
   int key_count = element->getNum("key_pair");
   for ( int i = 0; i < key_count; ++i )
   {
      key_list.push_back(element->getProperty<jccl::ConfigElementPtr>("key_pair", i));
   }
   mSimKeys = readKeyList(key_list);

   // Get sample filename
   std::string sample_file = element->getProperty<std::string>("trained_filename");
   loadTrainedFile(vpr::replaceEnvVars(sample_file));

   // Trim the lengths
   unsigned int num_gestures = getNumGestures();
   while(num_gestures < mSimKeys.size())     // If we have to many keys
   {
      mSimKeys.pop_back();
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "gadget::SimGloveGesture: Not enough gestures. Trimming"
         << std::endl << vprDEBUG_FLUSH;
   }

   // Find pos proxy
   std::string glove_pos_proxy = element->getProperty<std::string>("glove_position");    // Get the name of the pos_proxy
   if(glove_pos_proxy == std::string(""))
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:") << " SimGloveGesture has no posProxy."
         << std::endl << vprDEBUG_FLUSH;
      return false;
   }
   // init glove proxy interface
   /*
   int proxy_index = Kernel::instance()->getInputManager()->getProxyIndex(glove_pos_proxy);
   if(proxy_index != -1)
      mGlovePos[0] = Kernel::instance()->getInputManager()->getPosProxy(proxy_index);
   else
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:")
         << " SimGloveGesture::CyberGlove: Can't find posProxy."
         << std::endl << std::endl << vprDEBUG_FLUSH;
   */

   // Set the indexes to defaults
   //resetIndexes();

   return true;
}

/**
 * Gets the digital data for the given devNum.
 * Returns digital 0 or 1, if devNum makes sense.
 * Returns -1 if function fails or if devNum is out of range.
 *
 * @note If devNum is out of range, function will fail, possibly issueing
 *       an error to a log or console - but will not ASSERT.
 */
const DigitalData SimGloveGesture::getDigitalData(int devNum)
{
   int openLookupTable[] = { 0,0,0,0,0,-1,0,0,0,0,0 };
   int closedLookupTable[] = { 1,1,1,1,1,-1,1,1,1,1,1 };
   int pointingLookupTable[] = { 1,1,1,0,1,-1,1,1,1,0,1 };
   switch (mCurGesture)
   {
      case 0:
         //open
         mDigitalData = openLookupTable[devNum];
         break;
      case 1:
         //closed
         mDigitalData = closedLookupTable[devNum];
         break;
      case 2:
         // pointing
         mDigitalData = pointingLookupTable[devNum];
         break;
      default:
         mDigitalData = openLookupTable[devNum];
         break;
   }
   return mDigitalData;
}

/**
 * Gets the current gesture.
 * @return id of current gesture
 */
int SimGloveGesture::getGesture() const
{ return mCurGesture; }

/**
 * Updates the device data.
 * Get the gesture id.
 * Set the glove params.
 */
void SimGloveGesture::updateData()
{
/*  TEMPORARILY REMOVE
   // Get the current gesture
   for(unsigned int i=0;i<mSimKeys.size();i++)
   {
      if(checkKeyPair(mSimKeys[i]) > 0)
      {
         mCurGesture = i;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
            << "gadget::SimGloveGesture: Got gesture: "
            << getGestureString(mCurGesture).c_str()
            << std::endl << vprDEBUG_FLUSH;

         // Set the glove to the sample
         mTheData[0][current] = mGestureExamples[mCurGesture];    // Copy over the example
         mTheData[0][current].calcXforms();                       // Update the xform data
      }
   }
*/
}

/**
 * Loads trained data for the gesture object.
 * Loads the file for trained data.
 */
void SimGloveGesture::loadTrainedFile(std::string fileName)
{
   std::ifstream inFile(fileName.c_str());

   if(inFile)
   {
      this->loadFileHeader(inFile);
      inFile.close();                     // Close the file
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "gadget::SimGloveGesture:: Can't load trained file: "
         << fileName.c_str() << std::endl << vprDEBUG_FLUSH;
   }
}

} // End of gadget namespace
