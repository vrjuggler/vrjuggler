#ifndef BASEIOSTATS_H
#define BASEIOSTATS_H

#include <vpr/vprConfig.h>
#include <vpr/Util/Status.h>
#include <vpr/Util/Interval.h>

namespace vpr
{

class BlockIO;    // forward declare

/**
 * Base interface for IO Stat collection
 * 
 * This is the interface that the BlockIO class expects to deal with.
 * We add on other stats features using mixins that are added by a template composition adapter
 */
class VPR_CLASS_API BaseIOStatsStrategy
{
public:
   BaseIOStatsStrategy()
   {
       mRealObject = NULL;
   }

   virtual ~BaseIOStatsStrategy()
   {;}

   void setRealObject(vpr::BlockIO* real);

   enum { IS_NULL = 0 };

public:
   virtual void read_s(Status& status,
                         void* buffer, const size_t length,
                          ssize_t& bytes_read,
                          const vpr::Interval timeout = vpr::Interval::NoTimeout);
   
   virtual void readn_s(Status& status,
                          void* buffer, const size_t length,
                           ssize_t& bytes_read,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout);

   virtual void write_s(Status& status,
                          const void* buffer, const size_t length,
                           ssize_t& bytes_written,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout);
      
protected:
    vpr::BlockIO*   mRealObject;
};

/**
 * NULL strategy
 * 
 * NOTE: Must derive from BaseIOStatsStrategy because the compiler still expects that interface
 */
class NullIOStatsStrategy : public BaseIOStatsStrategy
{
public:
   enum { IS_NULL = 1 };
};

}; // namespace vpr

#endif //BASEIOSTATS_H


