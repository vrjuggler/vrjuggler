/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>

#include <gad/Type/AnalogProxy.h>
#include <gad/Type/DigitalProxy.h>
#include <gad/Type/PositionProxy.h>
#include <gad/Type/GloveProxy.h>
#include <gad/Type/GestureProxy.h>
#include <gad/Type/KeyboardProxy.h>
#include <gad/ProxyDepChecker.h>

namespace vrj
{

bool ProxyDepChecker::canHandle(ConfigChunk* chunk)
{
   std::string chunk_type = (std::string)chunk->getType();

   bool ret_val;
   ret_val = ((chunk_type == AnalogProxy::getChunkType()) ||
              (chunk_type == DigitalProxy::getChunkType()) ||
              (chunk_type == GestureProxy::getChunkType()) ||
              (chunk_type == GloveProxy::getChunkType()) ||
              (chunk_type == KeyboardProxy::getChunkType()) ||
              (chunk_type == PositionProxy::getChunkType()));

   return ret_val;
}


};
