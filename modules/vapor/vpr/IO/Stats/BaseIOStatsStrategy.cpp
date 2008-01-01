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

#include <vpr/IO/Stats/BaseIOStatsStrategy.h>
#include <vpr/IO/BlockIO.h>


namespace vpr
{

BaseIOStatsStrategy::BaseIOStatsStrategy()
   : mRealObject(NULL)
{
    /* Do nothing. */ ;
}

BaseIOStatsStrategy::~BaseIOStatsStrategy()
{
   mRealObject = NULL;
}

void BaseIOStatsStrategy::setRealObject(vpr::BlockIO* real)
{
   mRealObject = real;
}

void BaseIOStatsStrategy::read_s(void* buffer, const vpr::Uint32 length,
                                 vpr::Uint32& bytes_read,
                                 const vpr::Interval& timeout)
{
   bytes_read = mRealObject->read_i(buffer, length, timeout);
}

void BaseIOStatsStrategy::readn_s(void* buffer, const vpr::Uint32 length,
                                  vpr::Uint32& bytes_read,
                                  const vpr::Interval& timeout)
{
   bytes_read = mRealObject->readn_i(buffer, length, timeout);
}

void BaseIOStatsStrategy::write_s(const void* buffer, const vpr::Uint32 length,
                                  vpr::Uint32& bytes_written,
                                  const vpr::Interval& timeout)
{
   bytes_written = mRealObject->write_i(buffer, length, timeout);
}

NullIOStatsStrategy::NullIOStatsStrategy()
{
   /* Do nothing. */ ;
}

NullIOStatsStrategy::~NullIOStatsStrategy()
{
   /* Do nothing. */ ;
}

} // namespace vpr
