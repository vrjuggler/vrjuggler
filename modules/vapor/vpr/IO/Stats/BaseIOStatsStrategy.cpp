#include <vpr/IO/Stats/BaseIOStatsStrategy.h>
#include <vpr/IO/BlockIO.h>


namespace vpr
{

void BaseIOStatsStrategy::setRealObject(vpr::BlockIO* real)
{ mRealObject = real; }
 
void BaseIOStatsStrategy::read_s(Status& status,
                                    void* buffer, const size_t length,
                                    ssize_t& bytes_read,
                                    const vpr::Interval timeout = vpr::Interval::NoTimeout)
{ 
   status = mRealObject->read_i(buffer, length, bytes_read, timeout);
}

void BaseIOStatsStrategy::readn_s(Status& status,
                                    void* buffer, const size_t length,
                                    ssize_t& bytes_read,
                                    const vpr::Interval timeout = vpr::Interval::NoTimeout)
{ 
   status = mRealObject->readn_i(buffer, length, bytes_read, timeout);
}

void BaseIOStatsStrategy::write_s(Status& status,
                                    const void* buffer, const size_t length,
                                    ssize_t& bytes_written,
                                    const vpr::Interval timeout = vpr::Interval::NoTimeout)
{ 
   status = mRealObject->write_i(buffer, length, bytes_written, timeout);
}
      
}; // namespace

