#ifndef _VPR_ATTRIBUTE_MAP_BASE_H
#define _VPR_ATTRIBUTE_MAP_BASE_H

#include <vpr/vprConfig.h>
#include <map>


#include <boost/any.hpp>

namespace vpr
{

/** Basic interface for adding attributes to classes.
*
* Useful for having custom setter/getter type settings
* that are only used in certain contexts.
*
* Based upon boost::any type.
*/
class AttributeMapBase
{
public:
   /** Set an attribute.
   * Set the attribute of name to value of val
   */
   template<typename TYPE>
   void setAttrib(std::string name, TYPE val)
   {
      mAttribMap[name] = val;
   }

   /** Get an attribute value
   *
   * If not previously set, then return the value
   * of a default constructed instance of the given type
   */
   template<typename TYPE>
   TYPE getAttrib(std::string name)
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

   /** Does the named attribute exist
   * @return true if it exists
   */
   bool attribExists(std::string name)
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

