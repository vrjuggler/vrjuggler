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


package VjControl;

import java.util.EventObject;

import VjConfig.ChunkDescDB;
import VjConfig.ConfigChunkDB;


/** Event for notification of new classes added to ComponentFactory.
 *  ComponentFactoryListeners receive ComponentFactoryEvents for every
 *  VjComponent class that is registered with the ComponentFactory.
 *  Before Instantiation ConfigChunks were functional this was how all
 *  VjComponents were created - by someone listening and instantiating
 *  them when they became available.
 *  <p>
 *  With the improved Configuration/Dependency-checking semantics, this
 *  notification is less useful; most components will simply want to 
 *  wait for VjComponent.addConfig() to throw something useful at them.
 */
public class ComponentFactoryEvent extends EventObject {
    
    protected String component_class_name;
    protected Class  component_class;

    public ComponentFactoryEvent (Object _source,
                                  String _component_class_name, 
                                  Class _component_class) {
	super(_source);

        component_class = _component_class;
	component_class_name = _component_class_name;
    }


    public String getComponentClassName () {
        return component_class_name;
    }


    public Class getComponentClass () {
        return component_class;
    }
}



