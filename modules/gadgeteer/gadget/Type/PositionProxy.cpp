/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>

#include <jccl/Config/ConfigElement.h>

#include <gadget/Util/Debug.h>
#include <gadget/Filter/Position/PositionFilter.h>
#include <gadget/Filter/Position/PositionFilterFactory.h>
#include <gadget/Type/Position/PositionUnitConversion.h>
#include <gadget/Type/PositionProxy.h>

namespace gadget
{

PositionProxy::PositionProxy(const std::string& deviceName, const int unitNum)
   : base_type(deviceName, unitNum)
{
   /* Do nothing. */ ;
}

PositionProxyPtr PositionProxy::create(const std::string& deviceName,
                                       const int unitNum)
{
   return PositionProxyPtr(new PositionProxy(deviceName, unitNum));
}

PositionProxy::~PositionProxy()
{
}

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

   if (! base_type::config(element))
   {
      return false;
   }

   // --- Configure filters --- //
   const unsigned int num_filters = element->getNum("position_filters");

   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_VERB_LVL,
                        std::string("PositionProxy::config: ") +
                           element->getName() + std::string(":") +
                           element->getID() + std::string("\n"),
                        std::string("PositionProxy::config: done.\n") );

   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "Num filters: " << num_filters
                                         << std::endl << vprDEBUG_FLUSH;

   jccl::ConfigElementPtr cur_filter;
   PositionFilterPtr new_filter;

   for (unsigned int i = 0; i < num_filters; ++i)
   {
      cur_filter =
         element->getProperty<jccl::ConfigElementPtr>("position_filters", i);
      vprASSERT(cur_filter.get() != NULL);

      const std::string filter_id = cur_filter->getID();
      vprDEBUG( vprDBG_ALL, vprDBG_VERB_LVL)
         << "   Filter [" << i << "]: Type:" << filter_id << std::endl
         << vprDEBUG_FLUSH;

      new_filter =
         PositionFilterPtr(
            PositionFilterFactory::instance()->createObject(filter_id)
         );

      if (NULL != new_filter.get())
      {
         if (new_filter->config(cur_filter))
         {
            mPositionFilters.push_back(new_filter);
         }
         else
         {
            vprDEBUG( vprDBG_ERROR, vprDBG_CONFIG_STATUS_LVL )
               << "   Filter [" << i << "]: Type:" << filter_id
               << " configuration failed; " << filter_id << " will NOT "
               << "be loaded.\n"
               << vprDEBUG_FLUSH;

            new_filter.reset();
         }
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "   NULL Filter!!!"
                                               << std::endl << vprDEBUG_FLUSH;
      }
   }  // if have filters

   // --- SETUP PROXY with INPUT MGR ---- //
   refresh();

   return true;
}

// Deprecated, don't use
const gmtl::Matrix44f PositionProxy::getData(const float scaleFactor) const
{
   gmtl::Matrix44f ret_mat;

   if (! mStupefied)
   {
      if (gmtl::Math::isEqual(scaleFactor, PositionUnitConversion::ConvertToFeet, 0.01f))
      {
         ret_mat = mPosMatrix_feet;
      }
      else  // Convert using scale factor
      {
         applyScaleFactor(mData.getValue(), scaleFactor, ret_mat);
      }
   }

   return ret_mat;
}

void PositionProxy::applyScaleFactor(const gmtl::Matrix44f& xform,
                                     const float scaleFactor,
                                     gmtl::Matrix44f& result) const
{
   if (gmtl::Math::isEqual(scaleFactor, 1.0f, 0.01f))
   {
      result = xform;
   }
   else
   {
      gmtl::Vec3f trans;
      gmtl::setTrans(trans, xform);     // Get the translational vector
      trans *= scaleFactor;             // Scale the translation
      gmtl::setTrans(result, trans);    // Store the result
   }
}

void PositionProxy::updateData()
{
   if (! mStupefied && NULL != mTypedDevice.get())
   {
      // Make sure dependencies are updated.
      getProxiedInputDevice()->updateDataIfNeeded();

      // Applly the filters to our sample and store it in mData.
      mData = applyFilters(mTypedDevice->getPositionData(mUnit));

      // --- CACHE FEET Scaling ---- //
      mPosMatrix_feet = mData.getValue();
      gmtl::Vec3f trans;                                       // SCALE: to feet
      gmtl::setTrans(trans, mPosMatrix_feet);                  // Get the translational vector
      trans *= PositionUnitConversion::ConvertToFeet;          // Scale the translation and set the value again
      gmtl::setTrans(mPosMatrix_feet, trans);
   }
}

const PositionData
PositionProxy::applyFilters(const PositionData& posData) const
{
   // Create a vector to hold all 1 of our position data samples.
   std::vector<PositionData> temp_sample(1, posData);

   // Apply all the positional filters.
   std::for_each(mPositionFilters.begin(), mPositionFilters.end(),
                 boost::bind(&PositionFilter::apply, _1,
                             boost::ref(temp_sample)));

   // Now that the filters have been applied to our sample, return it.
   return temp_sample[0];
}

} // End of gadget namespace
