/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _VRJ_D3D_SIM_INTERFACE_FACTORY_H_
#define _VRJ_D3D_SIM_INTERFACE_FACTORY_H_

#include <vrj/Draw/Direct3D/Config.h>
#include <string>
#include <vpr/Util/Factory.h>
#include <vpr/Util/Singleton.h>
#include <vrj/Draw/DrawSimInterface.h>

/**
 * Registers a creator for the DrawSimInterface implementation classes.
 *
 * @pre Requires that the method std::string getElementType() be defined for
 *      class SimIfType.
 *
 * Ex: VRJ_REGISTER_D3D_SIM_INTERFACE_CREATOR(simulators::PySim)
 */
#define VRJ_REGISTER_D3D_SIM_INTERFACE_CREATOR(SimIfType) \
class SimIfType; \
const bool reg_ctr_ ## SimIfType = \
   vrj::D3dSimInterfaceFactory::instance()-> \
      registerCreator(SimIfType::getElementType(), \
                      vpr::CreateProduct<vrj::DrawSimInterface, SimIfType>);

namespace vrj
{

/** \class D3dSimInterfaceFactory D3dSimInterfaceFactory.h vrj/Draw/Direct3D/D3dSimInterfaceFactory.h
 *
 * Simulator renderer factory.
 *
 * @since 2.1.16
 */
class VJ_D3D_CLASS_API D3dSimInterfaceFactory :
   public vpr::Factory<DrawSimInterface, std::string>
{
public:
   vprSingletonHeader(D3dSimInterfaceFactory);
};

} // End of vrj namespace

#endif /* _VRJ_D3D_SIM_INTERFACE_FACTORY_H_ */
