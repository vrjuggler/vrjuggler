/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_SIM_DISPLAY_H_
#define _VRJ_SIM_DISPLAY_H_
//#pragma once

#include <vrj/vrjConfig.h>
//#include <vrj/Math/Coord.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Viewport.h>
#include <gadget/Type/PositionInterface.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>

#include <jccl/Config/ConfigChunkPtr.h>

namespace vrj
{

class SimViewport : public Viewport
{
public:
   SimViewport()
   {;}

public:
   /**  Configure the simulator */
   virtual void config(jccl::ConfigChunkPtr chunk);

   /** Update the projections
   * @param positionScale - Scale value for converting from Juggler units (meters) to the display units
   */
   virtual void updateProjections(const float positionScale);

   gmtl::Matrix44f getCameraPos()
   { return mCameraPos; }

   gmtl::Matrix44f getHeadPos()
   { return mHeadPos; }

   gmtl::Matrix44f getWandPos()
   { return mWandPos; }


public:  // Sim Drawing parameters
   bool shouldDrawProjections()
   { return mDrawProjections; }

   gmtl::Vec3f getSurfaceColor()
   { return mSurfaceColor; }

protected:
    /**  Update internal simulator data */
   void updateInternalData(float positionScale);

private:
   // Drawing attributes
   bool   mDrawProjections;       /**< Should we draw projections */
   gmtl::Vec3f   mSurfaceColor;   /**< Color to draw surfaces */

   gadget::PositionInterface mCamera; /**< Proxy interfaces to devices needed */
   gadget::PositionInterface mWand;

   gmtl::Matrix44f    mCameraPos;    /**< The data about the position of all this stuff */
   gmtl::Matrix44f    mHeadPos;
   gmtl::Matrix44f    mWandPos;
};

};
#endif
