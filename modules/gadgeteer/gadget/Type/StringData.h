/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _GADGET_STRING_DATA_H_
#define _GADGET_STRING_DATA_H_

#include <stdlib.h>
#include <string>

#include <gadget/Type/InputData.h>

namespace gadget
{

/** \class StringData StringData.h gadget/Type/StringData.h
 *
 * InputData subclass for string data.
 */
class StringData : public InputData
{
   // Hack around a GCC 3.3 bug on Mac OS X 10.3 that shows up with
   // boost::is_polymorphic.
#if defined(__MACH__) && defined(__APPLE_CC__) && defined(__GNUC__) && \
    __GNUC__ == 3 && __GNUC_MINOR__ == 3
   bool dummy_;
#endif

public:
   /** Constructor. */
   StringData()
      : InputData()
      , mStringData("")
   {
      ;
   }

   StringData(const std::string& s)
      : InputData()
      , mStringData(s)
   {
      ;
   }

   const std::string& getString() const
   {
      return mStringData;
   }

   void setString(const std::string& s)
   {
      mStringData = s;
   }

   StringData& operator= (const StringData& pd)
   {
      InputData::copy (pd);
      mStringData = pd.mStringData;
      return *this;
   }

   StringData& operator= (const std::string& data)
   {
      mStringData = data;
      return *this;
   }

   bool operator== (const StringData& o) const
   {
      return mStringData == o.mStringData && mTimeStamp == o.mTimeStamp;
   }

   bool operator!= (const StringData& o) const
   {
      return ! (*this == o);
   }

protected:
   std::string mStringData;
}; // class StringData

} // namespace gadget


#endif /* _GADGET_STRING_DATA_H_ */
