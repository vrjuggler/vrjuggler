#include <vpr/IO/Stats/BaseIOStatsStrategy.h>
#include <vpr/IO/BlockIO.h>


void vpr::BaseIOStatsStrategy::setRealObject(vpr::BlockIO* real)
{ mRealObject = real; }
 
void vpr::BaseIOStatsStrategy::read_s(Status& status,
                                      void* buffer, const vpr::Uint32 length,
                                      vpr::Uint32& bytes_read,
                                      const vpr::Interval timeout)
{ 
   status = mRealObject->read_i(buffer, length, bytes_read, timeout);
}

void vpr::BaseIOStatsStrategy::readn_s(Status& status,
                                       void* buffer, const vpr::Uint32 length,
                                       vpr::Uint32& bytes_read,
                                       const vpr::Interval timeout)
{ 
   status = mRealObject->readn_i(buffer, length, bytes_read, timeout);
}

void vpr::BaseIOStatsStrategy::write_s(Status& status,
                                       const void* buffer, const vpr::Uint32 length,
                                       vpr::Uint32& bytes_written,
                                       const vpr::Interval timeout)
{ 
   status = mRealObject->write_i(buffer, length, bytes_written, timeout);
}
