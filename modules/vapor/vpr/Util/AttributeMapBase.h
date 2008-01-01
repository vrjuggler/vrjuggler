/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

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

#ifndef _VPR_ATTRIBUTE_MAP_BASE_H
#define _VPR_ATTRIBUTE_MAP_BASE_H

#include <vpr/vprConfig.h>
#include <map>
#include <string>
#include <boost/any.hpp>


namespace vpr
{

/** \class AttributeMapBase AttributeMapBase.h vpr/Util/AttributeMapBase.h
 *
 * Basic interface for adding attributes to classes.
 *
 * Useful for having custom setter/getter type settings that are only used in
 * certain contexts.
 *
 * This is based upon boost::any type.
 */
class AttributeMapBase
{
public:
   /**
    * Sets an attribute to the given value.
    *
    * @param name The name of the attribute to set.
    * @param val  The value to be set.
    */
   template<typename TYPE>
   void setAttrib(const std::string& name, TYPE val)
   {
      mAttribMap[name] = val;
   }

   /**
    * Gets an attribute value.
    *
    * If not previously set, then return the value of a default constructed
    * instance of the given type.
    */
   template<typename TYPE>
   TYPE getAttrib(const std::string& name)
   {
      TYPE ret_val;     // Construct default return value
      if(attribExists(name))
      {
         boost::any any_val = mAttribMap[name];
         try
         {
            ret_val = boost::any_cast<TYPE>(any_val);
         }
         catch(const boost::bad_any_cast&)
         {
            ret_val = TYPE();    // Default value
         }
      }
      return ret_val;
   }

   /**
    * Does the named attribute exist?
    * @return \c true if it exists.
    */
   bool attribExists(const std::string& name)
   {
      attrib_map_t::iterator found = mAttribMap.find(name);
      return (found != mAttribMap.end());
   }

private:
   typedef std::map<std::string, boost::any> attrib_map_t;
   attrib_map_t   mAttribMap;
};

} // namespace vpr


#endif
