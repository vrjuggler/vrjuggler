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


#ifndef _JCCL_CONFIG_CHUNK_HANDLER_H_
#define _JCCL_CONFIG_CHUNK_HANDLER_H_

#include <jccl/jcclConfig.h>

namespace jccl {

class ConfigChunk;
class ConfigManager;


/** Abstract base class for all classes that can handle ConfigChunks
 *  Any class supporting this interface can be dynamically reconfigured,
 *  for example by Jackal's ConfigurationManager.
 * 
 *  The idea is that you override configCanHandle() to recognize those
 *  chunks that your derived class cares about.  Then you override
 *  configAdd() and configRemove() to actually process those chunks.
 *
 *  The actually work of checking the list of pending add and remove
 *  requests and throwing them at configCanHandle() et. al. is done
 *  by configProcessPending.  This can also be overriden to provide
 *  special behavior, however this is strongly discouraged.  The default
 *  implementation should be sufficient for almost any conceivable
 *  dynamic reconfiguration need.
 */

class JCCL_CLASS_API ConfigChunkHandler {

public:

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(ConfigChunk* chunk) = 0;

   //: Process any pending reconfiguration that we can deal with
   // This function processes each pending reconfiguration in configuration manager
   // The default implementation does the following. <br>
   // if(this->configCanHandle && passesDepCheck)
   //    configAdd/configRemove(chunk)
   // if(success)
   //   add/remove from pending
   //   remove/add from current
   //
   //! ARGS: lockIt - Should we lock the cfg_mgr pending list
   //+       defaults to true.  The only time this should be false is 
   //+       if you override this function and still want to make use of it's abilities
   //+         (ex.  The kernel needs to do this because if has to call configProcessPending on other managers in addition to itself)
   //! RETURNS: Number of chunks it actually processes
   virtual int configProcessPending (ConfigManager* cfg_mgr);

   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   virtual bool configAdd(ConfigChunk* chunk) = 0;

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   virtual bool configRemove(ConfigChunk* chunk) = 0;
};

}; // namespace jccl


#endif
