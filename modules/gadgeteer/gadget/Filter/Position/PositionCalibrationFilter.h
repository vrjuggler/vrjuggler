/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef _GADGET_POSITION_CALIBRATION_FILTER_H_
#define _GADGET_POSITION_CALIBRATION_FILTER_H_

#include <gadget/gadgetConfig.h>

#include <map>
#include <string>
#include <vector>

#include <gmtl/Vec.h>

#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Type/PositionData.h>
#include <gadget/Util/Debug.h>
#include <gadget/Filter/Position/PositionFilter.h>

namespace gadget
{
   /** \class PositionCalibrationFilter PositionCalibrationFilter.h gadget/Filter/Position/PositionCalibrationFilter.h 
    *
    * A calibration filter that uses a calibration table to correct 
    * electromagnetic interference error from the tracker.
    */
   class GADGET_CLASS_API PositionCalibrationFilter : public PositionFilter
   {
   public:
      
      /**
       * Default Ctor
       */
      PositionCalibrationFilter()
         : mWMatrix(NULL)
      {}

      ~PositionCalibrationFilter();

      bool config(jccl::ConfigElementPtr e);

      void apply(std::vector< PositionData >& posSample);

      static std::string getElementType();
      
   private:

      /** The calibration table. */
      std::vector< std::pair< gmtl::Vec3d, gmtl::Vec3d > > mTable;

      /** The name of the calibration file. */
      std::string mFileName;

      /** The "W" matrix.
       * The "W" Matrix is an NxN matrix where N = mTable.size()
       * It is composed of elements computed using the w(p) function:
       * w[j](p) = sqrt( length(p - p[j]) + R*R )
       * where 10 <= R*R <= 1000.
       * The matrix looks like:
       * ( w[1](p[1]) w[2](p[1]) w[3](p[1]) ... w[N](p[1]) )
       * ( w[1](p[2]) w[2](p[2]) w[3](p[2]) ... w[N](p[2]) )
       * ( w[1](p[3]) w[2](p[3]) w[3](p[3]) ... w[N](p[3]) )
       * (                      .                          )
       * (                      .                          )
       * (                      .                          )
       * ( w[1](p[N]) w[2](p[N]) w[3](p[N]) ... w[N](p[N]) )
       */
      double** mWMatrix;

      /** The Alpha Vector. */
      std::vector<gmtl::Vec3d> mAlphaVec;
   };
}
#endif
