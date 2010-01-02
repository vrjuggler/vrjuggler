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

#ifndef _VRJ_OPENGL_SIM_INTERFACE_FACTORY_H_
#define _VRJ_OPENGL_SIM_INTERFACE_FACTORY_H_

#include <vrj/Draw/OpenGL/Config.h>
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
 * Ex: VRJ_REGISTER_GL_SIM_INTERFACE_CREATOR(simulators::PySim)
 */
#define VRJ_REGISTER_GL_SIM_INTERFACE_CREATOR(SimIfType) \
class SimIfType; \
const bool reg_ctr_ ## SimIfType = \
   vrj::opengl::SimInterfaceFactory::instance()-> \
      registerCreator(SimIfType::getElementType(), \
                      vpr::CreateProduct<vrj::DrawSimInterface, SimIfType>);

namespace vrj
{

namespace opengl
{

/** \class vrj:opengl::SimInterfaceFactory SimInterfaceFactory.h vrj/Draw/OpenGL/SimInterfaceFactory.h
 *
 * Simulator renderer factory.
 *
 * @note This class was renamed from vrj::GlSimInterfaceFactory in VR Juggler
 *       2.3.11.
 */
class VJ_OGL_CLASS_API SimInterfaceFactory :
   public vpr::Factory<vrj::DrawSimInterface, std::string>
{
public:
   vprSingletonHeader(SimInterfaceFactory);
};

} // End of opengl namespace

} // End of vrj namespace


#endif /* _VRJ_OPENGL_SIM_INTERFACE_FACTORY_H_ */
