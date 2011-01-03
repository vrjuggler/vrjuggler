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

#ifndef _VRJ_DIRECT3D_SIM_INTERFACE_FACTORY_H_
#define _VRJ_DIRECT3D_SIM_INTERFACE_FACTORY_H_

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
   vrj::direct3d::SimInterfaceFactory::instance()-> \
      registerCreator(SimIfType::getElementType(), \
                      vpr::CreateProduct<vrj::DrawSimInterface, SimIfType>);

namespace vrj
{

namespace direct3d
{

/** \class vrj::direct3d::SimInterfaceFactory SimInterfaceFactory.h vrj/Draw/Direct3D/SimInterfaceFactory.h
 *
 * Simulator renderer factory.
 *
 * @since 2.1.16
 *
 * @note This class was renamed from vrj::D3dSimInterfaceFactory in VR Juggler
 *       2.3.11.
 */
class VJ_D3D_CLASS_API SimInterfaceFactory :
   public vpr::Factory<DrawSimInterface, std::string>
{
public:
   vprSingletonHeader(SimInterfaceFactory);
};

} // End of direct3d namespace

} // End of vrj namespace


#endif /* _VRJ_DIRECT3D_SIM_INTERFACE_FACTORY_H_ */
