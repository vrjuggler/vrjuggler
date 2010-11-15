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

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Output.h>

#include <jccl/Config/ConfigElement.h>

#include <gadget/Filter/Position/PositionFilter.h>
#include <gadget/Filter/Position/PositionFilterFactory.h>
#include <gadget/Filter/Position/FullPositionXformFilter.h>


namespace gadget
{

// Register this filter.
GADGET_REGISTER_POSFILTER_CREATOR(FullPositionXformFilter);

FullPositionXformFilter::FullPositionXformFilter()
{
   /* Do nothing. */ ;
}

/** Destructor. */
FullPositionXformFilter::~FullPositionXformFilter()
{
   /* Do nothing. */ ;
}

std::string FullPositionXformFilter::getElementType()
{
   return "full_position_transform_filter";
}

bool FullPositionXformFilter::config(jccl::ConfigElementPtr e)
{
   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_VERB_LVL,
                        std::string("[FullPositionXformFilter::config()]") +
                           e->getFullName() + std::string(":") +
                           e->getID() + std::string("\n"),
                        std::string("[PositionXformFilter::config9)] Done.\n"));

   vprASSERT(e->getID() == FullPositionXformFilter::getElementType());

   // These are the transforms from the base tracker coord system
   mPreXform[0][0] = e->getProperty<float>("pre_xform", 0);
   mPreXform[0][1] = e->getProperty<float>("pre_xform", 1);
   mPreXform[0][2] = e->getProperty<float>("pre_xform", 2);
   mPreXform[0][3] = e->getProperty<float>("pre_xform", 3);
   mPreXform[1][0] = e->getProperty<float>("pre_xform", 4);
   mPreXform[1][1] = e->getProperty<float>("pre_xform", 5);
   mPreXform[1][2] = e->getProperty<float>("pre_xform", 6);
   mPreXform[1][3] = e->getProperty<float>("pre_xform", 7);
   mPreXform[2][0] = e->getProperty<float>("pre_xform", 8);
   mPreXform[2][1] = e->getProperty<float>("pre_xform", 9);
   mPreXform[2][2] = e->getProperty<float>("pre_xform", 10);
   mPreXform[2][3] = e->getProperty<float>("pre_xform", 11);
   mPreXform[3][0] = e->getProperty<float>("pre_xform", 12);
   mPreXform[3][1] = e->getProperty<float>("pre_xform", 13);
   mPreXform[3][2] = e->getProperty<float>("pre_xform", 14);
   mPreXform[3][3] = e->getProperty<float>("pre_xform", 15);

   mPostXform[0][0] = e->getProperty<float>("post_xform", 0);
   mPostXform[0][1] = e->getProperty<float>("post_xform", 1);
   mPostXform[0][2] = e->getProperty<float>("post_xform", 2);
   mPostXform[0][3] = e->getProperty<float>("post_xform", 3);
   mPostXform[1][0] = e->getProperty<float>("post_xform", 4);
   mPostXform[1][1] = e->getProperty<float>("post_xform", 5);
   mPostXform[1][2] = e->getProperty<float>("post_xform", 6);
   mPostXform[1][3] = e->getProperty<float>("post_xform", 7);
   mPostXform[2][0] = e->getProperty<float>("post_xform", 8);
   mPostXform[2][1] = e->getProperty<float>("post_xform", 9);
   mPostXform[2][2] = e->getProperty<float>("post_xform", 10);
   mPostXform[2][3] = e->getProperty<float>("post_xform", 11);
   mPostXform[3][0] = e->getProperty<float>("post_xform", 12);
   mPostXform[3][1] = e->getProperty<float>("post_xform", 13);
   mPostXform[3][2] = e->getProperty<float>("post_xform", 14);
   mPostXform[3][3] = e->getProperty<float>("post_xform", 15);

   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
      << "m:preXform [T*R]:\n" << mPreXform << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
      << "m:postXform [T*R]:\n" << mPostXform << std::endl << vprDEBUG_FLUSH;

   return true;
}

void FullPositionXformFilter::apply(std::vector<PositionData>& posSamples)
{
   typedef std::vector<PositionData>::iterator iter_type;
   for ( iter_type i = posSamples.begin(); i != posSamples.end(); ++i )
   {
      gmtl::Matrix44f& pos_data((*i).editValue());
      // POST xform: cur = cur * postTrans * postRot
      gmtl::postMult(pos_data, mPostXform);

      // PRE: S_world = wMs * S_sensor
      gmtl::preMult(pos_data, mPreXform);
   }
}

} // End of gadget namespace
