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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_PROXY_TRAITS_H_
#define _GADGET_PROXY_TRAITS_H_

#include <gadget/gadgetConfig.h>


namespace gadget
{

/** \struct ProxyTraits ProxyTraits.h gadget/Type/ProxyTraits.h
 *
 * The input device proxy traits template type. As all built in device proxy
 * types use gadget::TypedProxy, there should not be a need to specialize this
 * type any further. However, that option exists for atypical cases should
 * they arise.
 *
 * @see gadget::TypedProxy
 *
 * @since 2.1.2
 */
template<typename T>
struct ProxyTraits
{
   typedef T                                     proxy_type;
   typedef boost::shared_ptr<proxy_type>         proxy_ptr_type;
   typedef typename proxy_type::device_type      device_type;
   typedef typename proxy_type::device_ptr_type  device_ptr_type;
   typedef typename proxy_type::device_data_type device_data_type;
   typedef typename proxy_type::raw_data_type    raw_data_type;
};

}


#endif /* _GADGET_PROXY_TRAITS_H_ */
