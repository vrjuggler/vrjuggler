/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
#include <jccl/Config/ConfigElement.h>

//#include <gadget/Type/PositionFilter.h>
//#include <gadget/Type/LinearSigmoidPositionFilter.h>
#include <gadget/Util/Debug.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>

#include <gadget/Filter/Position/PositionFilter.h>
#include <gadget/Filter/Position/PositionFilterFactory.h>
#include <gadget/Type/Position/PositionUnitConversion.h>
#include <gadget/Type/PositionProxy.h>

namespace gadget
{

std::string PositionProxy::getElementType()
{
   return "position_proxy";
}

bool PositionProxy::config(jccl::ConfigElementPtr element)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
      std::string("------------------ Position PROXY config() -----------------\n"),
      std::string("\n"));
   vprASSERT(element->getID() == getElementType());

   if ( ! Proxy::config(element) )
   {
      return false;
   }

   mUnitNum = element->getProperty<int>("unit");
   mDeviceName = element->getProperty<std::string>("device");

   // --- Configure filters --- //
   unsigned num_filters = element->getNum("position_filters");

   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_VERB_LVL,
                        std::string("PositionProxy::config: ") +
                           element->getName() + std::string(":") +
                           element->getID() + std::string("\n"),
                        std::string("PositionProxy::config: done.\n") );

   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "Num filters: " << num_filters << std::endl << vprDEBUG_FLUSH;

   jccl::ConfigElementPtr cur_filter;
   PositionFilter* new_filter(NULL);

   for(unsigned i=0;i<num_filters;++i)
   {
      cur_filter = element->getProperty<jccl::ConfigElementPtr>("position_filters",i);
      vprASSERT(cur_filter.get() != NULL);

      std::string filter_id = cur_filter->getID();
      vprDEBUG( vprDBG_ALL, vprDBG_VERB_LVL)
         << "   Filter [" << i << "]: Type:" << filter_id << std::endl
         << vprDEBUG_FLUSH;

      new_filter = PositionFilterFactory::instance()->createObject(filter_id);
      if(new_filter != NULL)
      {
         new_filter->config(cur_filter);
         mPositionFilters.push_back(new_filter);
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "   NULL Filter!!!" << std::endl << vprDEBUG_FLUSH;
      }
   }  // if have filters

   // --- SETUP PROXY with INPUT MGR ---- //
   refresh();

   return true;
}

// Deprecated, don't use
gmtl::Matrix44f PositionProxy::getData(float scaleFactor) const
{
   gmtl::Matrix44f ret_mat;

   if(mStupefied)
   {
      //gmtl::identity(mPositionData.mPosData);
      //ret_mat = mPositionData.mPosData;
      gmtl::identity(ret_mat);
   }
   else if(gmtl::Math::isEqual(scaleFactor, 1.0f, 0.01f))
   {
      ret_mat = mPositionData.mPosData;
   }
   else if(gmtl::Math::isEqual(scaleFactor, gadget::PositionUnitConversion::ConvertToFeet, 0.01f))
   {
      ret_mat = mPosMatrix_feet;
   }
   else  // Convert using scale factor
   {
      ret_mat = mPositionData.getPosition();
      gmtl::Vec3f trans;
      gmtl::setTrans(trans, ret_mat);           // Get the translational vector
      trans *= scaleFactor;                     // Scale the translation and set the value again
      gmtl::setTrans(ret_mat, trans);
   }

   return ret_mat;
}


void PositionProxy::updateData()
{
   if((!mStupefied) && (mTypedDevice != NULL))
   {
      mPositionData = (mTypedDevice->getPositionData (mUnitNum));

      // Create a vector to hold all 1 of our position data samples.
      std::vector<PositionData> temp_sample(1, mPositionData);

      // Apply all the positional filters
      for(std::vector<PositionFilter*>::iterator i = mPositionFilters.begin(); i != mPositionFilters.end(); ++i)
      {
          (*i)->apply(temp_sample);
      }

      // Now that the filters have been applied to our sample, copy it back
      // over to mPositionData.
      mPositionData = temp_sample[0];

      // --- CACHE FEET Scaling ---- //
      mPosMatrix_feet = mPositionData.getPosition();
      gmtl::Vec3f trans;                                       // SCALE: to feet
      gmtl::setTrans(trans, mPosMatrix_feet);                  // Get the translational vector
      trans *= gadget::PositionUnitConversion::ConvertToFeet;  // Scale the translation and set the value again
      gmtl::setTrans(mPosMatrix_feet, trans);
   }
}

} // End of gadget namespace
