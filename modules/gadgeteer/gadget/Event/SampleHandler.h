/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef _GADGET_SAMPLE_HANDLER_H_
#define _GADGET_SAMPLE_HANDLER_H_

#include <gadget/gadgetConfig.h>

#include <vector>

#include <gadget/Type/ProxyTraits.h>


namespace gadget
{

/**
 * The default handler of device samples. This will work for any device type
 * that uses std::vector<gadget::DeviceData<T> > to record samples.
 *
 * @since 2.1.6
 */
template<typename ProxyType>
struct DefaultSampleHandler
{
   typedef typename ProxyTraits<ProxyType>::device_data_type device_data_type;
   typedef std::vector<device_data_type>                     sample_type;
   typedef typename ProxyTraits<ProxyType>::raw_data_type    raw_data_type;

   const raw_data_type& getData(const sample_type& samples,
                                const unsigned int unit)
   {
      return samples[unit].getValue();
   }
};

}


#endif /* _GADGET_SAMPLE_HANDLER_H_ */
