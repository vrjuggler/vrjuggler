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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
#ifndef _GADGET_POSITION_FILTER_H_
#define _GADGET_POSITION_FILTER_H_

#include <gadget/gadgetConfig.h>
#include <typeinfo>
#include <vector>

#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Type/PositionData.h>
//#include <gadget/Type/SampleBuffer.h>

#include <gmtl/Matrix.h>
#include <vpr/Util/Debug.h>


namespace gadget
{

/** \class PositionFilter PositionFilter.h gadget/Filter/Position/PositionFilter.h
 *
 * PositionFilter is the abstract base class that all position filters derive
 * from.
 */
class GADGET_CLASS_API PositionFilter
{
public:
//   typedef gadget::SampleBuffer<PositionData> SampleBuffer_t;   

public:
   /** Constructor */
   PositionFilter()
   {;}

   /** Destructor */
   virtual ~PositionFilter()
   {;}

   /**
    * Configuration for the filter.
    * @return Returns true if configured correctly.
    */
   virtual bool config(jccl::ConfigElementPtr c) = 0;   

   /**
    * Applies the position filter to the given sample in place.
    *
    * @post posSample is updated with a modified version.
    *
    * @param posSample The sample to modify in place.
    */
   virtual void apply(std::vector< PositionData >& posSample) = 0;

   /**
    * Returns the string rep of the element type used to config this device.
    * This string is used by the device factory to look up device drivers
    * based up the type of element it is trying to load.
    */
   static std::string getElementType()
   {
      return std::string("UndefinedPosFilter");
   }

};

} // End of gadget namespace

#endif
