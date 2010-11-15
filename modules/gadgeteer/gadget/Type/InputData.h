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

#ifndef _GADGET_INPUT_DATA_H_
#define _GADGET_INPUT_DATA_H_

#include <gadget/gadgetConfig.h>
#include <vpr/Util/Interval.h>

namespace gadget 
{

/** \class InputData InputData.h gadget/Type/InputData.h
 *
 * Base class for all input data returned by Gadgeteer.
 * This provides all input data with a standard timestamping system used to
 * calculate input latency.
 */
class GADGET_CLASS_API InputData 
{
public:
   /** Constructor. */
   InputData();

   /** Record the current time using the vpr::Interval. */
   void setTime() 
   {
      mTimeStamp.setNow();
   }

   /** Record the given time. */
   void setTime(const vpr::Interval& iv) 
   {
      mTimeStamp = iv;
   }

   /** Return the last marked time using the vpr::Interval. */
   const vpr::Interval& getTime() const
   {
      return mTimeStamp;
   }

protected:
   void copy(const InputData& id) 
   {
      mTimeStamp = id.mTimeStamp;
   }

   bool isEqual(const InputData& id) const
   {
      return mTimeStamp == id.mTimeStamp;
   }

private:
   vpr::Interval mTimeStamp;
}; // class InputData

} // namespace gadget


#endif /* _GADGET_INPUT_DATA_H_ */
