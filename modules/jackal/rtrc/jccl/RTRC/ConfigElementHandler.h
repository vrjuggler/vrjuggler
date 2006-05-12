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

#ifndef _JCCL_CONFIG_ELEMENT_HANDLER_H_
#define _JCCL_CONFIG_ELEMENT_HANDLER_H_

#include <jccl/jcclConfig.h>
#include <jccl/Config/ConfigElementPtr.h>

namespace jccl
{

/** \class ConfigElementHandler ConfigElementHandler.h jccl/RTRC/ConfigElementHandler.h
 *
 * Abstract base class for all classes that can handle ConfigElement objects.
 * Any class supporting this interface can be dynamically reconfigured by
 * the JCCL Configuration Manager.
 *
 * The idea is that a subclass overrides configCanHandle() to recognize those
 * elements types that the derived class cares about.  Then override
 * configAdd() and configRemove() to process instances of those elements.
 *
 * The actual work of checking the list of pending add and remove requests
 * and passing them to the methods of this interface is done by
 * configProcessPending().  This can also be overriden to provide special
 * behavior, but this is strongly discouraged.  The default implementation
 * should be sufficient for almost any conceivable dynamic reconfiguration
 * need.
 *
 * @see ConfigManager
 */
class JCCL_CLASS_API ConfigElementHandler
{

public:
   virtual ~ConfigElementHandler()
   {
      /* Do nothing. */ ;
   }

   /**
    * Checks if this handler can process the given config element.
    * Typically, an implementation of this method will check the element's
    * definition ID to decide if it knows how to deal with it.
    *
    * @param element The current config element that is ready to be processed.
    *
    * @return true iff this handler can process element.
    */
   virtual bool configCanHandle(ConfigElementPtr element) = 0;

   /**
    * Processes any pending reconfiguration requests that we know how to
    * deal with.
    *
    * The default implementation does the following for each item in the
    * pending list:
    *
    * <code>
    *    for each pending item p in the pending list do
    *       if this->configCanHandle(p) AND p's dependencies are met
    *          retval = configAdd or configRemove (p)
    *          if retval == true
    *             remove request from pending
    *             add or remove p.element from active
    * </code>
    *
    * ConfigManager's pending list MUST be locked before this function
    * is called.  Typically, configProcessPending() will be called by
    * jccl::ConfigManager::attemptReconfiguration(), which takes care of
    * this automatically.
    *
    * @see ConfigManager
    */
   virtual int configProcessPending();

   /**
    * Adds the pending element to the configuration.
    *
    * @pre configCanHandle(element) == true.
    *
    * @param element A newly added config element to be processed.
    *
    * @return true is returned if and only if the given element was processed
    *         successfully.
    */
   virtual bool configAdd(ConfigElementPtr element) = 0;

   /**
    * Removes the pending element from the current configuration.
    *
    * @pre configCanHandle(element) == true.
    *
    * @param element A newly removed config element to be processed.
    *
    * @return true is returned if and only if the given element was processed
    *         successfully.
    */
   virtual bool configRemove(ConfigElementPtr element) = 0;
};

}

#endif
