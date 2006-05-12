/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VRJ_PF_SIM_INTERFACE_FACTORY_H_
#define _VRJ_PF_SIM_INTERFACE_FACTORY_H_

#include <vrj/vrjConfig.h>
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
 * Ex: VRJ_REGISTER_SIM_INTERFACE_CREATOR(simulators::PySim)
 */
#define VRJ_REGISTER_PF_SIM_INTERFACE_CREATOR(SimIfType) \
class SimIfType; \
const bool reg_ctr_ ## SimIfType = \
   vrj::PfSimInterfaceFactory::instance()-> \
      registerCreator(SimIfType::getElementType(), \
                      vpr::CreateProduct<vrj::DrawSimInterface, SimIfType>);

namespace vrj
{

/** \class PfSimInterfaceFactory PfSimInterfaceFactory.h vrj/Draw/Pf/PfSimInterfaceFactory.h
 *
 * Simulator renderer factory.
 */
class VJ_CLASS_API PfSimInterfaceFactory :
   public vpr::Factory<DrawSimInterface, std::string>
{
public:
   vprSingletonHeader(PfSimInterfaceFactory);
};

} // End of vrj namespace

#endif /* _VRJ_DRAW_SIM_INTERFACE_FACTORY_H_ */
