#ifndef BANDWIDTHSTATADAPTER_H
#define BANDWIDTHSTATADAPTER_H

#include <vpr/vprConfig.h>
#include <vpr/Util/Status.h>

namespace vpr
{

/**
 * This is a template adapter that combines two IO Stats strategies together into a single class
 * 
 * It multiply inherits a new class from the two classes to create a single new class
 *
 * It overrides the virtual functions read_i, readn_i, and write_i to create new functions that call
 * the parent functions in the order that they were specified as template parameters
 * 
 * It does this by over riding the virtual functions read_i, readn_i, and write_i. 
 *
 * All types here must be based on the BaseIOStatsStrategy_i interface
 */
template < class BASE_ONE, class BASE_TWO >
class IOStatsStrategyAdapter : public BASE_ONE, public BASE_TWO
{
public:
    
   
   virtual void read_s(Status& status,
                         void* buffer, const vpr::Uint32 length,
                          vpr::Uint32& bytes_read,
                          const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      BASE_ONE::read_s(status, buffer, length, bytes_read, timeout);
      BASE_TWO::read_s(status, buffer, length, bytes_read, timeout);
   }

   virtual void readn_s(Status& status,
                          void* buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_read,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      BASE_ONE::readn_s(status, buffer, length, bytes_read, timeout);
      BASE_TWO::readn_s(status, buffer, length, bytes_read, timeout);
   }

   virtual void write_s(Status& status,
                          const void* buffer, const vpr::Uint32 length,
                           vpr::Uint32& bytes_written,
                           const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      BASE_ONE::write_s(status, buffer, length, bytes_written, timeout);
      BASE_TWO::write_s(status, buffer, length, bytes_written, timeout);
   }
};

}; // namespace vpr

#endif //BANDWIDTHSTATADAPTER_H

