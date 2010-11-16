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

#ifndef _VRJ_TEST_MESSAGE_H_
#define _VRJ_TEST_MESSAGE_H_

#include <vector>
#include <string>
#include <sstream>

namespace vrj
{

namespace test
{

/** \class Message Message.h vrj/Test/Message.h
 *
 * Message with extra details.
 *
 * Really just a wrapper around a vector of std::strings
 * that describe the type of problem encountered
 */
class Message
{
public:
   Message()
   {;}

   explicit Message(const std::string& shortDesc)
      : mShortDesc(shortDesc)
   {
      mShortDesc = shortDesc;
   }

   Message(const std::string& shortDesc, const std::string& detail1)
      : mShortDesc(shortDesc)
   {
      addDetail(detail1);
   }

   Message(const std::string &shortDesc, const std::string &detail1,
           const std::string &detail2)
      : mShortDesc(shortDesc)
   {
      addDetail(detail1, detail2);
   }

   Message(const std::string &shortDesc, const std::string &detail1,
           const std::string &detail2, const std::string &detail3)
     : mShortDesc(shortDesc)
   {
      addDetail(detail1, detail2, detail3);
   }

   /*! \brief Returns the short description.
    * \return Short description.
    */
   const std::string& shortDesc()
   {
      return mShortDesc;
   }

   /** Returns a string that represents a list of the detail strings.
    *
    * Example:
    * \code
    * Message message( "not equal", "Expected: 3", "Actual: 7" );
    * std::string details = message.details();
    * // details contains:
    * // "- Expected: 3\n- Actual: 7\n"  \endcode
    *
    * @return A string that is a concatenation of all the detail strings. Each
    *         detail string is prefixed with '- ' and suffixed with '\n' before
    *         being concatenated to the other.
    */
   std::string details() const
   {
      std::stringstream oss;

      for(unsigned i=0;i<mDetails.size();++i)
      {
         oss << "- " << mDetails[i] << "\n";
      }

      return oss.str();
   }

   /*! \brief Removes all detail strings.
    */
   void clearDetails()
   {
      mDetails.clear();
   }

   /** Returns a list of the details. */
   const std::vector<std::string>& getDetails()
   {
      return mDetails;
   }

   /** Adds a detail. */
   void addDetail(const std::string &detail)
   {
      mDetails.push_back(detail);
   }

   /** Adds two details. */
   void addDetail(const std::string& detail1, const std::string& detail2)
   {
      addDetail(detail1);
      addDetail(detail2);
   }

   /** Adds three details. */
   void addDetail(const std::string &detail1, const std::string &detail2,
                  const std::string &detail3)
   {
      addDetail(detail1);
      addDetail(detail2);
      addDetail(detail3);
   }

   /** Sets the short description. */
   void setShortDesc(const std::string& shortDesc)
   {
      mShortDesc = shortDesc;
   }

private:
  std::string              mShortDesc;    /**< The short description of the message */
  std::vector<std::string> mDetails;      /**< The associated details */
};

}

}


#endif
